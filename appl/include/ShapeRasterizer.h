#pragma once

#include "Color.h"
#include "Screen.h"
#include "Maths.h"

class ShapeRasterizer {
public:
    static void dda_line_raster(int x1, int y1, int x2, int y2, Color c, Screen& s);
    static void bbox_triangle_raster(Vector2i p1, Vector2i p2, Vector2i p3, Color c, Screen& screen);
private:
    static bool bbox_is_point_in_triangle(Vector2i p, Vector2i a, Vector2i b, Vector2i c);
};

