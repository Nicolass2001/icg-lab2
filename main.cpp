#include <iostream>
#include <FreeImage.h>
#include "escena.h"
#include "fileManager.h"
#include <SDL.h>

Escena_RR escena("config.xml");

Color_RR traza_RR(Rayo_RR rayo, int profundidad);

Color_RR sombra_RR(ObjetoPtr objeto, Rayo_RR rayo, Vector punto, Vector normal, int profundidad)
{
    Color_RR color;
    color.setComponenteAmbiente(objeto->getColorAmbiente(), objeto->getCoeficienteAmbiente());
    escena.calcularColorIluminacion(objeto, rayo, punto, normal, color);
    if (profundidad < PROFUNDIDAD_MAXIMA)
    {
        // Calcular Reflexion
        if (objeto->getCoeficienteReflexion().length() > 0.0f)
        {
            Vector direccionReflexion = rayo.getDireccion() - normal * 2.0f * (rayo.getDireccion().dot(normal));
            Rayo_RR rayoReflexion(punto + normal * EPSILON, direccionReflexion.normalize());
            Color_RR colorReflexion = traza_RR(rayoReflexion, profundidad + 1);
            color.setComponenteReflexion(colorReflexion.getColorTotal(), objeto->getCoeficienteReflexion());
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
    FreeImage_Unload(bitmap);
    FreeImage_DeInitialise();

    while (1) {
        if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
            break;
    }

    // SDL
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
