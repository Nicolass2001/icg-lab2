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
    Pared_RR(Vector centro, Vector normal, float ancho, float alto, PropiedadesObjeto prop);
    Pared_RR(Vector centro, Vector normal, Vector up, float ancho, float alto, PropiedadesObjeto prop);
    bool calcularInterseccion(Rayo_RR rayo, Vector *puntoInterseccion, Vector *normal) override;
    bool estaDentro(Vector punto) override;
    bool punto_en_pared(Vector punto);
    void setColoresTextura(Vector punto, datosTextura textura) override;
};

using ParedPtr = std::shared_ptr<Pared_RR>;

Pared_RR::Pared_RR() {}

Pared_RR::Pared_RR(Vector centro, Vector normal, float ancho, float alto, PropiedadesObjeto prop)
    : Objeto_RR(prop)
{
    this->centro = centro;
    this->normal = normal.normalize();

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

Pared_RR::Pared_RR(Vector centro, Vector normal, Vector up, float ancho, float alto, PropiedadesObjeto prop)
    : Objeto_RR(prop)
{
    this->centro = centro;
    this->normal = normal;

    // Ejes locales al plano de la pared
    Vector ejeX = normal.cross(up).normalize(); // Se asume que 'up' es un vector no paralelo a 'normal'
    Vector ejeY = normal.cross(ejeX).normalize();

    this->anchoVec = ejeX * ancho; // Ancho de la pared
    this->altoVec = ejeY * alto;   // Alto de la pared
}

bool Pared_RR::calcularInterseccion(Rayo_RR rayo, Vector *puntoInterseccionRet, Vector *normalRet)
{

    // La escuacion para interseccion entre rayo y plano es asi:
    // t = - (O-Po).N / D.N
    // Rayo tiene origen O y direccion D
    // Plano definido por un punto (origen) y una normal (estos son los atributos de la clase)


    float denominador = normal.dot(rayo.getDireccion());

    // Verifica si el rayo es paralelo al plano
    if (glm::abs(denominador) < 1e-6f)
        return false;

    // Verifica si la normal y el rayo apuntan hacia el mismo lado (se ve desde atras)
    if (denominador > 0)
        return false;

    float nominador = normal.dot(centro - rayo.getOrigen());

    // Calculo t (distancia a la interseccion desde el origen del rayo)
    float t =  nominador / denominador;

    // La intersecc esta atras del origen del rayo
    if (t < 0) {
        return false;
    }

    // Calculo punto de interseccion P = O + t*D
    Vector puntoInterseccion = rayo.getOrigen() + rayo.getDireccion() * t;

    if (!punto_en_pared(puntoInterseccion)) return false;


    *puntoInterseccionRet = Vector(puntoInterseccion);
    *normalRet = Vector(normal);
    return true;
    
}

// Calculo vector desde centro del rectangulo hasta P, proyecto sobre los
// lados del rectangulo, y veo si esta dentro
bool Pared_RR::punto_en_pared(Vector punto){
    Vector relativePoint = punto - centro;

    float u = relativePoint.dot(anchoVec.normalize()); // Coordenada en la direcc del ancho
    float v = relativePoint.dot(altoVec.normalize());  // Coordenada en la direcc del largo

    float width = anchoVec.length();
    float height = altoVec.length();

    if ((u >= -width / 2 && u <= width / 2) && (v >= -height / 2 && v <= height / 2)) {
        return true;
    } else {
        return false;
    }
}

bool Pared_RR::estaDentro(Vector punto)
{
    return false; // Las paredes no tienen un interior definido, por lo que siempre devuelven false
}

void Pared_RR::setColoresTextura(Vector punto, datosTextura textura) {

    float puntoX, puntoY, puntoZ;

    float paredWidth = anchoVec.length();
    float paredHeight = altoVec.length();

    float imageWidth = textura.width;
    float imageHeight = textura.height;

    RGBQUAD color;

    // Calculo paredes izquierda y derecha
    if (this->getTipoObjeto() == 4 || this->getTipoObjeto() == 2) {
        puntoY = punto.y() + (paredHeight / 2.0f);
        puntoX = (punto.x() * (-1.0f)) + paredWidth;

        puntoX = std::trunc((puntoX / paredWidth) * imageWidth);
        puntoY = std::trunc((puntoY / paredHeight) * imageHeight);

        if (puntoX < 0.0f) {
            puntoX = 0.0f;
        }
        if (puntoY < 0.0f) {
            puntoY = 0.0f;
        }

        FreeImage_GetPixelColor(textura.datos, (unsigned int)puntoX, (unsigned int)puntoY, &color);
    }
    // Calculo paredes superior e inferior
    else if (this->getTipoObjeto() == 5 || this->getTipoObjeto() == 3) {
        puntoZ = punto.z() + 4.0f;
        puntoX = punto.x();

        puntoZ = std::trunc((puntoZ / paredWidth) * imageWidth);
        puntoX = std::trunc((puntoX / paredHeight) * imageHeight);

        if (puntoX < 0.0f) {
            puntoX = 0.0f;
        }
        if (puntoZ < 0.0f) {
            puntoZ = 0.0f;
        }

        FreeImage_GetPixelColor(textura.datos, (unsigned int)puntoZ, (unsigned int)puntoX, &color);
    }
    // Calculo pared fondo
    else {
        puntoZ = punto.z() + 4.0f;
        puntoY = punto.y() + 5.0f;

        puntoZ = std::trunc((puntoZ / paredWidth) * imageWidth);
        puntoY = std::trunc((puntoY / paredHeight) * imageHeight);

        if (puntoY < 0.0f) {
            puntoY = 0.0f;
        }
        if (puntoZ < 0.0f) {
            puntoZ = 0.0f;
        }

        FreeImage_GetPixelColor(textura.datos, (unsigned int)puntoZ, (unsigned int)puntoY, &color);
    }

    int r = color.rgbRed;
    int g = color.rgbGreen;
    int b = color.rgbBlue;

    this->prop.colorAmbiente = { r, g, b };
    this->prop.colorReflexionDifusa = { r, g, b };
    this->prop.colorReflexionEspecular = { r, g, b };

}

#endif // PARED_H