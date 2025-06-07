#include <vector>
#include "vector.h"
#include "color.h"
#include "pared.h"
#include "camara.h"
#include "esfera.h"
#include "objeto.h"
#include "luz.h"

#ifndef ESCENA_H
#define ESCENA_H

class Escena_RR
{
private:
    Camara_RR camara;
    std::vector<ObjetoPtr> objetos;
    std::vector<Luz> luces;
    Color_RR fondo;

public:
    Escena_RR();
    Color_RR getFondo();
    Camara_RR getCamara();
    ObjetoPtr calcularInterseccionMasCercana(Rayo_RR rayo, Vector *punto, Vector *normal);
    void calcularColorIluminacion(ObjetoPtr objeto, Rayo_RR rayo, Vector punto, Vector normal, Color_RR &color);
};

Color_RR Escena_RR::getFondo()
{
    return fondo;
}

Camara_RR Escena_RR::getCamara()
{
    return camara;
}

Escena_RR::Escena_RR()
{
    camara = Camara_RR(
        Vector(-10, 0, 0), // Posición de la cámara
        Vector(1, 0, 0),   // Dirección hacia adelante de la cámara
        Vector(0, 1, 0));  // Vector hacia arriba de la cámara

    fondo = Color_RR(51, 221, 221); // Fondo

    // LUCES

    // La suma de las intensidades de las luces no debería superar 1.0f
    Luz luz1(
        Vector(7, 4.5, 0),      // Posición de la luz
        Vector(0.7, 0.2, 0.2)); // Intensidad de la luz en RGB
    luces.push_back(luz1);
    Luz luz2(
        Vector(3, 4.5, 0),
        Vector(0.8, 0.8, 0.1));
    luces.push_back(luz2);

    // PAREDES

    ParedPtr paredFondo = std::make_shared<Pared_RR>(
        Vector(10, 0, 0),     // Centro de la pared
        Vector(-1, 0, 0),     // Normal
        10.0f,                // Ancho de la pared
        10.0f,                // Alto de la pared
        ColorRGB(255, 0, 0)); // Color
    objetos.push_back(paredFondo);

    ParedPtr paredIzquierda = std::make_shared<Pared_RR>(
        Vector(5, 0, -5),
        Vector(0, 0, 1),
        10.0f,
        10.0f,
        ColorRGB(0, 255, 0));
    objetos.push_back(paredIzquierda);

    ParedPtr paredDerecha = std::make_shared<Pared_RR>(
        Vector(5, 0, 5),
        Vector(0, 0, -1),
        10.0f,
        10.0f,
        ColorRGB(0, 0, 255));
    objetos.push_back(paredDerecha);

    ParedPtr paredSuperior = std::make_shared<Pared_RR>(
        Vector(5, 5, 0),
        Vector(0, -1, 0),
        10.0f,
        10.0f,
        ColorRGB(255, 255, 0));
    objetos.push_back(paredSuperior);

    ParedPtr paredInferior = std::make_shared<Pared_RR>(
        Vector(5, -5, 0),
        Vector(0, 1, 0),
        10.0f,
        10.0f,
        ColorRGB(255, 0, 255));
    objetos.push_back(paredInferior);

    // TEST Pared frontal, no se debería ver, porque la normal es hacia adentro del cubo
    // ParedPtr paredFrontal = std::make_shared<Pared_RR>(
    //     Vector(0, 0, 0),
    //     Vector(1, 0, 0),
    //     10.0f,
    //     10.0f,
    //     ColorRGB(0, 255, 255));
    // objetos.push_back(paredFrontal);

    // TEST Pared paralela a rayo de la cámara no se debería ver, porque la normal es perpendicular al rayo de la cámara
    // ParedPtr paredParalela = std::make_shared<Pared_RR>(
    //     Vector(5, 0, 0),
    //     Vector(0, 1, 0),
    //     10.0f,
    //     10.0f,
    //     ColorRGB(255, 255, 255));
    // objetos.push_back(paredParalela);

    // TEST Pared atrás de la cámara, no se debería ver
    // ParedPtr paredAtras = std::make_shared<Pared_RR>(
    //     Vector(-15, 0, 0),        // Centro de la pared
    //     Vector(-1, 0, 0),         // Normal
    //     10.0f,                    // Ancho de la pared
    //     10.0f,                    // Alto de la pared
    //     ColorRGB(128, 128, 128)); // Color gris
    // objetos.push_back(paredAtras);

    // MAS OBJETOS

    // Esfera_RR esfera = Esfera_RR(
    //     Vector(0, 0, 0),      // Centro de la esfera
    //     1.0f,                 // Radio de la esfera
    //     ColorRGB(255, 0, 0)); // Color rojo

    // objetos.push_back(&esfera);
}

ObjetoPtr Escena_RR::calcularInterseccionMasCercana(Rayo_RR rayo, Vector *punto, Vector *normal)
{
    ObjetoPtr objMasCercano = nullptr;
    float distanciaMinima = std::numeric_limits<float>::max();

    for (const auto &objeto : objetos)
    {
        if (objeto->calcularInterseccion(rayo, punto, normal))
        {
            float distancia = (rayo.getOrigen() - *punto).length();
            if (distancia < distanciaMinima)
            {
                distanciaMinima = distancia;
                objMasCercano = objeto;
            }
        }
    }

    return objMasCercano;
}

void Escena_RR::calcularColorIluminacion(ObjetoPtr objeto, Rayo_RR rayo, Vector punto, Vector normal, Color_RR &color)
{

    ColorRGB colorDifuso = ColorRGB(0, 0, 0);
    ColorRGB colorBaseDifuso = objeto->getColorReflexionDifusa();
    ColorRGB colorEspecular = ColorRGB(0, 0, 0);
    ColorRGB colorBaseEspecular = objeto->getColorReflexionEspecular();
    int brilloEspecular = objeto->getBrilloEspecular();
    for (const auto &luz : luces)
    {
        Vector direccionLuz = luz.getDirection(punto);
        Vector intensidad = luz.getIntensidad();
        float distanciaLuz = luz.getDistancia(punto);
        float factorAtenuacion = 1 / distanciaLuz * distanciaLuz;

        // Calcular iluminación difusa
        float dotProductDifuso = normal.dot(direccionLuz);
        if (dotProductDifuso > 0)
        {
            colorDifuso = colorDifuso + colorBaseDifuso * intensidad * dotProductDifuso * factorAtenuacion;
        }

        // Calcular iluminación especular
        Vector R = normal * 2.0f * (normal.dot(direccionLuz)) - direccionLuz;
        Vector V = rayo.getDireccion() * -1.0f;
        float dotProductEspecular = R.dot(V);
        if (dotProductEspecular > 0)
        {
            float intensidadEspecular = pow(dotProductEspecular, brilloEspecular);
            colorEspecular = colorEspecular + colorBaseEspecular * intensidad * intensidadEspecular * factorAtenuacion;
        }
    }
    color.setComponenteDifusa(colorDifuso, objeto->getCoeficienteReflexionDifusa());
    color.setComponenteEspecular(colorEspecular, objeto->getCoeficienteReflexionEspecular());
}

#endif // ESCENA_H