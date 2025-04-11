#pragma once
#include "SDL_image.h"
#include <string>
#include <cstdint>
#include <vector>

class Texture {
public:
    static Texture* load_from_file(const std::string& file_path);
    Texture(int width, int height, int pixel_size, std::vector<uint8_t> pixels);

public:
    int width;
    int height;
    int pixel_size;
    std::vector<uint8_t> pixels;
};