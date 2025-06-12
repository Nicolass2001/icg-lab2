#include "vector.h"

#ifndef RAYO_H
#define RAYO_H

class Rayo_RR
{
private:
    Vector origen;          // Punto de origen del rayo
    Vector direccion;       // Dirección del rayo
    float indiceRefraccion; // Indice de refracción del medio
public:
    Rayo_RR();
    Rayo_RR(Vector origen, Vector direccion, float indiceRefraccion);
    Vector getOrigen();
    Vector getDireccion();
    float getIndiceRefraccion();
    friend std::ostream &operator<<(std::ostream &os, const Rayo_RR &rayo);
};

Rayo_RR::Rayo_RR() {}

Rayo_RR::Rayo_RR(Vector origen, Vector direccion, float indiceRefraccion)
    : origen(origen), direccion(direccion.normalize()), indiceRefraccion(indiceRefraccion) {}

Vector Rayo_RR::getOrigen()
{
    return origen;
}

Vector Rayo_RR::getDireccion()
{
    return direccion;
}

float Rayo_RR::getIndiceRefraccion()
{
    return indiceRefraccion;
}

std::ostream &operator<<(std::ostream &os, const Rayo_RR &rayo)
{
    os << "Rayo: Origen(" << rayo.origen.x() << ", " << rayo.origen.y() << ", " << rayo.origen.z()
       << "), Direccion(" << rayo.direccion.x() << ", " << rayo.direccion.y() << ", " << rayo.direccion.z() << ")";
    return os;
}

#endif // RAYO_H