#include <vector>
#include "header.h"

// Clase representa un color
class Color_RR {
private:
    int r, g, b;
public:
    Color_RR(int r, int g, int b);
};

// Clase representa un rayo poner la logica de rayo aqui dentro
class Rayo_RR {

};

// Clase representa una normal
// No se si es muy necesario pero por las dudas
class Normal_RR {

};

// Clase que representa la escena completa
class Escena_RR {
private:
    std::vector<Objeto_RR*> objetos;
    Color_RR fondo;
public:
    Objeto_RR* calcularInterseccionMasCercana(Rayo_RR rayo);
    Color_RR getFondo();

};

class Objeto_RR {
public:
    virtual Normal_RR calcularNormal(Rayo_RR rayo) = 0;
};

class Pared_RR : public Objeto_RR {

};

Escena_RR escena;

Color_RR sombra_RR(Objeto_RR* objeto, Rayo_RR rayo, Normal_RR normal, int profundidad) {

}

Color_RR traza_RR(Rayo_RR rayo, int profundidad) {
    Objeto_RR* objMasCercano = escena.calcularInterseccionMasCercana(rayo);
    if (objMasCercano == nullptr) {
        return escena.getFondo();
    }

    Normal_RR normal = objMasCercano->calcularNormal(rayo);
    return sombra_RR(objMasCercano, rayo, normal, profundidad);
}

int main() {
    copiarImagen();

    Color_RR traza_RR();

    return 0;
}
