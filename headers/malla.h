#include <vector>
#include "objeto.h"
#include "vector.h"

#ifndef MALLA_H
#define MALLA_H

struct Triangle {
    int i0, i1, i2;
};

class Malla_RR : public Objeto_RR {
private:
    std::vector<Vector> vertices; // Represento los vertices con un vector
    std::vector<Triangle> triangulos; 

    Vector centro;
    float radio;

public:
    Malla_RR();
    Malla_RR(PropiedadesObjeto prop);
    bool calcularInterseccion(Rayo_RR rayo, Vector *puntoInterseccion, Vector *normal) override;

    bool estaDentro(Vector punto);

    bool punto_en_triangulo(const Vector& P, const Triangle& tri, const Vector& normal);

};

Malla_RR::Malla_RR() {}

// Para inicializar el rectangulo voy a construir las 6 caras una a la vez,
// cada cara tiene dos triangulos

// Representación con apuntadores a una lista de vértices.
// Voy a tener los vertices que definen la figura, con una lista de triangulos donde
// cada elemento es de la forma {0, 1, 2}, lo cual dice que esa figura se define por esos vertices

Malla_RR::Malla_RR(PropiedadesObjeto prop) : Objeto_RR(prop) {
    // Por como lo tenemos hecho interpretamos el eje x y z intercambiados, entonces 
    // anoto lo que viene a ser cada vertice en esta realidad que usamos para evitar confusiones

    Vector v0 = Vector(3.5, -5.0, -4.0); // adelante - abajo - izquierda
    Vector v1 = Vector(6.5, -5.0, -4.0); // atras - abajo - izquierda
    Vector v2 = Vector(6.5, -3.5, -4.0); // atras - arriba - izquierda
    Vector v3 = Vector(3.5, -3.5, -4.0); // adelante - arriba - izquierda
    Vector v4 = Vector(3.5, -5.0,  2.0); // adelante - abajo - derecha
    Vector v5 = Vector(6.5, -5.0,  2.0); // atras - abajo - derecha
    Vector v6 = Vector(6.5, -3.5,  2.0); // atras - arriba - derecha
    Vector v7 = Vector(3.5, -3.5,  2.0); // adelante - arriba - derecha
    this->vertices = {v0, v1, v2, v3, v4, v5, v6, v7};

    //"El polígono se define con una lista de índices a la lista de vértices"
    // A la lista de triangulos, cada uno se define por sus vertices

    // CARA FRONTAL
    triangulos.push_back({0, 1, 2}); // triangulo 0 definido por los vertices 0, 1 y 2
    triangulos.push_back({0, 2, 3}); 

    // CARA TRASERA
    triangulos.push_back({4, 5, 6});
    triangulos.push_back({4, 6, 7});    

    // CARA INFERIOR
    triangulos.push_back({0, 5, 1});
    triangulos.push_back({0, 4, 5}); 

    // CARA SUPERIOR
    triangulos.push_back({3, 6, 2});
    triangulos.push_back({3, 7, 6});

    // CARA IZQUIERDA
    triangulos.push_back({0, 7, 3});
    triangulos.push_back({0, 4, 7});

    // CARA DERECHA
    triangulos.push_back({1, 5, 6});
    triangulos.push_back({1, 6, 2});
}  



bool Malla_RR::calcularInterseccion(Rayo_RR rayo, Vector *puntoInterseccionRet, Vector *normalRet) {
    float t_min = std::numeric_limits<float>::max();
    bool hayInterseccion = false;

    for (const Triangle& tri : triangulos) {
        float t_aux = 0;
        Vector v0 = vertices[tri.i0]; 
        Vector v1 = vertices[tri.i1];
        Vector v2 = vertices[tri.i2];

        Vector normal = (v1 - v0).cross(v2 - v0).normalize();

        // normal • (P - v0) = 0
        // punto de interseccion P = O + t*D
        // Con P = O + tD (rayo: origen O, dirección D)
        // t = normal • (v0 - O) / normal • D

        float denominador = normal.dot(rayo.getDireccion());
        if (fabs(denominador) < 1e-6f) continue;

        Vector expr1 = v0 - rayo.getOrigen();
        float nominador = normal.dot(expr1);

        t_aux =  nominador / denominador;
        if (t_aux < 0 || t_aux > t_min) {
            continue;
        }

        // Calculo punto de interseccion P = O + t*D
        Vector puntoInterseccion = rayo.getOrigen() + rayo.getDireccion() * t_aux;


        if (punto_en_triangulo(puntoInterseccion, tri, normal)) {
            t_min = t_aux;
            *puntoInterseccionRet = puntoInterseccion;
            *normalRet = normal;
            hayInterseccion = true;
        }
        
    }

    return hayInterseccion;
}

bool Malla_RR::punto_en_triangulo(const Vector& P, const Triangle& triangulo, const Vector& normal) {
    int indice_A = triangulo.i0; // indice al vertice A del triang
    int indice_B = triangulo.i1;
    int indice_C = triangulo.i2;

    Vector A = vertices[indice_A];
    Vector B = vertices[indice_B];
    Vector C = vertices[indice_C];

    Vector edge0 = B - A;
    Vector vp0 = P - A;
    if (normal.dot(edge0.cross(vp0)) < 0) return false;

    Vector edge1 = C - B;
    Vector vp1 = P - B;
    if (normal.dot(edge1.cross(vp1)) < 0) return false;

    Vector edge2 = A - C;
    Vector vp2 = P - C;
    if (normal.dot(edge2.cross(vp2)) < 0) return false;

    return true;
}


bool Malla_RR::estaDentro(Vector punto)
{
    // Verifica si el punto está dentro de la esfera
    return (punto - centro).length() < radio;
}

#endif // MALLA_H