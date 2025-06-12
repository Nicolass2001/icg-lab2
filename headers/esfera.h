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
    bool estaDentro(Vector punto) override;
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
    // Vector desde el origen del rayo al centro de la esfera
    Vector L = centro - rayo.getOrigen();

    // Proyección del vector L en la dirección del rayo
    float tca = L.dot(rayo.getDireccion());

    // Distancia al punto más cercano desde el rayo al centro de la esfera
    float d2 = L.dot(L) - tca * tca;

    // Si d2 es mayor que el radio al cuadrado, no hay intersección
    if (d2 > radio * radio)
        return false;

    // Calcular la distancia desde el punto más cercano hasta el punto de intersección
    float thc = sqrt(radio * radio - d2);

    // Calcular los puntos de intersección
    float t0 = tca - thc; // Intersección "frontal"
    float t1 = tca + thc; // Intersección "posterior"

    // Asegurarnos de que t0 sea la menor distancia positiva
    if (t0 > t1)
        std::swap(t0, t1);

    // Si ambas distancias son negativas, el rayo no intersecta la esfera
    if (t0 < 0 && t1 < 0)
        return false;

    // Usar el punto de intersección más cercano en dirección positiva
    float t = (t0 < 0) ? t1 : t0;

    // Calcular el punto de intersección
    *puntoInterseccion = rayo.getOrigen() + rayo.getDireccion() * t;

    // Calcular la normal en el punto de intersección
    if ((rayo.getOrigen() - centro).length() < radio)
        *normal = (centro - *puntoInterseccion).normalize();
    else
        *normal = (*puntoInterseccion - centro).normalize();
    *normal = (*puntoInterseccion - centro).normalize();

    return true;
}

bool Esfera_RR::estaDentro(Vector punto)
{
    // Verifica si el punto está dentro de la esfera
    return (punto - centro).length() < radio;
}

#endif // ESFERA_H