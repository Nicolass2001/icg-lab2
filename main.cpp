#include <iostream>
#include <FreeImage.h>
#include "escena.h"
#include "fileManager.h"

Escena_RR escena;

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
            Rayo_RR rayoReflexion(punto + normal * EPSILON, direccionReflexion.normalize(), rayo.getIndiceRefraccion());
            Color_RR colorReflexion = traza_RR(rayoReflexion, profundidad + 1);
            color.setComponenteReflexion(colorReflexion.getColorTotal(), objeto->getCoeficienteReflexion());
        }
        // Calcular Refraccion
        if (objeto->getCoeficienteTransparencia().length() > 0.0f)
        {
            Vector puntoRefraccion = punto - normal * EPSILON;
            float indiceRefraccionIncidente = rayo.getIndiceRefraccion();
            float indiceRefraccionTransmitido = escena.indiceRefraccion(puntoRefraccion);
            float indiceRefraccionRelativo = indiceRefraccionIncidente / indiceRefraccionTransmitido;
            Vector direccionRefraccion;
            if (indiceRefraccionRelativo == 1)
            {
                direccionRefraccion = rayo.getDireccion();
                if (test == 0)
                {
                    std::cout << "Indice de refracción incidente: " << indiceRefraccionIncidente << std::endl;
                    std::cout << "direccionRefraccion: " << direccionRefraccion << std::endl;
                }
            }
            else
            {
                std::cout << "Indice de refracción incidente: " << indiceRefraccionIncidente << std::endl;
                // Ley de Snell
                float cosenoIncidencia = -rayo.getDireccion().dot(normal);
                float senoCuadradoRefraccion = indiceRefraccionRelativo * indiceRefraccionRelativo * (1.0f - cosenoIncidencia * cosenoIncidencia);
                if (senoCuadradoRefraccion > 1.0f)
                {
                    // Total internal reflection
                    color.setComponenteTransparencia(ColorRGB(0, 0, 0), Vector(0, 0, 0));
                    return color;
                }
                float cosenoRefraccion = sqrt(1.0f - senoCuadradoRefraccion);
                direccionRefraccion = rayo.getDireccion() * indiceRefraccionRelativo + normal * (indiceRefraccionRelativo * cosenoIncidencia - cosenoRefraccion);
            }
            Rayo_RR rayoRefraccion(puntoRefraccion, direccionRefraccion.normalize(), indiceRefraccionTransmitido);
            Color_RR colorRefraccion = traza_RR(rayoRefraccion, profundidad + 1);
            if (test == 0)
            {
                std::cout << "colorRefraccion: " << colorRefraccion << std::endl;
            }
            test = 1;
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

    return 0;
}
