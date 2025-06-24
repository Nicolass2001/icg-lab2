#include <iostream>
#include <FreeImage.h>
#include "fileManager.h"
#include "texturas.h"
#include "escena.h"
#include <SDL.h>

Escena_RR escena("config.xml");

Texturas texturas(3);

Color_RR traza_RR(Rayo_RR rayo, int profundidad, int eleccion);

Color_RR sombra_RR(ObjetoPtr objeto, Rayo_RR rayo, Vector punto, Vector normal, int profundidad, int eleccion)
{
    Color_RR color; // Color a devolver

    Rayo_RR rayo_r, rayo_t;    // Rayo reflejado, refractado y sombra
    Color_RR color_r, color_t; // color de los rayos reflejado y refractado

    // OPCIONAL TEXTURAS
    if (eleccion == 9 && objeto->getTipoObjeto() == 4)
    {
        objeto->setColoresTextura(punto, texturas.texturasCargadas[0]);
    }
    else if (eleccion == 9 && objeto->getTipoObjeto() == 2)
    {
        objeto->setColoresTextura(punto, texturas.texturasCargadas[0]);
    }
    else if (eleccion == 9 && objeto->getTipoObjeto() == 5)
    {
        objeto->setColoresTextura(punto, texturas.texturasCargadas[1]);
    }
    else if (eleccion == 9 && objeto->getTipoObjeto() == 3)
    {
        objeto->setColoresTextura(punto, texturas.texturasCargadas[2]);
    }
    else if (eleccion == 9 && objeto->getTipoObjeto() == 1)
    {
        objeto->setColoresTextura(punto, texturas.texturasCargadas[0]);
    }

    // Color = termino de ambiente
    if (eleccion == 1 || eleccion == 4 || eleccion == 9)
    {
        color.setComponenteAmbiente(objeto->getColorAmbiente(), objeto->getCoeficienteAmbiente());
    }

    // for (cada luz)
    // rayos = rayo desde punto a la luz;
    // if (el producto punto de normal y la dirección a la luz es positivo) {
    // calcular cuánta luz es bloqueada por superficie opacas y transparentes, y usarla
    // para escalar los términos difusos y especulares antes de añadirlos a color;

    if (eleccion == 1 || eleccion == 5 || eleccion == 6 || eleccion == 9)
    {
        escena.calcularColorIluminacion(objeto, rayo, punto, normal, color, eleccion);
    }

    if (profundidad < PROFUNDIDAD_MAXIMA)
    {

        // If objeto es reflejante
        if (objeto->getCoeficienteReflexion() > 0.0f && eleccion == 1 || eleccion == 7 || eleccion == 9)
        {

            Vector dirReflexion = rayo.getDireccion() - normal * 2.0f * (rayo.getDireccion().dot(normal));
            dirReflexion = dirReflexion.normalize();

            Rayo_RR rayo_r(punto + normal * EPSILON, dirReflexion, rayo.getIndiceRefraccion());

            Color_RR colorReflexion;

            if (eleccion != 9)
            {
                colorReflexion = traza_RR(rayo_r, profundidad + 1, 1);
            }
            else
            {
                colorReflexion = traza_RR(rayo_r, profundidad + 1, 9);
            }

            color.setComponenteReflexion(colorReflexion.getColorTotal(), objeto->getCoeficienteReflexion());
        }

        // If objeto es transparente

        // Si el indice de refraccion es distinto de 0, el objeto es transparente
        if (objeto->getIndiceRefraccion() > 0.0f && eleccion == 1 || eleccion == 8 || eleccion == 9)
        {
            // rayo en la direccion de refraccion

            float n1 = 1;                             // indice de refraccion del medio
            float n2 = objeto->getIndiceRefraccion(); // ind de refracc del objeto, en este caso vidrio 1.5

            bool entrando = (normal.dot(rayo.getDireccion()) < 0.0f);

            if (!entrando)
            { // Rayo saliente
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

            if (expr2 > 1)
            {
                Vector R = (I - normal * 2.0f * (I.dot(normal))).normalize();
                Rayo_RR rayoReflexion(punto + normal * EPSILON, R, rayo.getIndiceRefraccion());

                Color_RR colorRefraccion;

                if (eleccion != 9)
                {
                    colorRefraccion = traza_RR(rayoReflexion, profundidad + 1, 1);
                }
                else
                {
                    colorRefraccion = traza_RR(rayoReflexion, profundidad + 1, 9);
                }

                color.setComponenteReflexion(colorRefraccion.getColorTotal(), objeto->getCoeficienteReflexion());
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

            Color_RR colorRefraccion = traza_RR(rayo_t, profundidad + 1, 1);
            color.setComponenteTransparencia(colorRefraccion.getColorTotal(), objeto->getCoeficienteTransparencia());
        }
    }
    return color;
}

Color_RR traza_RR(Rayo_RR rayo, int profundidad, int eleccion)
{
    Vector interseccion;
    Vector normal;
    ObjetoPtr objMasCercano = escena.calcularInterseccionMasCercana(rayo, &interseccion, &normal);
    if (objMasCercano == nullptr)
    {
        return escena.getFondo();
    }

    Color_RR color;
    ColorRGB colorFigura(255, 255, 255);

    if (eleccion == 2)
    {
        color.setComponenteAmbiente(colorFigura, objMasCercano->getCoeficienteReflexion());
        return color;
    }
    else if (eleccion == 3)
    {
        color.setComponenteAmbiente(colorFigura, objMasCercano->getCoeficienteTransparencia());
        return color;
    }

    return sombra_RR(objMasCercano, rayo, interseccion, normal, profundidad, eleccion);
}

int main(int argc, char *argv[])
{

    int eleccion;

    std::cout << "Elija que imagen desea generar: " << std::endl;
    std::cout << "1 - Escena | 2 - Coeficientes reflexion | 3 - Coeficientes refraccion" << std::endl;
    std::cout << "4 - Componentes ambiente | 5 - Componentes difuso | 6 - Componentes especular" << std::endl;
    std::cout << "7 - Componentes reflexion | 8 - Componentes refraccion | 9 - Texturas" << std::endl;
    std::cin >> eleccion;

    // SDL
    // PAGINA DE REFERENCIA: https://stackoverflow.com/questions/20579658/how-to-draw-pixels-in-sdl-2-0

    SDL_Event event;
    SDL_Renderer *renderer;
    SDL_Window *window;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(800, 600, 0, &window, &renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    uint8_t *rojo = 0;
    uint8_t *verde = 0;
    uint8_t *azul = 0;

    // Inicializo FreeImage en texturas.h

    // Crear una imagen vacía de 24 bits
    FIBITMAP *bitmap = FreeImage_Allocate(IMAGE_WIDTH, IMAGE_HEIGHT, 24);
    if (!bitmap)
    {
        std::cerr << "No se pudo crear la imagen." << std::endl;
        FreeImage_DeInitialise();
        return 1;
    }

    Camara_RR camara = escena.getCamara();
    Color_RR colorPixel;
    // Llenar la imagen utilizando la función `colorFunction`
    for (int y = 0; y < IMAGE_HEIGHT; ++y)
    {
        for (int x = 0; x < IMAGE_WIDTH; ++x)
        {
            colorPixel = traza_RR(camara.generarRayo(x, y), 1, eleccion);

            // Convertir Color_RR a RGBQUAD
            RGBQUAD color = colorPixel.toRGBQUAD();
            FreeImage_SetPixelColor(bitmap, x, y, &color);

            // SDL
            rojo = &color.rgbRed;
            verde = &color.rgbGreen;
            azul = &color.rgbBlue;
            SDL_SetRenderDrawColor(renderer, *rojo, *verde, *azul, 255);
            SDL_RenderDrawPoint(renderer, x, 600 - y);
            if (x == 0)
                SDL_RenderPresent(renderer);
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
    texturas.liberarTexturas();
    FreeImage_Unload(bitmap);
    FreeImage_DeInitialise();

    while (1)
    {
        if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
            break;
    }

    // SDL
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
