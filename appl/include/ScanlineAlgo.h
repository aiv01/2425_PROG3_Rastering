#pragma once

#include "Color.h"
#include "Screen.h"
#include "Maths.h"


struct GpuVertex
{
    Vector2i screen_pos;
    Color color;
};


class ScanlineAlgo {
public:
    static void rasterize(GpuVertex& p1, GpuVertex& p2, GpuVertex& p3, Screen& screen);
};

