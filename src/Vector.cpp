#include "Vector.h"

using namespace SimpleECS;
double SimpleECS::Vector::dotProduct(const Vector& other) const
{
    return other.x * this->x + other.y * this->y;
}
Vector SimpleECS::Vector::operator+(const Vector& other) const
{
    return Vector(this->x + other.x, this->y + other.y);
}

Vector SimpleECS::Vector::operator-(const Vector& other) const
{
    return Vector(this->x - other.x, this->y - other.y);
}

Vector SimpleECS::Vector::operator*(const double& other) const
{
    return Vector(this->x * other, this->y * other);
}

Vector SimpleECS::Vector::operator*(const int& other) const
{
    return Vector(this->x * other, this->y * other);
}