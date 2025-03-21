#pragma once

#include "SDL.h"
#include "Screen.h"

class Scene {
public:
    Scene(int w, int h, SDL_Renderer* r);
    void update(float delta_time);
    void destroy();

private:
    SDL_Renderer* _renderer;
    Screen _screen;
};