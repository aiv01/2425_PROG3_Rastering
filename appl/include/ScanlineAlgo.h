#pragma once

#include "Color.h"
#include "Screen.h"
#include "Maths.h"

#include "Texture.h"
#include <type_traits>

enum PaintingMode : uint8_t
    {
        COLOR = 1 << 0, // 0001 
        TEXTURE = 1 << 1, // 0010
        BLEND = COLOR | TEXTURE // 0011
    };

class VGpu {
public:
    Texture* texture;
    PaintingMode painting_mode = PaintingMode::COLOR;
    
};   

struct GpuVertex
{
    Vector2i screen_pos;
    Color color;
    float z_pos;
    Vector2f uv;
};

class ScanlineAlgo {
public:
    static void rasterize(VGpu& gpu, GpuVertex& p1, GpuVertex& p2, GpuVertex& p3, Screen& screen);
};

// -------------------------------------------------------------------------------------------------------

// SFINAE: uint8_t
template<typename T>
std::enable_if_t<std::is_same_v<T, uint8_t>, float> interpolate(T a, T b, float gradient) 
{
    return static_cast<float>(a) + gradient * static_cast<float>(b - a);
}

// SFINAE: float
template<typename T>
std::enable_if_t<std::is_same_v<T, float>, float> interpolate(T a, T b, float gradient) 
{
    return a + gradient * (b - a);
}

// SFINAE: Color
template<typename T>
std::enable_if_t<std::is_same_v<T, Color>, Color> interpolate(T a, T b, float gradient) 
{
    Color result;
    result.r = static_cast<uint8_t>(interpolate(a.r, b.r, gradient));
    result.g = static_cast<uint8_t>(interpolate(a.g, b.g, gradient));
    result.b = static_cast<uint8_t>(interpolate(a.b, b.b, gradient));
    result.a = static_cast<uint8_t>(interpolate(a.a, b.a, gradient));
    return result;
}

// SFINAE: Vector2f
template<typename T>
std::enable_if_t<std::is_same_v<T, Vector2f>, Vector2f> interpolate(T a, T b, float gradient) 
{
    Vector2f result;
    result.x = interpolate(a.x, b.x, gradient);
    result.y = interpolate(a.y, b.y, gradient);
    return result;
}