#pragma once

#include "Color.h"
#include "Screen.h"
#include "Maths.h"

#include "Texture.h"

class VGpu {
public:
    Texture* texture;
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

