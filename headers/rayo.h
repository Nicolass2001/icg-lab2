#include "vector.h"

#ifndef RAYO_H
#define RAYO_H

class Rayo_RR
{
private:
    Vector origen;    // Punto de origen del rayo
    Vector direccion; // Dirección del rayo
public:
    Rayo_RR();
    Rayo_RR(Vector origen, Vector direccion);
    Vector getOrigen();
    Vector getDireccion();
    friend std::ostream &operator<<(std::ostream &os, const Rayo_RR &rayo);
};

Rayo_RR::Rayo_RR() {}

Rayo_RR::Rayo_RR(Vector origen, Vector direccion)
    : origen(origen), direccion(direccion.normalize()) {}

Vector Rayo_RR::getOrigen()
{
    return origen;
}

Vector Rayo_RR::getDireccion()
{
    return direccion;
}

std::ostream &operator<<(std::ostream &os, const Rayo_RR &rayo)
{
    os << "Rayo: Origen(" << rayo.origen.x() << ", " << rayo.origen.y() << ", " << rayo.origen.z()
       << "), Direccion(" << rayo.direccion.x() << ", " << rayo.direccion.y() << ", " << rayo.direccion.z() << ")";
    return os;
}

#endif // RAYO_H