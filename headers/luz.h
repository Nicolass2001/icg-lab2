#include <iostream>
#include "vector.h"
#include "color_rgb.h"

#ifndef LUZ_H
#define LUZ_H

class Luz
{
private:
    Vector posicion;
    Vector intensidad;

public:
    Luz();
    Luz(const Vector &posicion, const Vector &intensidad);
    Luz(const Luz &other);
    Vector getIntensidad() const;
    Vector getPosicion() const;
    friend std::ostream &operator<<(std::ostream &os, const Luz &luz);
    Vector getDirection(Vector punto) const;
    float getDistancia(Vector punto) const;
};

Luz::Luz() : posicion(Vector()), intensidad(Vector(1, 1, 1)) {}

Luz::Luz(const Vector &posicion, const Vector &intensidad)
    : posicion(posicion), intensidad(intensidad) {}

Luz::Luz(const Luz &other)
    : posicion(other.posicion), intensidad(other.intensidad) {}

Vector Luz::getIntensidad() const
{
    return intensidad;
}

Vector Luz::getPosicion() const
{
    return posicion;
}

std::ostream &operator<<(std::ostream &os, const Luz &luz)
{
    os << "Luz(intensidad: " << luz.intensidad
       << ", posicion: " << luz.posicion << ")";
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