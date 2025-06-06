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
    friend std::ostream &operator<<(std::ostream &os, const ColorRGB &color);
    ColorRGB operator*(float factor) const;
    ColorRGB operator+(const ColorRGB &other) const;
    ColorRGB operator/(float factor) const;
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

std::ostream &operator<<(std::ostream &os, const ColorRGB &color)
{
    os << "ColorRGB(r: " << color.r << ", g: " << color.g << ", b: " << color.b << ")";
    return os;
}

ColorRGB ColorRGB::operator*(float factor) const
{
    return ColorRGB(static_cast<int>(r * factor), static_cast<int>(g * factor), static_cast<int>(b * factor));
}

ColorRGB ColorRGB::operator+(const ColorRGB &other) const
{
    return ColorRGB(r + other.r, g + other.g, b + other.b);
}

ColorRGB ColorRGB::operator/(float factor) const
{
    return ColorRGB(static_cast<int>(r / factor), static_cast<int>(g / factor), static_cast<int>(b / factor));
}

#endif // COLOR_RGB_H