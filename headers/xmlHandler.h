#include <iostream>
#include <string>
#include "tinyxml2.h"
#include "camara.h"
#include "luz.h"
#include "objeto.h"
#include "pared.h"
#include "esfera.h"
#include "cilindro.h"

#ifndef XML_HANDLER_H
#define XML_HANDLER_H

class XMLHandler
{
private:
    tinyxml2::XMLDocument datos;
    tinyxml2::XMLElement *root;

public:
    XMLHandler(std::string pathConfig);
    Camara_RR getCamera();
    std::vector<Luz> getLuces();
    std::vector<ObjetoPtr> getObjetos();

private:
    PropiedadesObjeto getPropiedadesObjeto(tinyxml2::XMLElement *propElement);
    ObjetoPtr getPared(tinyxml2::XMLElement *objetoElement);
    ObjetoPtr getEsfera(tinyxml2::XMLElement *objetoElement);
    ObjetoPtr getCilindro(tinyxml2::XMLElement *objetoElement);
};

XMLHandler::XMLHandler(std::string pathConfig)
{
    datos.LoadFile(pathConfig.c_str());

    if (datos.ErrorID() != 0)
    {
        std::cout << "XML - ERROR" << std::endl;
    }
    else
    {
        std::cout << "XML - EXITO" << std::endl;
    }

    root = datos.FirstChildElement("config");
}

Camara_RR XMLHandler::getCamera()
{

    tinyxml2::XMLElement *camara = root->FirstChildElement("camara");

    Vector position(std::stof(camara->FirstChildElement("x")->GetText()),
                    std::stof(camara->FirstChildElement("y")->GetText()),
                    std::stof(camara->FirstChildElement("z")->GetText())); // Posición de la cámara

    Vector lookAt(std::stof(camara->FirstChildElement("lookatx")->GetText()),
                  std::stof(camara->FirstChildElement("lookaty")->GetText()),
                  std::stof(camara->FirstChildElement("lookatz")->GetText())); // Dirección hacia adelante de la cámara

    Vector up(std::stof(camara->FirstChildElement("upx")->GetText()),
              std::stof(camara->FirstChildElement("upy")->GetText()),
              std::stof(camara->FirstChildElement("upz")->GetText())); // Vector hacia arriba de la cámara

    return Camara_RR(position, lookAt, up);
}

std::vector<Luz> XMLHandler::getLuces()
{
    std::vector<Luz> luces;
    tinyxml2::XMLElement *lucesElement = root->FirstChildElement("luces");

    for (tinyxml2::XMLElement *luzElement = lucesElement->FirstChildElement(); luzElement; luzElement = luzElement->NextSiblingElement())
    {
        Luz luz(
            Vector(std::stof(luzElement->FirstChildElement("x")->GetText()),
                   std::stof(luzElement->FirstChildElement("y")->GetText()),
                   std::stof(luzElement->FirstChildElement("z")->GetText())), // Posición de la luz
            Vector(std::stof(luzElement->FirstChildElement("r")->GetText()),
                   std::stof(luzElement->FirstChildElement("g")->GetText()),
                   std::stof(luzElement->FirstChildElement("b")->GetText()))); // Intensidad de la luz en RGB

        luces.push_back(luz);
    }

    return luces;
}

std::vector<ObjetoPtr> XMLHandler::getObjetos()
{
    std::vector<ObjetoPtr> objetos;
    tinyxml2::XMLElement *objetosElement = root->FirstChildElement("objetos");

    for (tinyxml2::XMLElement *objetoElement = objetosElement->FirstChildElement(); objetoElement; objetoElement = objetoElement->NextSiblingElement())
    {
        // obtener el tipo de elemento
        std::string tipo = objetoElement->Name();
        if (tipo == "pared")
        {
            objetos.push_back(getPared(objetoElement));
            continue;
        }
        if (tipo == "esfera")
        {
            objetos.push_back(getEsfera(objetoElement));
            continue;
        }
        if (tipo == "cilindro")
        {
            objetos.push_back(getCilindro(objetoElement));
            continue;
        }
    }

    return objetos;
}

PropiedadesObjeto XMLHandler::getPropiedadesObjeto(tinyxml2::XMLElement *propElement)
{
    float coeficienteAmbiente = std::stof(propElement->FirstChildElement("ka")->FirstChildElement("valor")->GetText());
    ColorRGB colorAmbiente(
        std::stoi(propElement->FirstChildElement("ka")->FirstChildElement("r")->GetText()),
        std::stoi(propElement->FirstChildElement("ka")->FirstChildElement("g")->GetText()),
        std::stoi(propElement->FirstChildElement("ka")->FirstChildElement("b")->GetText()));

    float coeficienteReflexionDifusa = std::stof(propElement->FirstChildElement("kd")->FirstChildElement("valor")->GetText());
    ColorRGB colorReflexionDifusa(
        std::stoi(propElement->FirstChildElement("kd")->FirstChildElement("r")->GetText()),
        std::stoi(propElement->FirstChildElement("kd")->FirstChildElement("g")->GetText()),
        std::stoi(propElement->FirstChildElement("kd")->FirstChildElement("b")->GetText()));

    float coeficienteReflexionEspecular = std::stof(propElement->FirstChildElement("ks")->FirstChildElement("valor")->GetText());
    int brilloEspecular = std::stoi(propElement->FirstChildElement("ks")->FirstChildElement("brillo")->GetText());
    ColorRGB colorReflexionEspecular(
        std::stoi(propElement->FirstChildElement("ks")->FirstChildElement("r")->GetText()),
        std::stoi(propElement->FirstChildElement("ks")->FirstChildElement("g")->GetText()),
        std::stoi(propElement->FirstChildElement("ks")->FirstChildElement("b")->GetText()));

    float coeficienteTransparencia = std::stof(propElement->FirstChildElement("kt")->GetText());

    float indiceRefraccion = std::stof(propElement->FirstChildElement("indiceRefraccion")->GetText());
    float coeficienteReflexion = std::stof(propElement->FirstChildElement("kreflexion")->GetText());

    return PropiedadesObjeto(coeficienteAmbiente, colorAmbiente,
                             coeficienteReflexionDifusa, colorReflexionDifusa,
                             coeficienteReflexionEspecular, brilloEspecular, colorReflexionEspecular,
                             coeficienteTransparencia, indiceRefraccion, coeficienteReflexion);
}

