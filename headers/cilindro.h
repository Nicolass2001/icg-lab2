#include <algorithm>
#include "objeto.h"
#include "vector.h"

#ifndef CILINDRO_H
#define CILINDRO_H

class Cilindro_RR : public Objeto_RR
{
private:
    Vector base;      // Centro de la base del cilindro
    Vector direccion; // Dirección del eje del cilindro (normalizada)
    float radio;
    float altura;

public:
    Cilindro_RR();
    Cilindro_RR(Vector base, Vector direccion, float radio, float altura, PropiedadesObjeto prop);
    bool calcularInterseccion(Rayo_RR rayo, Vector *puntoInterseccion, Vector *normal) override;

private:
    bool interseccionConTapa(Vector centroTapa, Vector normalTapa, Rayo_RR rayo, Vector *puntoInterseccion);
};

using CilindroPtr = std::shared_ptr<Cilindro_RR>;

Cilindro_RR::Cilindro_RR() {}

Cilindro_RR::Cilindro_RR(Vector base, Vector direccion, float radio, float altura, PropiedadesObjeto prop)
    : Objeto_RR(prop)
{
    this->base = base;
    this->direccion = direccion.normalize();
    this->radio = radio;
    this->altura = altura;
}

bool Cilindro_RR::calcularInterseccion(Rayo_RR rayo, Vector *puntoInterseccion, Vector *normal)
{
    Vector puntoInterseccionLateral, normalLateral;
    Vector puntoInterseccionBase, puntoInterseccionTapa;
    bool hayInterseccionLateral = false;
    bool hayInterseccionBase = false;
    bool hayInterseccionTapa = false;

    // Paso 1: Calcular intersección con la superficie lateral del cilindro
    Vector rayoOrigenProy = rayo.getOrigen() - direccion * (rayo.getOrigen().dot(direccion));
    Vector rayoDirProy = rayo.getDireccion() - direccion * (rayo.getDireccion().dot(direccion));
    Vector baseProy = base - direccion * (base.dot(direccion));

    Vector delta = rayoOrigenProy - baseProy;

    float a = rayoDirProy.dot(rayoDirProy);
    float b = 2 * delta.dot(rayoDirProy);
    float c = delta.dot(delta) - radio * radio;

    float discriminante = b * b - 4 * a * c;

    if (discriminante >= 0)
    {
        float t0 = (-b - sqrt(discriminante)) / (2 * a);
        float t1 = (-b + sqrt(discriminante)) / (2 * a);

        if (t0 > t1)
            std::swap(t0, t1);

        float t = t0 >= 0 ? t0 : t1;
        if (t >= 0)
        {
            Vector punto = rayo.getOrigen() + rayo.getDireccion() * t;
            float distanciaAltura = (punto - base).dot(direccion);
            if (distanciaAltura >= 0 && distanciaAltura <= altura)
            {
                puntoInterseccionLateral = punto;
                normalLateral = (punto - base - direccion * distanciaAltura).normalize();
                hayInterseccionLateral = true;
            }
        }
    }

    // Paso 2: Calcular intersección con la base
    hayInterseccionBase = interseccionConTapa(base, direccion * (-1.0f), rayo, &puntoInterseccionBase);

    // Paso 3: Calcular intersección con la tapa superior
    Vector centroTapa = base + direccion * altura;
    hayInterseccionTapa = interseccionConTapa(centroTapa, direccion, rayo, &puntoInterseccionTapa);

    // Paso 4: Determinar la intersección más cercana
    float distanciaLateral = hayInterseccionLateral ? (puntoInterseccionLateral - rayo.getOrigen()).length() : FLT_MAX;
    float distanciaBase = hayInterseccionBase ? (puntoInterseccionBase - rayo.getOrigen()).length() : FLT_MAX;
    float distanciaTapa = hayInterseccionTapa ? (puntoInterseccionTapa - rayo.getOrigen()).length() : FLT_MAX;

    if (distanciaLateral < distanciaBase && distanciaLateral < distanciaTapa && hayInterseccionLateral)
    {
        *puntoInterseccion = puntoInterseccionLateral;
        *normal = normalLateral;
        return true;
    }
    else if (distanciaBase < distanciaTapa && hayInterseccionBase)
    {
        *puntoInterseccion = puntoInterseccionBase;
        *normal = direccion * (-1.0f); // Normal hacia afuera
        return true;
    }
    else if (hayInterseccionTapa)
    {
        *puntoInterseccion = puntoInterseccionTapa;
        *normal = direccion; // Normal hacia afuera
        return true;
    }

    return false;
}

bool Cilindro_RR::interseccionConTapa(Vector centroTapa, Vector normalTapa, Rayo_RR rayo, Vector *puntoInterseccion)
{
    float denominador = normalTapa.dot(rayo.getDireccion());

    // Verifica si el rayo es paralelo a la tapa
    if (glm::abs(denominador) < 1e-6f)
        return false;

    float t = normalTapa.dot(centroTapa - rayo.getOrigen()) / denominador;

    if (t < 0)
        return false;

    Vector punto = rayo.getOrigen() + rayo.getDireccion() * t;

    // Verifica si el punto está dentro del círculo de la tapa
    if ((punto - centroTapa).length() <= radio)
    {
        *puntoInterseccion = punto;
        return true;
    }

    return false;
}

#endif // CILINDRO_H
