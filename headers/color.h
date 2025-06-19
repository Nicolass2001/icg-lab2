#include "color_rgb.h"
#include "globalVariables.h"

#ifndef COLOR_H
#define COLOR_H

class Color_RR
{
private:
    float coeficienteAmbiente = DEFAULT_AMBIENT_COEFFICIENT;
    ColorRGB colorAmbiente;
    float coeficienteReflexionDifusa = DEFAULT_DIFFUSE_REFLECTION_COEFFICIENT;
    ColorRGB colorReflexionDifusa;
    float coeficienteReflexionEspecular = DEFAULT_SPECULAR_REFLECTION_COEFFICIENT;
    ColorRGB colorReflexionEspecular;
    float coeficienteTransparencia = DEFAULT_TRANSPARENCY_COEFFICIENT;
    ColorRGB colorTransparencia;
    float coeficienteReflexion = DEFAULT_REFLECTION_COEFFICIENT;
    ColorRGB colorReflexion;

public:
    Color_RR();
    Color_RR(int r, int g, int b);
    Color_RR(const ColorRGB &other);
    friend std::ostream &operator<<(std::ostream &os, const Color_RR &color);
    RGBQUAD toRGBQUAD();
    ColorRGB getColorTotal() const;
    void setComponenteAmbiente(ColorRGB color, float coeficiente);
    void setComponenteDifusa(ColorRGB color, float coeficiente);
    void setComponenteEspecular(ColorRGB color, float coeficiente);
    void setComponenteTransparencia(ColorRGB color, float coeficiente);
    void setComponenteReflexion(ColorRGB color, float coeficiente);

    ColorRGB getComponenteReflexion() const;
    ColorRGB getComponenteTransparencia() const;

};

Color_RR::Color_RR()
{
    this->colorAmbiente = ColorRGB(0, 0, 0);
}

Color_RR::Color_RR(int r, int g, int b)
{
    this->colorAmbiente = ColorRGB(r, g, b);
}

Color_RR::Color_RR(const ColorRGB &other)
{
    this->colorAmbiente = other;
}

std::ostream &operator<<(std::ostream &os, const Color_RR &color)
{
    os << "Color_RR: " << color.getColorTotal();
    return os;
}

RGBQUAD Color_RR::toRGBQUAD()
{
    ColorRGB color = getColorTotal();
    RGBQUAD colorRGBQUAD;
    colorRGBQUAD.rgbRed = static_cast<BYTE>(color.getR());
    colorRGBQUAD.rgbGreen = static_cast<BYTE>(color.getG());
    colorRGBQUAD.rgbBlue = static_cast<BYTE>(color.getB());
    colorRGBQUAD.rgbReserved = 0; // Reservado, no se usa en este contexto
    return colorRGBQUAD;
}

ColorRGB Color_RR::getColorTotal() const
{
    return this->colorAmbiente * this->coeficienteAmbiente +
           this->colorReflexionDifusa * this->coeficienteReflexionDifusa +
           this->colorReflexionEspecular * this->coeficienteReflexionEspecular +
           this->colorReflexion * this->coeficienteReflexion +
           this->colorTransparencia * this->coeficienteTransparencia;
}

void Color_RR::setComponenteAmbiente(ColorRGB color, float coeficiente)
{
    this->colorAmbiente = color;
    this->coeficienteAmbiente = coeficiente;
}

void Color_RR::setComponenteDifusa(ColorRGB color, float coeficiente)
{
    this->colorReflexionDifusa = color;
    this->coeficienteReflexionDifusa = coeficiente;
}

void Color_RR::setComponenteEspecular(ColorRGB color, float coeficiente)
{
    this->colorReflexionEspecular = color;
    this->coeficienteReflexionEspecular = coeficiente;
}

void Color_RR::setComponenteTransparencia(ColorRGB color, float coeficiente)
{
    this->colorTransparencia = color;
    this->coeficienteTransparencia = coeficiente;
}

void Color_RR::setComponenteReflexion(ColorRGB color, float coeficiente)
{
    this->colorReflexion = color;
    this->coeficienteReflexion = coeficiente;
}

ColorRGB Color_RR::getComponenteReflexion() const {
     return colorReflexion * coeficienteReflexion; 
}

ColorRGB Color_RR::getComponenteTransparencia() const { 
    return colorTransparencia * coeficienteTransparencia; 
}


#endif // COLOR_H