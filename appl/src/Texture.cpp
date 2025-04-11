#include "Texture.h"
#include <iostream>


Texture* Texture::load_from_file(const std::string& file_path)
{
    SDL_Surface* surface = IMG_Load(file_path.c_str());
    if (surface == NULL) {
        std::cout << "Error loading image: " << file_path << "\n";
        return nullptr;
    }

    int width = surface->w;
    int height = surface->h;
    int bytes_per_pixel = surface->format->BytesPerPixel;

    uint8_t* data = reinterpret_cast<uint8_t*>(surface->pixels);
    size_t size = width * height * bytes_per_pixel;

    std::vector<uint8_t> pixels(data, data + size);

    auto* t = new Texture(width, height, bytes_per_pixel, pixels);
    
    SDL_FreeSurface(surface);
    return t;
}

Texture::Texture(int in_width, int in_height, int in_pixel_size, std::vector<uint8_t> in_pixels) 
    : width(in_width), height(in_height), pixel_size(in_pixel_size),
      pixels(in_pixels)
{

}