#include "Scene.h"
#include "SDL.h"
#include "Color.h"
#include "ShapeRasterizer.h"
#include "Screen.h"

Scene::Scene(int w, int h, SDL_Renderer* r) 
    : _renderer(r), _screen(Screen(w, h, r))
{ 

}

void Scene::update(float delta_time) 
{ 
    _screen.clear();

    static float x1 = 50;
    static float y1 = 50;

    static float x2 = 200;
    static float y2 = 250;

    Color red{255, 0, 0, 255};

    float speed = 10;
    x1 += (speed * delta_time);
    y1 += (speed * delta_time);

    x2 += (speed * delta_time);
    y2 += (speed * delta_time);

    ShapeRasterizer::dda_line_raster((int)x1, (int)y1, (int)x2, (int)y2, red, _screen);

    static Color green{0, 255, 0, 255};
    ShapeRasterizer::bbox_triangle_raster({50, 200}, {150, 200}, {100, 50}, green, _screen);

    //triangle by 3 edges
    ShapeRasterizer::dda_line_raster(50, 200, 150, 200, red, _screen);
    ShapeRasterizer::dda_line_raster(50, 200, 100, 50, red, _screen);
    ShapeRasterizer::dda_line_raster(150, 200, 100, 50, red, _screen);

    _screen.blit();
}

void Scene::destroy() 
{ 
    _screen.destroy();
}

