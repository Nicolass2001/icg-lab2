#include "objeto.h"
#include "vector.h"
#include "globalVariables.h"

#ifndef PARED_H
#define PARED_H

class Pared_RR : public Objeto_RR
{
private:
    Vector centro;
    Vector normal;
    Vector anchoVec; // Vector con dirección del ancho de la pared y longitud igual al ancho de la pared
    Vector altoVec;  // Vector con dirección del alto de la pared y longitud igual al alto de la pared

public:
    Pared_RR();
    Pared_RR(Vector centro, Vector normal, float ancho, float alto, ColorRGB color);
    Vector calcularNormal(Rayo_RR rayo) override;
    bool calcularInterseccion(Rayo_RR rayo, Vector *puntoInterseccion) override;
};

using ParedPtr = std::shared_ptr<Pared_RR>;

Pared_RR::Pared_RR() {}

Pared_RR::Pared_RR(Vector centro, Vector normal, float ancho, float alto, ColorRGB color)
{
    this->centro = centro;
    this->normal = normal;
    this->colorAmbiente = color;

    // Ejes locales al plano de la pared
    Vector ejeX;
    Vector ejeY;
    if (normal.parallel(Vector(0, 1, 0)))
        ejeX = normal.cross(Vector(1, 0, 0)).normalize();
    else
        ejeX = normal.cross(Vector(0, 1, 0)).normalize();

    ejeY = normal.cross(ejeX).normalize();

    this->anchoVec = ejeX * ancho; // Ancho de la pared
    this->altoVec = ejeY * alto;   // Alto de la pared
}

Vector Pared_RR::calcularNormal(Rayo_RR rayo)
{
    return normal;
}

bool Pared_RR::calcularInterseccion(Rayo_RR rayo, Vector *puntoInterseccion)
{
    // Paso 1: Intersección con el plano
    float denominator = normal.dot(rayo.getDireccion());

    // Verifica si el rayo es paralelo al plano
    if (glm::abs(denominator) < 1e-6f)
    {
        return false; // No hay intersección
    }

    // Verifica si la normal y el rayo apuntan hacia el mismo lado (se ve desde atrás)
    if (denominator > 0)
    {
        return false; // El rayo viene desde atrás de la pared
    }

    // Calcula el parámetro t
    float t = normal.dot(centro - rayo.getOrigen()) / denominator;

    // Verifica si la intersección está en la dirección positiva del rayo
    if (t < 0)
    {
        return false; // El plano está detrás del rayo
    }

    // Calcula el punto de intersección
    *puntoInterseccion = rayo.getOrigen() + rayo.getDireccion() * t;

    // Paso 2: Proyección en el espacio del semiplano
    Vector relativePoint = *puntoInterseccion - centro;

    float u = relativePoint.dot(anchoVec.normalize()); // Coordenada en la dirección del ancho
    float v = relativePoint.dot(altoVec.normalize());  // Coordenada en la dirección del largo

    float width = anchoVec.length();
    float height = altoVec.length();

    // Paso 3: Verificar si está dentro de los límites
    if ((u >= -width / 2 && u <= width / 2) && (v >= -height / 2 && v <= height / 2))
        return true;
    return false;
}

#endif // PARED_H