#include "Scene.h"
#include "SDL.h"
#include "Color.h"
#include "LineRaster.h"
#include "Screen.h"

Scene::Scene(int w, int h, SDL_Renderer* r) 
    : _renderer(r), _screen(Screen(w, h, r))
{ 

}

void Scene::Update(float delta_time) 
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

    dda_line_raster((int)x1, (int)y1, (int)x2, (int)y2, red, _screen);

    _screen.blit();
}

void Scene::Destroy() 
{ 
    _screen.destroy();
}

