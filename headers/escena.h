#include <vector>
#include "vector.h"
#include "color.h"
#include "pared.h"
#include "camara.h"
#include "esfera.h"
#include "cilindro.h"
#include "objeto.h"
#include "luz.h"
#include "tinyxml2.h"
#include <string>
#include "xmlHandler.h"
#include "malla.h"

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
    Escena_RR(std::string pathConfig);
    Color_RR getFondo();
    Camara_RR getCamara();
    ObjetoPtr calcularInterseccionMasCercana(Rayo_RR rayo, Vector *punto, Vector *normal);
    void calcularColorIluminacion(ObjetoPtr objeto, Rayo_RR rayo, Vector punto, Vector normal, Color_RR &color);
    Vector calcularLuzBloqueada(Rayo_RR rayo, Vector puntoLuz); // Método para calcular la luz bloqueada por otros objetos
    float indiceRefraccion(Vector punto);
};

Color_RR Escena_RR::getFondo()
{
    return fondo;
}

Camara_RR Escena_RR::getCamara()
{
    return camara;
}

Escena_RR::Escena_RR(std::string pathConfig)
{
    fondo = Color_RR(51, 221, 221); // Fondo

    // Leer XML
    XMLHandler xmlHandler(pathConfig);
    camara = xmlHandler.getCamera();
    luces = xmlHandler.getLuces();
    objetos = xmlHandler.getObjetos();


    float coeficienteAmbiente = 0.1;
    ColorRGB colorAmbiente(255,255,255);
    float coeficienteReflexionDifusa = 0.5;
    ColorRGB colorReflexionDifusa(255,255,255);
    float coeficienteReflexionEspecular = 1.0;
    int brilloEspecular = 5;
    ColorRGB colorReflexionEspecular(255,255,255);
    Vector coeficienteTransparencia(0,0,0);
    float indiceRefraccion = 0;
    Vector coeficienteReflexion(0,0,0);

    PropiedadesObjeto prop = PropiedadesObjeto(coeficienteAmbiente, colorAmbiente,
                             coeficienteReflexionDifusa, colorReflexionDifusa,
                             coeficienteReflexionEspecular, brilloEspecular, colorReflexionEspecular,
                             coeficienteTransparencia, indiceRefraccion, coeficienteReflexion);


    std::shared_ptr<Malla_RR> mallaPtr = std::make_shared<Malla_RR>(prop);
    objetos.push_back(mallaPtr);
}

ObjetoPtr Escena_RR::calcularInterseccionMasCercana(Rayo_RR rayo, Vector *punto, Vector *normal)
{
    ObjetoPtr objMasCercano = nullptr;
    float distanciaMinima = std::numeric_limits<float>::max();
    Vector puntoInterseccion, normalInterseccion;

    for (const auto &objeto : objetos)
    {
        if (objeto->calcularInterseccion(rayo, &puntoInterseccion, &normalInterseccion))
        {
            float distancia = (rayo.getOrigen() - puntoInterseccion).length();
            if (distancia < distanciaMinima)
            {
                distanciaMinima = distancia;
                objMasCercano = objeto;
                *punto = puntoInterseccion;
                *normal = normalInterseccion;
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

        // Calcular atenuación por bloqueo
        Vector factorAtenuacionPorBloqueo = calcularLuzBloqueada(Rayo_RR(punto + direccionLuz * EPSILON, direccionLuz, 0), luz.getPosicion());
        intensidad = intensidad * factorAtenuacionPorBloqueo;

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

Vector Escena_RR::calcularLuzBloqueada(Rayo_RR rayo, Vector puntoLuz)
{
    Vector origenRayo = rayo.getOrigen();
    Vector punto, normal;
    float distanciaLuz = (puntoLuz - origenRayo).length();
    Vector factorDeAtenuacion(1.0f, 1.0f, 1.0f); // Inicialmente la luz no está bloqueada

    for (const auto &objeto : objetos)
    {
        if (objeto->calcularInterseccion(rayo, &punto, &normal))
        {
            float distancia = (punto - origenRayo).length();
            if (distancia < distanciaLuz)
            {
                Vector coeficienteTransparencia = objeto->getCoeficienteTransparencia();
                if (coeficienteTransparencia.length() == 0.0f)
                    return Vector(0.0f, 0.0f, 0.0f); // La luz está bloqueada
                factorDeAtenuacion = factorDeAtenuacion * coeficienteTransparencia;
            }
        }
    }
    return factorDeAtenuacion;
}

float Escena_RR::indiceRefraccion(Vector punto)
{
    for (const auto &objeto : objetos)
    {
        if (objeto->estaDentro(punto))
        {
            return objeto->getIndiceRefraccion();
        }
    }
    return DEFAULT_REFRACTION_INDEX; // Si no hay intersección, se asume el índice de refracción del aire
}

#endif // ESCENA_H