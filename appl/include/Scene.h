#pragma once

#include "SDL.h"
#include "Screen.h"

class Scene {
public:
    Scene(int w, int h, SDL_Renderer* r);
    void Update(float delta_time);
    void Destroy();

private:
    SDL_Renderer* _renderer;
    Screen _screen;
};