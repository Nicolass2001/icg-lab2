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
    //Leer XML
    tinyxml2::XMLDocument datos;
    datos.LoadFile(pathConfig.c_str());

    if (datos.ErrorID() != 0) {
        std::cout << "XML - ERROR" << std::endl;
    }
    else {
        std::cout << "XML - EXITO" << std::endl;
    }

    tinyxml2::XMLElement* root = datos.FirstChildElement("config");

    tinyxml2::XMLElement* objetoActual = root->FirstChildElement("camara");

    camara = Camara_RR(
        Vector(std::stof(objetoActual->FirstChildElement("x")->GetText()),
            std::stof(objetoActual->FirstChildElement("y")->GetText()),
            std::stof(objetoActual->FirstChildElement("z")->GetText())), // Posición de la cámara
        Vector(std::stof(objetoActual->FirstChildElement("lookatx")->GetText()),
            std::stof(objetoActual->FirstChildElement("lookaty")->GetText()),
            std::stof(objetoActual->FirstChildElement("lookatz")->GetText())),  // Dirección hacia adelante de la cámara
        Vector(std::stof(objetoActual->FirstChildElement("upx")->GetText()),
            std::stof(objetoActual->FirstChildElement("upy")->GetText()),
            std::stof(objetoActual->FirstChildElement("upz")->GetText()))); // Vector hacia arriba de la cámara

    fondo = Color_RR(51, 221, 221); // Fondo

    // LUCES

    // La suma de las intensidades de las luces no debería superar 1.0f
    objetoActual = root->FirstChildElement("luces")->FirstChildElement("luz1");

    Luz luz1(
        // Posición de la luz
        Vector(std::stof(objetoActual->FirstChildElement("x")->GetText()),
            std::stof(objetoActual->FirstChildElement("y")->GetText()),
            std::stof(objetoActual->FirstChildElement("z")->GetText())),
        // Intensidad de la luz en RGB
        Vector(std::stof(objetoActual->FirstChildElement("r")->GetText()),
            std::stof(objetoActual->FirstChildElement("g")->GetText()),
            std::stof(objetoActual->FirstChildElement("b")->GetText())));
    luces.push_back(luz1);

    objetoActual = root->FirstChildElement("luces")->FirstChildElement("luz2");

    Luz luz2(
        Vector(std::stof(objetoActual->FirstChildElement("x")->GetText()),
            std::stof(objetoActual->FirstChildElement("y")->GetText()),
            std::stof(objetoActual->FirstChildElement("z")->GetText())),
        Vector(std::stof(objetoActual->FirstChildElement("r")->GetText()),
            std::stof(objetoActual->FirstChildElement("g")->GetText()),
            std::stof(objetoActual->FirstChildElement("b")->GetText())));
    luces.push_back(luz2);

    // PAREDES

    objetoActual = root->FirstChildElement("paredes")->FirstChildElement("paredFondo");

    ParedPtr paredFondo = std::make_shared<Pared_RR>(
        // Centro de la pared
        Vector(std::stof(objetoActual->FirstChildElement("centro")->FirstChildElement("x")->GetText()),
            std::stof(objetoActual->FirstChildElement("centro")->FirstChildElement("y")->GetText()),
            std::stof(objetoActual->FirstChildElement("centro")->FirstChildElement("z")->GetText())),
        // Normal
        Vector(std::stof(objetoActual->FirstChildElement("normal")->FirstChildElement("x")->GetText()),
            std::stof(objetoActual->FirstChildElement("normal")->FirstChildElement("y")->GetText()),
            std::stof(objetoActual->FirstChildElement("normal")->FirstChildElement("z")->GetText())),
        // Ancho de la pared
        std::stof(objetoActual->FirstChildElement("ancho")->GetText()),
        // Alto de la pared
        std::stof(objetoActual->FirstChildElement("alto")->GetText()),
        // Color
        ColorRGB(std::stoi(objetoActual->FirstChildElement("r")->GetText()),
            std::stoi(objetoActual->FirstChildElement("g")->GetText()),
            std::stoi(objetoActual->FirstChildElement("b")->GetText())));
    objetos.push_back(paredFondo);

    objetoActual = root->FirstChildElement("paredes")->FirstChildElement("paredIzquierda");

    ParedPtr paredIzquierda = std::make_shared<Pared_RR>(
        Vector(std::stof(objetoActual->FirstChildElement("centro")->FirstChildElement("x")->GetText()),
            std::stof(objetoActual->FirstChildElement("centro")->FirstChildElement("y")->GetText()),
            std::stof(objetoActual->FirstChildElement("centro")->FirstChildElement("z")->GetText())),
        Vector(std::stof(objetoActual->FirstChildElement("normal")->FirstChildElement("x")->GetText()),
            std::stof(objetoActual->FirstChildElement("normal")->FirstChildElement("y")->GetText()),
            std::stof(objetoActual->FirstChildElement("normal")->FirstChildElement("z")->GetText())),
        std::stof(objetoActual->FirstChildElement("ancho")->GetText()),
        std::stof(objetoActual->FirstChildElement("alto")->GetText()),
        ColorRGB(std::stoi(objetoActual->FirstChildElement("r")->GetText()),
            std::stoi(objetoActual->FirstChildElement("g")->GetText()),
            std::stoi(objetoActual->FirstChildElement("b")->GetText())));
    objetos.push_back(paredIzquierda);

    objetoActual = root->FirstChildElement("paredes")->FirstChildElement("paredDerecha");

    ParedPtr paredDerecha = std::make_shared<Pared_RR>(
        Vector(std::stof(objetoActual->FirstChildElement("centro")->FirstChildElement("x")->GetText()),
            std::stof(objetoActual->FirstChildElement("centro")->FirstChildElement("y")->GetText()),
            std::stof(objetoActual->FirstChildElement("centro")->FirstChildElement("z")->GetText())),
        Vector(std::stof(objetoActual->FirstChildElement("normal")->FirstChildElement("x")->GetText()),
            std::stof(objetoActual->FirstChildElement("normal")->FirstChildElement("y")->GetText()),
            std::stof(objetoActual->FirstChildElement("normal")->FirstChildElement("z")->GetText())),
        std::stof(objetoActual->FirstChildElement("ancho")->GetText()),
        std::stof(objetoActual->FirstChildElement("alto")->GetText()),
        ColorRGB(std::stoi(objetoActual->FirstChildElement("r")->GetText()),
            std::stoi(objetoActual->FirstChildElement("g")->GetText()),
            std::stoi(objetoActual->FirstChildElement("b")->GetText())));
    objetos.push_back(paredDerecha);

    objetoActual = root->FirstChildElement("paredes")->FirstChildElement("paredSuperior");

    ParedPtr paredSuperior = std::make_shared<Pared_RR>(
        Vector(std::stof(objetoActual->FirstChildElement("centro")->FirstChildElement("x")->GetText()),
            std::stof(objetoActual->FirstChildElement("centro")->FirstChildElement("y")->GetText()),
            std::stof(objetoActual->FirstChildElement("centro")->FirstChildElement("z")->GetText())),
        Vector(std::stof(objetoActual->FirstChildElement("normal")->FirstChildElement("x")->GetText()),
            std::stof(objetoActual->FirstChildElement("normal")->FirstChildElement("y")->GetText()),
            std::stof(objetoActual->FirstChildElement("normal")->FirstChildElement("z")->GetText())),
        std::stof(objetoActual->FirstChildElement("ancho")->GetText()),
        std::stof(objetoActual->FirstChildElement("alto")->GetText()),
        ColorRGB(std::stoi(objetoActual->FirstChildElement("r")->GetText()),
            std::stoi(objetoActual->FirstChildElement("g")->GetText()),
            std::stoi(objetoActual->FirstChildElement("b")->GetText())));
    objetos.push_back(paredSuperior);

    objetoActual = root->FirstChildElement("paredes")->FirstChildElement("paredInferior");

    ParedPtr paredInferior = std::make_shared<Pared_RR>(
        Vector(std::stof(objetoActual->FirstChildElement("centro")->FirstChildElement("x")->GetText()),
            std::stof(objetoActual->FirstChildElement("centro")->FirstChildElement("y")->GetText()),
            std::stof(objetoActual->FirstChildElement("centro")->FirstChildElement("z")->GetText())),
        Vector(std::stof(objetoActual->FirstChildElement("normal")->FirstChildElement("x")->GetText()),
            std::stof(objetoActual->FirstChildElement("normal")->FirstChildElement("y")->GetText()),
            std::stof(objetoActual->FirstChildElement("normal")->FirstChildElement("z")->GetText())),
        std::stof(objetoActual->FirstChildElement("ancho")->GetText()),
        std::stof(objetoActual->FirstChildElement("alto")->GetText()),
        ColorRGB(std::stoi(objetoActual->FirstChildElement("r")->GetText()),
            std::stoi(objetoActual->FirstChildElement("g")->GetText()),
            std::stoi(objetoActual->FirstChildElement("b")->GetText())));
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

    // ESFERAS

    objetoActual = root->FirstChildElement("esfera1");

    PropiedadesObjeto propEsfera(
        // Coeficiente de ambiente
        std::stof(objetoActual->FirstChildElement("ka")->FirstChildElement("valor")->GetText()),
        // Color ambiente
        ColorRGB(std::stoi(objetoActual->FirstChildElement("ka")->FirstChildElement("r")->GetText()),
            std::stoi(objetoActual->FirstChildElement("ka")->FirstChildElement("g")->GetText()),
            std::stoi(objetoActual->FirstChildElement("ka")->FirstChildElement("b")->GetText())),
        // Coeficiente de reflexión difusa
        std::stof(objetoActual->FirstChildElement("kd")->FirstChildElement("valor")->GetText()),
        // Color de reflexión difusa
        ColorRGB(std::stoi(objetoActual->FirstChildElement("kd")->FirstChildElement("r")->GetText()),
            std::stoi(objetoActual->FirstChildElement("kd")->FirstChildElement("g")->GetText()),
            std::stoi(objetoActual->FirstChildElement("kd")->FirstChildElement("b")->GetText())),
        // Coeficiente de reflexión especular
        std::stof(objetoActual->FirstChildElement("ks")->FirstChildElement("valor")->GetText()),
        // Brillo especular
        std::stoi(objetoActual->FirstChildElement("ks")->FirstChildElement("brillo")->GetText()),
        // Color de reflexión especular
        ColorRGB(std::stoi(objetoActual->FirstChildElement("ks")->FirstChildElement("r")->GetText()),
            std::stoi(objetoActual->FirstChildElement("ks")->FirstChildElement("g")->GetText()),
            std::stoi(objetoActual->FirstChildElement("ks")->FirstChildElement("b")->GetText())),
        // Coeficiente de transparencia
        Vector(std::stof(objetoActual->FirstChildElement("kt")->FirstChildElement("r")->GetText()),
            std::stof(objetoActual->FirstChildElement("kt")->FirstChildElement("g")->GetText()),
            std::stof(objetoActual->FirstChildElement("kt")->FirstChildElement("b")->GetText())),
        // Coeficiente de reflexion
        Vector(std::stof(objetoActual->FirstChildElement("kreflx")->FirstChildElement("r")->GetText()),
            std::stof(objetoActual->FirstChildElement("kreflx")->FirstChildElement("g")->GetText()),
            std::stof(objetoActual->FirstChildElement("kreflx")->FirstChildElement("b")->GetText())));

    Esfera_RR esfera = Esfera_RR(
        // Centro de la esfera
        Vector(std::stof(objetoActual->FirstChildElement("x")->GetText()),
            std::stof(objetoActual->FirstChildElement("y")->GetText()),
            std::stof(objetoActual->FirstChildElement("z")->GetText())),
        // Radio de la esfera
        std::stof(objetoActual->FirstChildElement("radio")->GetText()),
        // Propiedades de la esfera
        propEsfera);
    objetos.push_back(std::make_shared<Esfera_RR>(esfera));

    objetoActual = root->FirstChildElement("esfera2");

    PropiedadesObjeto propEsfera2(
        std::stof(objetoActual->FirstChildElement("ka")->FirstChildElement("valor")->GetText()),
        ColorRGB(std::stoi(objetoActual->FirstChildElement("ka")->FirstChildElement("r")->GetText()),
            std::stoi(objetoActual->FirstChildElement("ka")->FirstChildElement("g")->GetText()),
            std::stoi(objetoActual->FirstChildElement("ka")->FirstChildElement("b")->GetText())),
        std::stof(objetoActual->FirstChildElement("kd")->FirstChildElement("valor")->GetText()),
        ColorRGB(std::stoi(objetoActual->FirstChildElement("kd")->FirstChildElement("r")->GetText()),
            std::stoi(objetoActual->FirstChildElement("kd")->FirstChildElement("g")->GetText()),
            std::stoi(objetoActual->FirstChildElement("kd")->FirstChildElement("b")->GetText())),
        std::stof(objetoActual->FirstChildElement("ks")->FirstChildElement("valor")->GetText()),
        std::stoi(objetoActual->FirstChildElement("ks")->FirstChildElement("brillo")->GetText()),
        ColorRGB(std::stoi(objetoActual->FirstChildElement("ks")->FirstChildElement("r")->GetText()),
            std::stoi(objetoActual->FirstChildElement("ks")->FirstChildElement("g")->GetText()),
            std::stoi(objetoActual->FirstChildElement("ks")->FirstChildElement("b")->GetText())),
        Vector(std::stof(objetoActual->FirstChildElement("kt")->FirstChildElement("r")->GetText()),
            std::stof(objetoActual->FirstChildElement("kt")->FirstChildElement("g")->GetText()),
            std::stof(objetoActual->FirstChildElement("kt")->FirstChildElement("b")->GetText())),
        Vector(std::stof(objetoActual->FirstChildElement("kreflx")->FirstChildElement("r")->GetText()),
            std::stof(objetoActual->FirstChildElement("kreflx")->FirstChildElement("g")->GetText()),
            std::stof(objetoActual->FirstChildElement("kreflx")->FirstChildElement("b")->GetText())));

    Esfera_RR esfera2 = Esfera_RR(
        Vector(std::stof(objetoActual->FirstChildElement("x")->GetText()),
            std::stof(objetoActual->FirstChildElement("y")->GetText()),
            std::stof(objetoActual->FirstChildElement("z")->GetText())),
        std::stof(objetoActual->FirstChildElement("radio")->GetText()),
        propEsfera2);
    objetos.push_back(std::make_shared<Esfera_RR>(esfera2));

    // CILINDROS

    objetoActual = root->FirstChildElement("cilindro1");

    PropiedadesObjeto propCilindro(
        std::stof(objetoActual->FirstChildElement("ka")->FirstChildElement("valor")->GetText()),
        ColorRGB(std::stoi(objetoActual->FirstChildElement("ka")->FirstChildElement("r")->GetText()),
            std::stoi(objetoActual->FirstChildElement("ka")->FirstChildElement("g")->GetText()),
            std::stoi(objetoActual->FirstChildElement("ka")->FirstChildElement("b")->GetText())),
        std::stof(objetoActual->FirstChildElement("kd")->FirstChildElement("valor")->GetText()),
        ColorRGB(std::stoi(objetoActual->FirstChildElement("kd")->FirstChildElement("r")->GetText()),
            std::stoi(objetoActual->FirstChildElement("kd")->FirstChildElement("g")->GetText()),
            std::stoi(objetoActual->FirstChildElement("kd")->FirstChildElement("b")->GetText())),
        std::stof(objetoActual->FirstChildElement("ks")->FirstChildElement("valor")->GetText()),
        std::stoi(objetoActual->FirstChildElement("ks")->FirstChildElement("brillo")->GetText()),
        ColorRGB(std::stoi(objetoActual->FirstChildElement("ks")->FirstChildElement("r")->GetText()),
            std::stoi(objetoActual->FirstChildElement("ks")->FirstChildElement("g")->GetText()),
            std::stoi(objetoActual->FirstChildElement("ks")->FirstChildElement("b")->GetText())),
        Vector(std::stof(objetoActual->FirstChildElement("kt")->FirstChildElement("r")->GetText()),
            std::stof(objetoActual->FirstChildElement("kt")->FirstChildElement("g")->GetText()),
            std::stof(objetoActual->FirstChildElement("kt")->FirstChildElement("b")->GetText())),
        Vector(std::stof(objetoActual->FirstChildElement("kreflx")->FirstChildElement("r")->GetText()),
            std::stof(objetoActual->FirstChildElement("kreflx")->FirstChildElement("g")->GetText()),
            std::stof(objetoActual->FirstChildElement("kreflx")->FirstChildElement("b")->GetText())));

    Cilindro_RR cilindro = Cilindro_RR(
        // Base del cilindro
        Vector(std::stof(objetoActual->FirstChildElement("base")->FirstChildElement("x")->GetText()),
            std::stof(objetoActual->FirstChildElement("base")->FirstChildElement("y")->GetText()),
            std::stof(objetoActual->FirstChildElement("base")->FirstChildElement("z")->GetText())),
        // Dirección del cilindro (eje)
        Vector(std::stof(objetoActual->FirstChildElement("direccion")->FirstChildElement("x")->GetText()),
            std::stof(objetoActual->FirstChildElement("direccion")->FirstChildElement("y")->GetText()),
            std::stof(objetoActual->FirstChildElement("direccion")->FirstChildElement("z")->GetText())),
        // Radio del cilindro
        std::stof(objetoActual->FirstChildElement("radio")->GetText()),
        // Altura del cilindro
        std::stof(objetoActual->FirstChildElement("altura")->GetText()),
        // Propiedades del cilindro
        propCilindro);
    objetos.push_back(std::make_shared<Cilindro_RR>(cilindro));

    objetoActual = root->FirstChildElement("cilindro2");

    Cilindro_RR cilindro2 = Cilindro_RR(
        Vector(std::stof(objetoActual->FirstChildElement("base")->FirstChildElement("x")->GetText()),
            std::stof(objetoActual->FirstChildElement("base")->FirstChildElement("y")->GetText()),
            std::stof(objetoActual->FirstChildElement("base")->FirstChildElement("z")->GetText())),
        Vector(std::stof(objetoActual->FirstChildElement("direccion")->FirstChildElement("x")->GetText()),
            std::stof(objetoActual->FirstChildElement("direccion")->FirstChildElement("y")->GetText()),
            std::stof(objetoActual->FirstChildElement("direccion")->FirstChildElement("z")->GetText())),
        std::stof(objetoActual->FirstChildElement("radio")->GetText()),
        std::stof(objetoActual->FirstChildElement("altura")->GetText()),
        propCilindro);
    objetos.push_back(std::make_shared<Cilindro_RR>(cilindro2));
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
        Vector factorAtenuacionPorBloqueo = calcularLuzBloqueada(Rayo_RR(punto + direccionLuz * EPSILON, direccionLuz), luz.getPosicion());
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

#endif // ESCENA_H