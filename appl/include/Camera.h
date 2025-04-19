#pragma once
#include "Maths.h"
#include <cmath>

class Camera {
public:
    Camera(int screen_witdh, int screen_height, float fov);
    Vector2i world_to_screen_space(Vector3f world_point);
    Vector3f world_to_camera_space(const Vector3f& world_point) const;
    
    bool is_face_culled(const Vector3f& p1, const Vector3f& p2, const Vector3f& p3);
    bool is_frustrum_culled(const Vector2i& sp1, float cpz1, const Vector2i& sp2, float cpz2, const Vector2i& sp3, float cpz3);

    Vector3f _position;
private:
    int _screen_width;
    int _screen_height;
    float _fov;
};
