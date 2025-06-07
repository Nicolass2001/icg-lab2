#include "propObjetos.h"
#include "rayo.h"

#ifndef OBJETO_H
#define OBJETO_H

class Objeto_RR
{
protected:
    PropiedadesObjeto prop;

public:
    Objeto_RR() {}
    Objeto_RR(PropiedadesObjeto prop) : prop(prop) {}
    ColorRGB getColorAmbiente() const { return prop.colorAmbiente; }
    float getCoeficienteAmbiente() const { return prop.coeficienteAmbiente; }
    float getCoeficienteReflexionDifusa() const { return prop.coeficienteReflexionDifusa; }
    ColorRGB getColorReflexionDifusa() const { return prop.colorReflexionDifusa; }
    float getCoeficienteReflexionEspecular() const { return prop.coeficienteReflexionEspecular; }
    int getBrilloEspecular() const { return prop.brilloEspecular; }
    ColorRGB getColorReflexionEspecular() const { return prop.colorReflexionEspecular; }
    float getCoeficienteTransparencia() const { return prop.coeficienteTransparencia; }
    float getCoeficienteRefraccion() const { return prop.coeficienteRefraccion; }
    virtual bool calcularInterseccion(Rayo_RR rayo, Vector *puntoInterseccion, Vector *normal) = 0;
};

using ObjetoPtr = std::shared_ptr<Objeto_RR>;

#endif // OBJETO_H