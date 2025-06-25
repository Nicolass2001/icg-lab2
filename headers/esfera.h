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
    void setColoresTextura(Vector punto, datosTextura textura) override {};
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

    // Ecuacion de la esfera es:
    // ||P-C||^2 = r^2
    // Con:
    // P punto en superficie, C centro de la esfera, r radio

    // Rayo tiene origen O = (Ox, Oy, Oz) y direccion D = (Dx, Dy, Dz)
    // Sustituyo en ecuacion de la esfera:
    // ||O + tD - C||^2 = r^2
    // (O+tD−C).(O+tD−C) = r^2
    // (D.D)t^2 + 2D.(O-C)t + (O−C)⋅(O−C)−r^2 = 0
    // At^2 + Bt + S = 0
    // Si discriminante es < 0 no hay intersecc
    // Si discriminante es >=0 calculo las raices con bhaskara

    // Vector desde el origen del rayo al centro de la esfera
    Vector oc = rayo.getOrigen() - centro;

    float A = rayo.getDireccion().dot(rayo.getDireccion());
    float B = 2.0f * rayo.getDireccion().dot(oc);
    float S = oc.dot(oc) - radio * radio;

    float discriminante = B * B - (4 * A * S);

    if (discriminante < 0)
        return false;

    float sqrt_disc = sqrt(discriminante);
    float t1 = (-B - sqrt_disc) / (2 * A);
    float t2 = (-B + sqrt_disc) / (2 * A);

    float t_esfera = -1;
    if (t1 > 0)
    {
        t_esfera = t1;
    }
    else if (t2 > 0)
    {
        t_esfera = t2;
    }
    else
    {
        return false;
    }

    // El punto de interseccion esta dado por
    // P = O + D*t
    *puntoInterseccion = rayo.getOrigen() + rayo.getDireccion() * t_esfera;
    *normal = (*puntoInterseccion - centro).normalize();

    return true;
}

#endif // ESFERA_H