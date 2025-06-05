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
    float x() const { return components.x; }
    float y() const { return components.y; }
    float z() const { return components.z; }
    bool parallel(const Vector &other) const
    {
        return glm::length(glm::cross(components, other.components)) < 1e-6f;
    }
    float length() const { return glm::length(components); }
    Vector normalize() { return Vector(glm::normalize(components)); }
    Vector operator+(const Vector &other) { return Vector(components + other.components); }
    Vector operator-(const Vector &other) { return Vector(components - other.components); }
    Vector operator*(float scalar) { return Vector(components * scalar); }
    Vector operator/(float scalar) { return Vector(components / scalar); }
    Vector operator*(const Vector &other) { return Vector(components * other.components); }
    Vector cross(const Vector &other) { return Vector(glm::cross(components, other.components)); }
    float dot(const Vector &other) const { return glm::dot(components, other.components); }
    friend std::ostream &operator<<(std::ostream &os, const Vector &vec)
    {
        os << "Vector(" << vec.x() << ", " << vec.y() << ", " << vec.z() << ")";
        return os;
    }
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

#endif // VECTOR_H