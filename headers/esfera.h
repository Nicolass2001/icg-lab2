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
    bool calcularInterseccion(Rayo_RR rayo, Vector *puntoInterseccion, Vector *normal) override;
};

using EsferaPtr = std::shared_ptr<Esfera_RR>;

Esfera_RR::Esfera_RR() {}

Esfera_RR::Esfera_RR(Vector centro, float radio, PropiedadesObjeto prop)
    : Objeto_RR(prop)
{
    this->centro = centro;
    this->radio = radio;
}

bool Esfera_RR::calcularInterseccion(Rayo_RR rayo, Vector *puntoInterseccion, Vector *normal)
{
    return false; // Por simplicidad, no implementamos la intersección de la esfera
}

#endif // ESFERA_H