#pragma once

#include "Color.h"
#include "Screen.h"

void dda_line_raster(int x1, int y1, int x2, int y2, Color c, Screen& s) 
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
