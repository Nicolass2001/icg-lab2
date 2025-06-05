#include "rayo.h"
#include "color_rgb.h"

#ifndef OBJETO_H
#define OBJETO_H

class Objeto_RR
{
protected:
    ColorRGB colorAmbiente;

public:
    ColorRGB getColorAmbiente() const { return colorAmbiente; }
    virtual Vector calcularNormal(Rayo_RR rayo) = 0;
    virtual bool calcularInterseccion(Rayo_RR rayo, Vector *puntoInterseccion) = 0;
};

using ObjetoPtr = std::shared_ptr<Objeto_RR>;

#endif // OBJETO_H