#include <glm/glm.hpp>

#ifndef VECTOR_H
#define VECTOR_H

class Vector
{
private:
public:
    glm::vec3 components;
    Vector();
    Vector(float x, float y, float z);
    Vector(const glm::vec3 &vec);
    Vector(const Vector &other);
    float x() const;
    float y() const;
    float z() const;
    bool parallel(const Vector &other) const;
    float length() const;
    Vector normalize();
    Vector operator+(const Vector &other);
    Vector operator-(const Vector &other);
    Vector operator*(float scalar);
    Vector operator/(float scalar);
    Vector operator*(const Vector &other);
    Vector cross(const Vector &other);
    float dot(const Vector &other);
    friend std::ostream &operator<<(std::ostream &os, const Vector &vec);
};

Vector::Vector()
{
    components = glm::vec3(0.0f, 0.0f, 0.0f);
}

Vector::Vector(float x, float y, float z)
{
    components = glm::vec3(x, y, z);
}

Vector::Vector(const glm::vec3 &vec)
{
    components = vec;
}

Vector::Vector(const Vector &other)
{
    components = other.components;
}

float Vector::x() const
{
    return components.x;
}

float Vector::y() const
{
    return components.y;
}

float Vector::z() const
{
    return components.z;
}

bool Vector::parallel(const Vector &other) const
{
    float crossProductLength = glm::length(glm::cross(components, other.components));
    return crossProductLength < 1e-6f;
}

float Vector::length() const
{
    return glm::length(components);
}

Vector Vector::normalize()
{
    return Vector(glm::normalize(components));
}

Vector Vector::operator+(const Vector &other)
{
    return Vector(components + other.components);
}

Vector Vector::operator-(const Vector &other)
{
    return Vector(components - other.components);
}

Vector Vector::operator*(float scalar)
{
    return Vector(components * scalar);
}

Vector Vector::operator/(float scalar)
{
    return Vector(components / scalar);
}

Vector Vector::operator*(const Vector &other)
{
    return Vector(components * other.components);
}

Vector Vector::cross(const Vector &other)
{
    return Vector(glm::cross(components, other.components));
}

float Vector::dot(const Vector &other)
{
    return glm::dot(components, other.components);
}

std::ostream &operator<<(std::ostream &os, const Vector &vec)
{
    os << "Vector(" << vec.x() << ", " << vec.y() << ", " << vec.z() << ")";
    return os;
}

#endif // VECTOR_H