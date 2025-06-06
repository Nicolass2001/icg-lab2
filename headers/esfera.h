#include "objeto.h"
#include "vector.h"

#ifndef ESFERA_H
#define ESFERA_H

class Esfera_RR : public Objeto_RR
{
private:
    Vector centro;
    float radio;

public:
    Esfera_RR();
    Esfera_RR(Vector centro, float radio, PropiedadesObjeto prop);
    Vector calcularNormal(Rayo_RR rayo) override;
    bool calcularInterseccion(Rayo_RR rayo, Vector *puntoInterseccion) override;
};

using EsferaPtr = std::shared_ptr<Esfera_RR>;

Esfera_RR::Esfera_RR() {}

Esfera_RR::Esfera_RR(Vector centro, float radio, PropiedadesObjeto prop)
    : Objeto_RR(prop)
{
    this->centro = centro;
    this->radio = radio;
}

Vector Esfera_RR::calcularNormal(Rayo_RR rayo)
{
    // Implementación de la normal para una esfera
    // Aquí se debería calcular la normal en el punto de intersección
    // con el rayo, pero por simplicidad retornamos una normal nula.
    return Vector();
}

bool Esfera_RR::calcularInterseccion(Rayo_RR rayo, Vector *puntoInterseccion)
{
    return false; // Por simplicidad, no implementamos la intersección de la esfera
}

#endif // ESFERA_H