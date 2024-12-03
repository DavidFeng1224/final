#ifndef VECTOR_2D_H
#define VECTOR_2D_H

using namespace std;

class Vector2D
{
private:
    double x, y;

public:
    // Constructors
    Vector2D();
    Vector2D(double x, double y);

    // Getters and setters
    double getX() const;
    double getY() const;
    void setX(double x);
    void setY(double y);

    // Vector operations
    Vector2D operator+(const Vector2D &other) const;
    Vector2D operator-(const Vector2D &other) const;
    Vector2D operator*(double scalar) const; // Scalar multiplication
    double dot(const Vector2D &other) const; // Dot product

    // Magnitude and normalization
    double mag() const;
    Vector2D norm() const;
};

#endif