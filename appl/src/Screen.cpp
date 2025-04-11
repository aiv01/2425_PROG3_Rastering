#include "Screen.h"
#include <algorithm>
#include <limits>
#include <iostream>

Screen::Screen(int w, int h, SDL_Renderer* r)
{
    _color_buffer.resize(w * h);
    _depth_buffer.resize(w * h);
    _width = w;
    _height = h;
    _renderer = r;
    _texture = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STATIC, _width, _height);
}

void Screen::put_pixel(int x, int y, Color c) {
    this->put_pixel(x, y, 0.f, c);
}

void Screen::put_pixel(int x, int y, float z, Color c)
{
    if (x < 0 || x >= _width) return;
    if (y < 0 || y >= _height) return;
    
    int index = y * _width + x;
    
    float prev_z = _depth_buffer[index];
    if ( prev_z > z) return;

    _depth_buffer[index] = z;
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
    std::fill(_depth_buffer.begin(), _depth_buffer.end(), std::numeric_limits<float>().lowest());

    /*
    //#include <bit>
    //std::memset(_depth_buffer.data(), 0xFF, _depth_buffer.size() * sizeof(float));
    float my_pattern = std::bit_cast<float>(0xFFFFFFFF); //0xff7fffff 
    std::fill(_depth_buffer.begin(), _depth_buffer.end(), my_pattern);

    auto val = std::numeric_limits<float>::lowest();
    auto bits = std::bit_cast<uint32_t>(val);
    std::cout << "Bit pattern: 0x" << std::hex << bits << std::endl;
    */
}

void Screen::destroy() 
{
    SDL_DestroyTexture(_texture);
}