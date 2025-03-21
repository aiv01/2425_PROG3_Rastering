#include "ShapeRasterizer.h"

void ShapeRasterizer::dda_line_raster(int x1, int y1, int x2, int y2, Color c, Screen& s) 
{
    int dx = abs(x1 - x2);
    int dy = abs(y1 - y2);

    int steps = (dx < dy) ? dy : dx;

    int sx = (x1 > x2) ? -1 : 1;
    int sy = (y1 > y2) ? -1 : 1;

    float steps_x = (float)dx * (float)sx / (float)steps;
    float steps_y = (float)dy * (float)sy / (float)steps;

    float x = x1;
    float y = y1;

    for(int i=0; i <= steps; ++i) 
    {
        //SDL_SetRenderDrawColor(r, c.r, c.g, c.b, c.a);
        //SDL_RenderDrawPoint(r, (int)x, (int)y);
        s.put_pixel(x, y, c);

        x += steps_x;
        y += steps_y;
    }
}

bool ShapeRasterizer::bbox_is_point_in_triangle(Vector2i p, Vector2i a, Vector2i b, Vector2i c) {
    bool b1 = det(p, a, b) < 0;
    bool b2 = det(p, b, c) < 0;
    bool b3 = det(p, c, a) < 0;
    return (b1 == b2) && (b2 == b3);
}

void ShapeRasterizer::bbox_triangle_raster(Vector2i p1, Vector2i p2, Vector2i p3, Color c, Screen& screen) {
    int x_min = min3(p1.x, p2.x, p3.x); 
    int y_min = min3(p1.y, p2.y, p3.y); 
    
    int x_max = max3(p1.x, p2.x, p3.x);
    int y_max = max3(p1.y, p2.y, p3.y);

    for(int x = x_min; x <= x_max; ++x) {
        for(int y = y_min; y <= y_max; ++y) {
            Vector2i point{x, y};
            if (bbox_is_point_in_triangle(point, p1, p2, p3)) {
                screen.put_pixel(x, y, c);
            }
        }
    } 
}
