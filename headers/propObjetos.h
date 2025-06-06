#include "color_rgb.h"
#include "globalVariables.h"

#ifndef PROPIEDADES_COLOR_OBJETO_H
#define PROPIEDADES_COLOR_OBJETO_H

class PropiedadesObjeto
{
public:
    float coeficienteAmbiente;
    ColorRGB colorAmbiente;
    float coeficienteReflexionDifusa;
    ColorRGB colorReflexionDifusa;
    float coeficienteReflexionEspecular;
    int brilloEspecular;
    ColorRGB colorReflexionEspecular;
    float coeficienteTransparencia;
    float coeficienteRefraccion;

    PropiedadesObjeto()
        : coeficienteAmbiente(DEFAULT_AMBIENT_COEFFICIENT), colorAmbiente(ColorRGB(0, 0, 0)),
          coeficienteReflexionDifusa(DEFAULT_DIFFUSE_REFLECTION_COEFFICIENT), colorReflexionDifusa(ColorRGB(0, 0, 0)),
          coeficienteReflexionEspecular(DEFAULT_SPECULAR_REFLECTION_COEFFICIENT), brilloEspecular(DEFAULT_SPECULAR_BRIGHTNESS),
          colorReflexionEspecular(ColorRGB(255, 255, 255)),
          coeficienteTransparencia(DEFAULT_TRANSPARENCY_COEFFICIENT), coeficienteRefraccion(DEFAULT_REFRACTION_COEFFICIENT) {}

    PropiedadesObjeto(float coefAmbiente, ColorRGB colorAmbiente,
                      float coefRefDifusa, ColorRGB colorRefDifusa,
                      float coefRefEspecular, int brilloEspecular,
                      ColorRGB colorRefEspecular,
                      float coefTransparencia, float coefRefraccion)
        : coeficienteAmbiente(coefAmbiente), colorAmbiente(colorAmbiente),
          coeficienteReflexionDifusa(coefRefDifusa), colorReflexionDifusa(colorRefDifusa),
          coeficienteReflexionEspecular(coefRefEspecular), brilloEspecular(brilloEspecular),
          colorReflexionEspecular(colorRefEspecular),
          coeficienteTransparencia(coefTransparencia), coeficienteRefraccion(coefRefraccion) {}
    PropiedadesObjeto(const ColorRGB &color)
        : coeficienteAmbiente(DEFAULT_AMBIENT_COEFFICIENT), colorAmbiente(color),
          coeficienteReflexionDifusa(DEFAULT_DIFFUSE_REFLECTION_COEFFICIENT), colorReflexionDifusa(color),
          coeficienteReflexionEspecular(DEFAULT_SPECULAR_REFLECTION_COEFFICIENT), brilloEspecular(DEFAULT_SPECULAR_BRIGHTNESS),
          colorReflexionEspecular(color),
          coeficienteTransparencia(DEFAULT_TRANSPARENCY_COEFFICIENT), coeficienteRefraccion(DEFAULT_REFRACTION_COEFFICIENT) {}
};

#endif // PROPIEDADES_COLOR_OBJETO_H