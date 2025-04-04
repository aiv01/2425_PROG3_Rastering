#pragma once

#include "Color.h"
#include "Screen.h"
#include "Maths.h"

class ScanlineAlgo {
public:
    static void rasterize(Vector2i& p1, Vector2i& p2, Vector2i& p3, Screen& screen);
};

