#pragma once
#include "Color.h"
#include <vector>
#include "SDL.h"

class Screen {
public:
    Screen(int w, int h, SDL_Renderer* r);
    void put_pixel(int x, int y, Color c);
    void put_pixel(int x, int y, float z, Color c);
    void blit();
    void clear();
    void destroy();
private:
    std::vector<Color> _color_buffer;
    std::vector<float> _depth_buffer;
    int _width;
    int _height;
    SDL_Texture* _texture;
    SDL_Renderer* _renderer;
};