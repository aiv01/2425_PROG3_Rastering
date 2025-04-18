#include "Color.h"
#include <algorithm>

Color Color::operator*(float scale) const {
    Color result;
    result.r = static_cast<uint8_t>(static_cast<float>(r) * scale);
    result.g = static_cast<uint8_t>(static_cast<float>(g) * scale);
    result.b = static_cast<uint8_t>(static_cast<float>(b) * scale);
    result.a = static_cast<uint8_t>(static_cast<float>(a) * scale);
    return result;
}

Color Color::operator+(const Color& other) const {
    Color result;
    result.r = std::min(r + other.r, 255);
    result.g = std::min(g + other.g, 255);
    result.b = std::min(b + other.b, 255);
    result.a = std::min(a + other.a, 255);
    return result;
}