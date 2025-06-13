#include <iostream>
#include <FreeImage.h>
#include "escena.h"
#include "fileManager.h"
#include <SDL.h>

Escena_RR escena("config.xml");

Color_RR traza_RR(Rayo_RR rayo, int profundidad);

Color_RR sombra_RR(ObjetoPtr objeto, Rayo_RR rayo, Vector punto, Vector normal, int profundidad)
{
    Color_RR color; // Color a devolver

    color.setComponenteAmbiente(objeto->getColorAmbiente(), objeto->getCoeficienteAmbiente());
    escena.calcularColorIluminacion(objeto, rayo, punto, normal, color);

    if (profundidad < PROFUNDIDAD_MAXIMA) {
        // Calcular Reflexion

        // If objeto es reflejante
        /*
        if (objeto->getCoeficienteReflexion().length() > 0.0f)
        {
            Vector direccionReflexion = rayo.getDireccion() - normal * 2.0f * (rayo.getDireccion().dot(normal));
            Rayo_RR rayoReflexion(punto + normal * EPSILON, direccionReflexion.normalize(), rayo.getIndiceRefraccion());
            Color_RR colorReflexion = traza_RR(rayoReflexion, profundidad + 1);
            color.setComponenteReflexion(colorReflexion.getColorTotal(), objeto->getCoeficienteReflexion());
        }
        */
        // Calcular Refraccion

        // If objeto es transparente

        // Si el indice de refraccion es distinto de 0, el objeto es transparente
        if (objeto->getIndiceRefraccion() > 0.0f) {
            // rayo en la direccion de refraccion

            //float n1 =  1.00029f; // ESTO VIENE A SER EL INDICE DE REFRACCION DEL AMBIENTE, EL DEL AIRE POR AHORA
            //float n2 = objeto->getIndiceRefraccion(); // EL IND DE REFR DEL OBJETO!!!

            float n1 = rayo.getIndiceRefraccion();
            float n2 = objeto->getIndiceRefraccion();

            bool entrando = (normal.dot(rayo.getDireccion()) < 0.0f);
            if (!entrando) {
                std::swap(n1, n2);
                normal = normal * -1.0f;
            }

            // La ley de Snell me dice que: sen(o1)/sen(o2) = n2/n1
            float n = n1 / n2;

            // DEL LIBRO: El vector de refraccion es: T = sen(o2)*M - cos(o1)*N 
            // Con M un vector unidad perpendicular a N en el plano del rayo incidente T
            // SIGO LOS PASOS DEL LIBRO PARA CALCULAR T, que lo simplifica a algo asi:
            // T = (η(N.I) − sqrt[1 − η^2 (1 − (N.I)^2)]) * N - η*I
  
 

            Vector rayo_normalizado = rayo.getDireccion().normalize();

            // calculo: −(N.I)
            float expr1 = -normal.dot(rayo_normalizado);

            // calculo: η^2 (1 − (N.I)^2)
            float expr2 = n * n * (1.0f - (expr1 * expr1));

            // En el libro dice que "...La reflexión interna total ocurre cuando la raíz 
            // cuadrada en la ecuación (14.30) es imaginaria."
            if (expr2 > 1) {
                // ACA EN VEZ DE HABER REFRACCION SOLO HAY REFLEXION INTERNA TOTAL (angulo critico.....)
                Vector R = rayo_normalizado - normal * 2.0f * (rayo_normalizado.dot(normal));
                Rayo_RR rayoReflexion(punto + normal * EPSILON, R.normalize(), rayo.getIndiceRefraccion());
                Color_RR colorReflexion = traza_RR(rayoReflexion, profundidad + 1);
                color.setComponenteReflexion(colorReflexion.getColorTotal(), objeto->getCoeficienteReflexion());
                return color;                
            }

            // calculo: sqrt[1 − η^2 (1 − (N.I)^2)]
            float expr3 = sqrt(1.0f - expr2);

            // A esta altura tengo:
            // T = (η(N.I) − expr3) * N - η*I

            // Calculo η(N.I)
            float expr4 = (normal.dot(rayo.getDireccion())) * n;

            // Calculo (η(N.I) − expr3) * N
            Vector expr5 = normal * (expr4 - expr3);
  
            Vector T = expr5 - (rayo.getDireccion() * n);
            T = T.normalize();

            Vector origenRefraccion = punto - normal * EPSILON;

            float indRefraccionTransmitido;

            // Para saber el indice de refraccion transmitido, 

            float indiceRefraccionTransmitido = escena.indiceRefraccion(origenRefraccion);


            // origen direccion indice_refr
            Rayo_RR rayoRefraccion(origenRefraccion, T, indiceRefraccionTransmitido);
          

            Color_RR colorRefraccion = traza_RR(rayoRefraccion, profundidad + 1);
            
            Vector direccionReflexion = rayo.getDireccion() - normal * 2.0f * (rayo.getDireccion().dot(normal));
            Rayo_RR rayoReflexion(punto + normal * EPSILON, direccionReflexion.normalize(), rayo.getIndiceRefraccion());
            Color_RR colorReflexion = traza_RR(rayoReflexion, profundidad + 1);
            
            //color.setComponenteReflexion(colorReflexion.getColorTotal(), objeto->getCoeficienteReflexion());



           
            color.setComponenteTransparencia(colorRefraccion.getColorTotal(), objeto->getCoeficienteTransparencia());

            

            // Buscar la salida del objeto
            
            Vector punto_salida, normal_salida;
            ObjetoPtr obj_salida = escena.calcularInterseccionMasCercana(rayoRefraccion, &punto_salida, &normal_salida);

            if (obj_salida && obj_salida.get() == objeto.get()) {
                float n3 = 1.0f; // índice de refracción del aire
                float n4 = objeto->getIndiceRefraccion();

                float n_rel = n4 / n3;
                Vector dir_in = rayoRefraccion.getDireccion().normalize();
                Vector normal_out = normal_salida;

                if (normal_out.dot(dir_in) > 0.0f) {
                    normal_out = normal_out * -1.0f;
                }

                float cos_i = -normal_out.dot(dir_in);
                float sin2_t = n_rel * n_rel * (1.0f - cos_i * cos_i);

                if (sin2_t < 1.0f) { // No hay reflexión interna total
                    float cos_t = sqrtf(1.0f - sin2_t);
                    Vector dir_refractada_salida = (dir_in * n_rel) + (normal_out * (n_rel * cos_i - cos_t));
                    dir_refractada_salida = dir_refractada_salida.normalize();

                    Vector origen_salida = punto_salida + dir_refractada_salida * EPSILON;

                    Rayo_RR rayo_salida(origen_salida, dir_refractada_salida, n3);


                    Color_RR color_salida = traza_RR(rayo_salida, profundidad + 2);
                    color.setComponenteTransparencia(color_salida.getColorTotal(), objeto->getCoeficienteTransparencia());
                }
            }
            


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
