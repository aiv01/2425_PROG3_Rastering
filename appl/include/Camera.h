#pragma once
#include "Maths.h"
#include <cmath>

#define PI 3.14159265359

class Camera {
public:
    Camera(int screen_witdh, int screen_height, float fov);
    Vector2i world_to_screen_space(Vector3f world_point);
private:
    Vector3f _position;
    int _screen_width;
    int _screen_height;
    float _fov;
};
