#pragma once

#include "SDL.h"

class Scene {
public:
    Scene(int w, int h, SDL_Renderer*);
    void Update(float delta_time);
    void Destroy();
};