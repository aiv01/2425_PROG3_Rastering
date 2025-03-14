#include "Screen.h"
#include <algorithm>

Screen::Screen(int w, int h, SDL_Renderer* r)
{
    _color_buffer.resize(w * h);
    _width = w;
    _height = h;
    _renderer = r;
    _texture = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STATIC, _width, _height);
}

void Screen::put_pixel(int x, int y, Color c)
{
    if (x < 0 || x >= _width) return;
    if (y < 0 || y >= _height) return;
    
    int index = y * _width + x;
    _color_buffer[index] = c;
}

void Screen::blit() 
{
    SDL_UpdateTexture(_texture, NULL, _color_buffer.data(), _width * 4);
    SDL_RenderCopy(_renderer, _texture, NULL, NULL);
}

void Screen::clear() 
{
    Color black{0, 0, 0, 0};
    std::fill(_color_buffer.begin(), _color_buffer.end(), black);
}

void Screen::destroy() 
{
    SDL_DestroyTexture(_texture);
}