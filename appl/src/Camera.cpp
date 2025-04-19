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

bool Camera::is_face_culled(const Vector3f& cp1, const Vector3f& cp2, const Vector3f& cp3)
{
    Vector3f v12 = cp2 - cp1;
    Vector3f v13 = cp3 - cp1;

    Vector3f face_normal = v12.cross(v13);

    Vector3f cp1_to_camera = cp1 * -1.f;

    float dot = cp1_to_camera.dot(face_normal);
    if (dot >=0) return false; //visible
    return true;
}

bool Camera::is_frustrum_culled(const Vector2i& sp1, float cpz1, const Vector2i& sp2, float cpz2, const Vector2i& sp3, float cpz3)
{
    if (sp1.x < 0 || sp2.x < 0 || sp3.x < 0) return true;
    if (sp1.y < 0 || sp2.y < 0 || sp3.y < 0) return true;
    if (sp1.x > _screen_width || sp2.x > _screen_width || sp3.x > _screen_width) return true;
    if (sp1.y > _screen_height || sp2.y > _screen_height || sp3.y > _screen_height) return true;
    //Eventual check for z near and far
    return false;
}