ObjetoPtr XMLHandler::getPared(tinyxml2::XMLElement *objetoElement)
{
    Vector centro(
        std::stof(objetoElement->FirstChildElement("centro")->FirstChildElement("x")->GetText()),
        std::stof(objetoElement->FirstChildElement("centro")->FirstChildElement("y")->GetText()),
        std::stof(objetoElement->FirstChildElement("centro")->FirstChildElement("z")->GetText()));

    Vector normal(
        std::stof(objetoElement->FirstChildElement("normal")->FirstChildElement("x")->GetText()),
        std::stof(objetoElement->FirstChildElement("normal")->FirstChildElement("y")->GetText()),
        std::stof(objetoElement->FirstChildElement("normal")->FirstChildElement("z")->GetText()));

    float ancho = std::stof(objetoElement->FirstChildElement("ancho")->GetText());
    float alto = std::stof(objetoElement->FirstChildElement("alto")->GetText());

    if (objetoElement->FirstChildElement("propObjeto") == nullptr)
    {
        ColorRGB color(
            std::stoi(objetoElement->FirstChildElement("r")->GetText()),
            std::stoi(objetoElement->FirstChildElement("g")->GetText()),
            std::stoi(objetoElement->FirstChildElement("b")->GetText()));

        return std::make_shared<Pared_RR>(centro, normal, ancho, alto, color);
    }

    PropiedadesObjeto prop = getPropiedadesObjeto(objetoElement->FirstChildElement("propObjeto"));

    return std::make_shared<Pared_RR>(centro, normal, ancho, alto, prop);
}

ObjetoPtr XMLHandler::getEsfera(tinyxml2::XMLElement *objetoElement)
{
    Vector centro(
        std::stof(objetoElement->FirstChildElement("centro")->FirstChildElement("x")->GetText()),
        std::stof(objetoElement->FirstChildElement("centro")->FirstChildElement("y")->GetText()),
        std::stof(objetoElement->FirstChildElement("centro")->FirstChildElement("z")->GetText()));

    float radio = std::stof(objetoElement->FirstChildElement("radio")->GetText());

    if (objetoElement->FirstChildElement("propObjeto") == nullptr)
    {
        ColorRGB color(
            std::stoi(objetoElement->FirstChildElement("r")->GetText()),
            std::stoi(objetoElement->FirstChildElement("g")->GetText()),
            std::stoi(objetoElement->FirstChildElement("b")->GetText()));

        return std::make_shared<Esfera_RR>(centro, radio, color);
    }

    PropiedadesObjeto prop = getPropiedadesObjeto(objetoElement->FirstChildElement("propObjeto"));

    return std::make_shared<Esfera_RR>(centro, radio, prop);
}

ObjetoPtr XMLHandler::getCilindro(tinyxml2::XMLElement *objetoElement)
{
    Vector centro(
        std::stof(objetoElement->FirstChildElement("base")->FirstChildElement("x")->GetText()),
        std::stof(objetoElement->FirstChildElement("base")->FirstChildElement("y")->GetText()),
        std::stof(objetoElement->FirstChildElement("base")->FirstChildElement("z")->GetText()));

    Vector direccion(
        std::stof(objetoElement->FirstChildElement("direccion")->FirstChildElement("x")->GetText()),
        std::stof(objetoElement->FirstChildElement("direccion")->FirstChildElement("y")->GetText()),
        std::stof(objetoElement->FirstChildElement("direccion")->FirstChildElement("z")->GetText()));

    float radio = std::stof(objetoElement->FirstChildElement("radio")->GetText());
    float altura = std::stof(objetoElement->FirstChildElement("altura")->GetText());

    if (objetoElement->FirstChildElement("propObjeto") == nullptr)
    {
        ColorRGB color(
            std::stoi(objetoElement->FirstChildElement("r")->GetText()),
            std::stoi(objetoElement->FirstChildElement("g")->GetText()),
            std::stoi(objetoElement->FirstChildElement("b")->GetText()));

        return std::make_shared<Cilindro_RR>(centro, direccion, radio, altura, color);
    }

    PropiedadesObjeto prop = getPropiedadesObjeto(objetoElement->FirstChildElement("propObjeto"));

    return std::make_shared<Cilindro_RR>(centro, direccion, radio, altura, prop);
}

#endif // XML_HANDLER_H