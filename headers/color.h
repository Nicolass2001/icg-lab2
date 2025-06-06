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
    float coeficienteRefraccion = DEFAULT_REFRACTION_COEFFICIENT;
    ColorRGB colorRefraccion;

public:
    Color_RR();
    Color_RR(int r, int g, int b);
    Color_RR(const ColorRGB &other);
    friend std::ostream &operator<<(std::ostream &os, const Color_RR &color);
    RGBQUAD toRGBQUAD();
    ColorRGB getColorTotal() const;
    void setComponenteAmbiente(ColorRGB color, float coeficiente);
    void setComponenteDifusa(ColorRGB color, float coeficiente);
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
           this->colorReflexionDifusa * this->coeficienteReflexionDifusa;
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

#endif // COLOR_H