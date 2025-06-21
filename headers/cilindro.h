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
    bool estaDentro(Vector punto) override;
    void setColoresTextura(Vector punto, datosTextura textura) override {};

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
    // PARA LA INTERSECCION CON LA SUPERFICIE LATERAL:
    // La ecuacion del cilindro en eje Y es:
    // (x−cx)^2 +(z−cz)^2 = r^2
    // Con:
    // C = (cx, cz) el centro del cilindro, r el radio (estos son atributos de la clase)

    // Rayo tiene origen O = (Ox, Oy, Oz) y direccion D = (Dx, Dy, Dz)
    // Posicion del rayo en el cilindro:
    // x(t)=Ox+Dx⋅t
    // z(t)=Oz+Dz⋅t
    // Voy a tener que reemplazar esos valores en la ecuacion del cilindro:
    // (x(t) - Cx)^2 + (z(t) - Cz)^2 = r^2
    // Expando y tengo la ecuacion a resolver:
    // [(Ox+Dx⋅t)−Cx]^2 + [(Oz+Dz⋅t)−Cz]^2 = r^2
    // Y encuentro los puntos t de interseccion (si los hay) y me quedo con el mas cercano

    Vector puntoInterseccionLateral, normalLateral;
    Vector puntoInterseccionBase, puntoInterseccionTapa;
    bool hayInterseccionLateral = false;
    bool hayInterseccionBase = false;
    bool hayInterseccionTapa = false;

    // Paso 1: Calculo de interseccion con superficie lateral
    float ox = rayo.getOrigen().x();
    float oy = rayo.getOrigen().y();
    float oz = rayo.getOrigen().z();

    float dx = rayo.getDireccion().x();
    float dy = rayo.getDireccion().y();
    float dz = rayo.getDireccion().z();

    float cx = base.x();
    float cz = base.z();

    // Resuelvo la ecuacion aplicando bhaskara
    float a = dx * dx + dz * dz;
    float b = 2 * ((ox - cx) * dx + (oz - cz) * dz);
    float c = (ox - cx) * (ox - cx) + (oz - cz) * (oz - cz) - radio * radio;

    float discriminante = b * b - (4 * a * c); // Si el discriminante de bhaskara da menor a 0 no hay solucion real

    if (discriminante >= 0)
    {
        float sqrt_disc = sqrt(discriminante);
        float t1 = (-b - sqrt_disc) / (2 * a);
        float t2 = (-b + sqrt_disc) / (2 * a);
        
        // Me voy a quedar con el t positivo mas cercano
        float t_cil = -1;
        if (t1 > 0) {
            t_cil = t1;
        } else if (t2 > 0) {
            t_cil = t2;
        }

        if (t_cil >= 0)
        {
            // El punto de interseccion esta dado por
            // P = O + D*t
            Vector punto = rayo.getOrigen() + rayo.getDireccion() * t_cil;

            // Veo que este entre la altura definida para mi cilindro
            float distanciaAltura = (punto - base).dot(direccion);
            if (distanciaAltura >= 0 && distanciaAltura <= altura)
            {
                puntoInterseccionLateral = punto;
                normalLateral = (punto - base - direccion * distanciaAltura).normalize();
                hayInterseccionLateral = true;
            }
        }
    }

    // Paso 2: Calculo de interseccion con la base
    hayInterseccionBase = interseccionConTapa(base, direccion * (-1.0f), rayo, &puntoInterseccionBase);

    // Paso 3: Calculo de interseccion con la base
    Vector centroTapa = base + direccion * altura;
    hayInterseccionTapa = interseccionConTapa(centroTapa, direccion, rayo, &puntoInterseccionTapa);

    // Paso 4: Determinar interseccion mas cercana
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
    // La escuacion para interseccion entre rayo y plano es asi:
    // t = - (O-Po).N / D.N
    // Rayo tiene origen O y direccion D
    // Plano definido por un punto (origen) y una normal (estos son los atributos de la clase)
    // Primero veo que el rayo intersecte con el plano, y luego que sea dentro del area de la tapa

    float denominador = normalTapa.dot(rayo.getDireccion());

    // Si el denominador se aproxima a 0, entonces el rayo es paralelo a la tapa
    if (glm::abs(denominador) < 1e-10f)
        return false;

    // Calculo nominador de la ecuacion: (O-Po).N
    float nominador = normalTapa.dot(centroTapa - rayo.getOrigen());

    float t = nominador / denominador;

    if (t < 0)
        return false;

    // Calculo punto de interseccion P = O + t*D
    Vector punto = rayo.getOrigen() + rayo.getDireccion() * t;

    // Veo si el punto esta en la tapa
    if ((punto - centroTapa).length() <= radio)
    {
        *puntoInterseccion = punto;
        return true;
    }

    return false;
}

bool Cilindro_RR::estaDentro(Vector punto)
{
    // Verifica si el punto está dentro del cilindro
    Vector delta = punto - base;
    float distanciaBase = delta.dot(direccion);
    if (distanciaBase < 0 || distanciaBase > altura)
        return false;

    Vector proyeccion = delta - direccion * distanciaBase;
    return proyeccion.length() <= radio;
}

#endif // CILINDRO_H
