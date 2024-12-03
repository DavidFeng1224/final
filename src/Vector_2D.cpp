#include "Vector_2D.h"
#include <cmath>

// Constructors
Vector2D::Vector2D() : x(0), y(0) {}

Vector2D::Vector2D(double x, double y) : x(x), y(y) {}

// Getters and setters
double Vector2D::getX() const
{
    return x;
}

double Vector2D::getY() const
{
    return y;
}

void Vector2D::setX(double x)
{
    this->x = x;
}

void Vector2D::setY(double y)
{
    this->y = y;
}

// Vector operations
Vector2D Vector2D::operator+(const Vector2D &other) const
{
    return Vector2D(x + other.x, y + other.y);
}

Vector2D Vector2D::operator-(const Vector2D &other) const
{
    return Vector2D(x - other.x, y - other.y);
}

Vector2D Vector2D::operator*(double scalar) const
{
    return Vector2D(x * scalar, y * scalar);
}

double Vector2D::dot(const Vector2D &other) const
{
    return x * other.x + y * other.y;
}

// Magnitude and normalization
double Vector2D::mag() const
{
    return sqrt(x * x + y * y);
}

Vector2D Vector2D::norm() const
{
    double magnitude = mag();
    return (magnitude == 0) ? Vector2D(0, 0) : Vector2D(x / magnitude, y / magnitude);
}
