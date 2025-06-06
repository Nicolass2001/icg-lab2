#include <iostream>
#include <FreeImage.h>
#include "escena.h"

Escena_RR escena;

Color_RR sombra_RR(ObjetoPtr objeto, Rayo_RR rayo, Vector punto, Vector normal, int profundidad)
{
    Color_RR color;
    color.setComponenteAmbiente(objeto->getColorAmbiente(), objeto->getCoeficienteAmbiente());
    escena.calcularColorIluminacion(objeto, rayo, punto, normal, color);
    return color;
}

Color_RR traza_RR(Rayo_RR rayo, int profundidad)
{
    Vector interseccion;
    ObjetoPtr objMasCercano = escena.calcularInterseccionMasCercana(rayo, &interseccion);
    if (objMasCercano == nullptr)
    {
        return escena.getFondo();
    }

    Vector normal = objMasCercano->calcularNormal(rayo);
    return sombra_RR(objMasCercano, rayo, interseccion, normal, profundidad);
}

int main()
{
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
        }
    }

    // Guardar la imagen en un archivo
    if (FreeImage_Save(FIF_PNG, bitmap, "output.png", 0))
    {
        std::cout << "Imagen guardada como 'output.png'" << std::endl;
    }
    else
    {
        std::cerr << "Error al guardar la imagen." << std::endl;
    }

    // Liberar memoria y finalizar FreeImage
    FreeImage_Unload(bitmap);
    FreeImage_DeInitialise();

    return 0;
}
