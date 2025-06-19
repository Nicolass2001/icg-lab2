#include <iostream>
#include <FreeImage.h>
#include "escena.h"
#include "fileManager.h"
#include <SDL.h>

Escena_RR escena("config.xml");

Color_RR traza_RR(Rayo_RR rayo, int profundidad);

Color_RR sombra_RR(ObjetoPtr objeto, Rayo_RR rayo, Vector punto, Vector normal, int profundidad) {
    Color_RR color; // Color a devolver

    Rayo_RR rayo_r, rayo_t; // Rayo reflejado, refractado y sombra
    Color_RR color_r, color_t; // color de los rayos reflejado y refractado

    // Color = termino de ambiente
    color.setComponenteAmbiente(objeto->getColorAmbiente(), objeto->getCoeficienteAmbiente());

    // for (cada luz)
    // rayos = rayo desde punto a la luz;
    //if (el producto punto de normal y la dirección a la luz es positivo) {
    //calcular cuánta luz es bloqueada por superficie opacas y transparentes, y usarla
    //para escalar los términos difusos y especulares antes de añadirlos a color;

    escena.calcularColorIluminacion(objeto, rayo, punto, normal, color);

    if (profundidad < PROFUNDIDAD_MAXIMA) {

        // If objeto es reflejante
        if (objeto->getCoeficienteReflexion().length() > 0.0f) {

            Vector dirReflexion = rayo.getDireccion() - normal * 2.0f * (rayo.getDireccion().dot(normal));
            dirReflexion = dirReflexion.normalize();

            Rayo_RR rayo_r(punto + normal * EPSILON, dirReflexion, rayo.getIndiceRefraccion());
            Color_RR colorReflexion = traza_RR(rayo_r, profundidad + 1);
            color.setComponenteReflexion(colorReflexion.getColorTotal(), objeto->getCoeficienteReflexion());

        }
        
        // If objeto es transparente

        // Si el indice de refraccion es distinto de 0, el objeto es transparente
        if (objeto->getCoeficienteTransparencia().length() > 0.0f) {
            // rayo en la direccion de refraccion


            float n1 = 1; // indice de refraccion del medio 
            float n2 = objeto->getIndiceRefraccion(); // ind de refracc del objeto, en este caso vidrio 1.5

            bool entrando = (normal.dot(rayo.getDireccion()) < 0.0f);
            
            if (!entrando) { // Rayo saliente
                std::swap(n1, n2);
                normal = normal * -1.0f;
            }
            // La ley de Snell me dice que: sen(o1)/sen(o2) = n2/n1
            float n = n1 / n2;

            // DEL LIBRO: El vector de refraccion es: T = sen(o2)*M - cos(o1)*N 
            // Con M un vector unidad perpendicular a N en el plano del rayo incidente T
            // SIGO LOS PASOS DEL LIBRO PARA CALCULAR T, que lo simplifica a algo asi:
            // T = (η(N.I) − sqrt[1 − η^2 (1 − (N.I)^2)]) * N - η*I
 
            Vector I = rayo.getDireccion().normalize() * (-1.0f);

            // calculo: (N.I)
            float expr1 = normal.dot(I); 

            // calculo: η^2 (1 − (N.I)^2)
            float expr2 = n * n * (1.0f - (expr1 * expr1));

            // En el libro dice que "...La reflexión interna total ocurre cuando la raíz 
            // cuadrada en la ecuación (14.30) es imaginaria."
            
            if (expr2 > 1) {
                Vector R = (I - normal * 2.0f * (I.dot(normal))).normalize();
                Rayo_RR rayoReflexion(punto + normal * EPSILON, R, rayo.getIndiceRefraccion());
                Color_RR colorReflexion = traza_RR(rayoReflexion, profundidad + 1);
                color.setComponenteReflexion(colorReflexion.getColorTotal(), objeto->getCoeficienteReflexion());
                return color;              
            }
                

            // calculo: sqrt[1 − η^2 (1 − (N.I)^2)]
            float expr3 = sqrt(1.0f - expr2);

            // A esta altura tengo:
            // T = (η(N.I) − expr3) * N - η*I

            // Calculo η(N.I)
            float expr4 = (normal.dot(I)) * n;

            // Calculo (η(N.I) − expr3) * N
            Vector expr5 = normal * (expr4 - expr3);
  
            Vector T = expr5 - (I * n);
            T = T.normalize();

            Vector origenRefraccion = punto - normal * EPSILON;

            float indiceRefraccionTransmitido = escena.indiceRefraccion(origenRefraccion);
            Rayo_RR rayo_t(origenRefraccion, T, indiceRefraccionTransmitido);

            Color_RR colorRefraccion = traza_RR(rayo_t, profundidad + 1);
            color.setComponenteTransparencia(colorRefraccion.getColorTotal(), objeto->getCoeficienteTransparencia()); 
        }
    }
    return color;
}

Color_RR traza_RR(Rayo_RR rayo, int profundidad)
{
    Vector interseccion;
    Vector normal;
    ObjetoPtr objMasCercano = escena.calcularInterseccionMasCercana(rayo, &interseccion, &normal);
    if (objMasCercano == nullptr)
    {
        return escena.getFondo();
    }

    return sombra_RR(objMasCercano, rayo, interseccion, normal, profundidad);
}

int main(int argc, char* argv[])
{
    // SDL
    // PAGINA DE REFERENCIA: https://stackoverflow.com/questions/20579658/how-to-draw-pixels-in-sdl-2-0
    /*
    SDL_Event event;
    SDL_Renderer* renderer;
    SDL_Window* window;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(800, 600, 0, &window, &renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    uint8_t* rojo = 0;
    uint8_t* verde = 0;
    uint8_t* azul = 0;
    */

    // Inicializar FreeImage
    FreeImage_Initialise();

    // Crear una imagen vacía de 24 bits
    FIBITMAP *bitmap = FreeImage_Allocate(IMAGE_WIDTH, IMAGE_HEIGHT, 24);
    if (!bitmap)
    {
        std::cerr << "No se pudo crear la imagen." << std::endl;
        FreeImage_DeInitialise();
        return 1;
    }

    Camara_RR camara = escena.getCamara();
    // Llenar la imagen utilizando la función `colorFunction`
    for (int y = 0; y < IMAGE_HEIGHT; ++y)
    {
        for (int x = 0; x < IMAGE_WIDTH; ++x)
        {
            Color_RR colorPixel = traza_RR(camara.generarRayo(x, y), 1);
            // Convertir Color_RR a RGBQUAD
            RGBQUAD color = colorPixel.toRGBQUAD();
            FreeImage_SetPixelColor(bitmap, x, y, &color);

            // SDL
            /*
            rojo = &color.rgbRed;
            verde = &color.rgbGreen;
            azul = &color.rgbBlue;
            SDL_SetRenderDrawColor(renderer, *rojo, *verde, *azul, 255);
            SDL_RenderDrawPoint(renderer, x, 600 - y);
            if (x == 0)
                SDL_RenderPresent(renderer);
            */
        }
    }

    // Guardar la imagen en un archivo
    std::string outputPath = getPathToFile();
    
    if (FreeImage_Save(FIF_PNG, bitmap, outputPath.c_str(), 0))
    {
        std::cout << "Imagen guardada en: " << outputPath << std::endl;
    }
    else
    {
        std::cerr << "Error al guardar la imagen." << std::endl;
    }

    // Liberar memoria y finalizar FreeImage
    FreeImage_Unload(bitmap);
    FreeImage_DeInitialise();

    /*
    while (1) {
        if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
            break;
    }

    // SDL
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    */

    return 0;
}
