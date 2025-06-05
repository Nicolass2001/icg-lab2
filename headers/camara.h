#include "globalVariables.h"
#include "vector.h"

#ifndef CAMARA_H
#define CAMARA_H

class Camara_RR
{
private:
    Vector position; // Posición de la cámara
    Vector forward;  // Vector hacia adelante de la cámara
    Vector right;    // Vector hacia la derecha de la cámara
    Vector up;       // Vector hacia arriba de la cámara
    float distancia; // Distancia de la cámara al plano de proyección

    float planeHeight; // Altura del plano de proyección
    float planeWidth;  // Ancho del plano de proyección

    float pixelWidth;  // Ancho de cada pixel en el plano de proyección
    float pixelHeight; // Alto de cada pixel en el plano de proyección

    Vector planeCenter; // Centro del plano de proyección

public:
    Camara_RR();
    Camara_RR(Vector pos, Vector dir, Vector upVec);
    Rayo_RR generarRayo(int i, int j);
    friend std::ostream &operator<<(std::ostream &os, const Camara_RR &camara);
};

Camara_RR::Camara_RR() {}

Camara_RR::Camara_RR(Vector pos, Vector dir, Vector upVec)
{
    // Normalizar los vectores
    position = pos;
    forward = dir.normalize();
    right = forward.cross(upVec).normalize();
    up = right.cross(forward).normalize() * -1.0f;
    distancia = 1.0f; // Por ejemplo, puede ser parametrizable

    // Tamaño de cada pixel en unidades del espacio
    float aspectRatio = static_cast<float>(IMAGE_WIDTH) / IMAGE_HEIGHT;
    planeHeight = 2.0f; // Por ejemplo, puede ser parametrizable
    planeWidth = planeHeight * aspectRatio;

    pixelWidth = planeWidth / IMAGE_WIDTH;
    pixelHeight = planeHeight / IMAGE_HEIGHT;

    // Calcular el centro del plano de proyección
    planeCenter = position + forward * distancia;
}

Rayo_RR Camara_RR::generarRayo(int i, int j)
{
    // Coordenadas en el plano de proyección
    float u = (i + 0.5f) * pixelWidth - planeWidth / 2.0f;
    float v = (j + 0.5f) * pixelHeight - planeHeight / 2.0f;

    // Calcular la posición del pixel en el plano de proyección
    Vector pixelPosition = planeCenter + right * u - up * v;

    // Calcular el vector que apunta hacia el pixel
    Vector direccionRayo = (pixelPosition - position).normalize();
    // Crear y retornar el rayo
    return Rayo_RR(position, direccionRayo);
}

std::ostream &operator<<(std::ostream &os, const Camara_RR &camara)
{
    os << "Camara: Position(" << camara.position.x() << ", " << camara.position.y() << ", " << camara.position.z()
       << "), Forward(" << camara.forward.x() << ", " << camara.forward.y() << ", " << camara.forward.z()
       << "), Right(" << camara.right.x() << ", " << camara.right.y() << ", " << camara.right.z()
       << "), Up(" << camara.up.x() << ", " << camara.up.y() << ", " << camara.up.z() << ")";
    return os;
}

#endif // CAMARA_H