#include "Maths.h"
#include <algorithm>
#include <cmath>

int min3(int a, int b, int c) {
    return std::min(std::min(a, b), c);
}

int max3(int a, int b, int c) {
    return std::max(std::max(a, b), c);
}

int det(Vector2i p, Vector2i v1, Vector2i v2) {
    //V1_V2
    int vx = v2.x - v1.x;
    int vy = v2.y - v1.y;

    //V1_P
    int px = p.x - v1.x;
    int py = p.y - v1.y;

    return vx * py - vy * px;
}

Vector3f Vector3f::operator-(const Vector3f& other) const {
    return Vector3f{x - other.x, y - other.y, z - other.z};
}

Vector3f Vector3f::rotate_y(float angle) const {
    
    float rads = angle * PI / 180.f;

    Vector3f result;
    result.x = cosf(rads) * x - sinf(rads) * z;
    result.y = y;
    result.z = sinf(rads) * x + cosf(rads) * z;
    return result;
}

Vector3f Vector3f::operator*(float scalar) const {
    return Vector3f{x * scalar, y * scalar, z * scalar};
}

void Vector3f::normalize() 
{
    float magn = magnitude();
    x /= magn;
    y /= magn;
    z /= magn;
}

float Vector3f::dot(const Vector3f& other) const 
{
    return x * other.x + y * other.y + z * other.z;
}

float Vector3f::magnitude() const {
    return sqrtf(dot(*this));
}

Vector3f Vector3f::reflect(const Vector3f& normal) const 
{
    //R = I – 2 * dot(I,N) * N
    return *this - normal * (2.f * dot(normal));
}

Vector3f Vector3f::cross(const Vector3f& other) const 
{
    Vector3f r;
    r.x = y * other.z - z * other.y;
    r.y = z * other.x - x * other.z;
    r.z = x * other.y - y * other.x;
    return r;
}