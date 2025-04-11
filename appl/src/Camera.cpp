#include "Camera.h"

Camera::Camera(int screen_witdh, int screen_height, float fov) 
    : 
        _position({0.f, 0.f, 0.f}),
        _screen_width(screen_witdh),
        _screen_height(screen_height),
        _fov(fov)
{ }

Vector2i Camera::world_to_screen_space(Vector3f world_point) {
    Vector3f camera_point = world_point - _position;

    float plane_x = camera_point.x / -camera_point.z;
    float plane_y = camera_point.y / -camera_point.z;

    float half_fov_degrees = _fov * 0.5f;
    float half_fov_radians = half_fov_degrees * PI / 180.f;
    float tan_fov = tanf(half_fov_radians);

    plane_x /= tan_fov;
    plane_y /= tan_fov;

    float aspect = (float)_screen_width / (float) _screen_height;
    plane_x /= aspect;
        
    // Plane {-1, 1} to Screen {w, h}
    int screen_x  = (int) ( (plane_x + 1.f) * 0.5f * (float)_screen_width );
    int screen_y  = (int) ( (1.f - (plane_y + 1.f) * 0.5f) * (float)_screen_height );

    return {screen_x, screen_y};
}

Vector3f Camera::world_to_camera_space(const Vector3f& world_point) const 
{
    Vector3f camera_point = world_point - _position;
    return camera_point;
}