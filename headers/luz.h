#include <iostream>
#include "vector.h"
#include "color_rgb.h"

#ifndef LUZ_H
#define LUZ_H

class Luz
{
private:
    float intensidad;
    Vector posicion;
    ColorRGB color;

public:
    Luz();
    Luz(float intensidad, const Vector &posicion, const ColorRGB &color);
    Luz(const Luz &other);
    float getIntensidad() const;
    Vector getPosicion() const;
    ColorRGB getColor() const;
    friend std::ostream &operator<<(std::ostream &os, const Luz &luz);
    Vector getDirection(Vector punto) const;
    float getDistancia(Vector punto) const;
};

Luz::Luz() : intensidad(0.0f), posicion(Vector()), color(ColorRGB()) {}

Luz::Luz(float intensidad, const Vector &posicion, const ColorRGB &color)
    : intensidad(intensidad), posicion(posicion), color(color) {}

Luz::Luz(const Luz &other)
    : intensidad(other.intensidad), posicion(other.posicion), color(other.color) {}

float Luz::getIntensidad() const
{
    return intensidad;
}

Vector Luz::getPosicion() const
{
    return posicion;
}

ColorRGB Luz::getColor() const
{
    return color;
}

std::ostream &operator<<(std::ostream &os, const Luz &luz)
{
    os << "Luz(intensidad: " << luz.intensidad
       << ", posicion: " << luz.posicion
       << ", color: " << luz.color << ")";
    return os;
}

Vector Luz::getDirection(Vector punto) const
{
    Vector direction = punto - posicion;
    direction = direction * -1.0f;
    return direction.normalize();
}

float Luz::getDistancia(Vector punto) const
{
    Vector distancia = punto - posicion;
    return distancia.length();
}

#endif // LUZ_H