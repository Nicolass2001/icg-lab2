#ifndef COLOR_RGB_H
#define COLOR_RGB_H

class ColorRGB
{
private:
    int r, g, b;

public:
    ColorRGB();
    ColorRGB(int r, int g, int b);
    int getR() const;
    int getG() const;
    int getB() const;
};

ColorRGB::ColorRGB()
{
    this->r = 0;
    this->g = 0;
    this->b = 0;
}

ColorRGB::ColorRGB(int r, int g, int b)
{
    this->r = r;
    this->g = g;
    this->b = b;
}

int ColorRGB::getR() const { return r; }

int ColorRGB::getG() const { return g; }

int ColorRGB::getB() const { return b; }

#endif // COLOR_RGB_H