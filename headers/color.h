#include "color_rgb.h"

#ifndef COLOR_H
#define COLOR_H

class Color_RR
{
private:
    int r, g, b;

public:
    Color_RR();
    Color_RR(int r, int g, int b);
    Color_RR(const ColorRGB &other);
    RGBQUAD toRGBQUAD() const
    {
        RGBQUAD color;
        color.rgbRed = static_cast<BYTE>(r);
        color.rgbGreen = static_cast<BYTE>(g);
        color.rgbBlue = static_cast<BYTE>(b);
        color.rgbReserved = 0; // Reservado, no se usa en este contexto
        return color;
    }
};

Color_RR::Color_RR()
{
    this->r = 0;
    this->g = 0;
    this->b = 0;
}

Color_RR::Color_RR(int r, int g, int b)
{
    this->r = r;
    this->g = g;
    this->b = b;
}

Color_RR::Color_RR(const ColorRGB &other)
{
    this->r = other.getR();
    this->g = other.getG();
    this->b = other.getB();
}

#endif // COLOR_H