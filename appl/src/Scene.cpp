#include "Scene.h"
#include "SDL.h"
#include "Color.h"
#include "ShapeRasterizer.h"
#include "Screen.h"
#include "Camera.h"
#include <iostream>
#include "ObjParser.h"
#include "ScanlineAlgo.h"
#include "Texture.h"


Obj _quad;
Obj _suzanne;
Obj _trup;

Texture* _smile_texture;
Texture* _trup_texture;

Scene::Scene(int w, int h, SDL_Renderer* r) 
    : _renderer(r), 
      _screen(Screen(w, h, r)), 
      _camera(Camera(w, h, 60.f))
{ 
    ObjParser::TryParseObj("bin/appl/resources/quad.obj", _quad);
    ObjParser::TryParseObj("bin/appl/resources/suzanne.obj", _suzanne);
    ObjParser::TryParseObj("bin/appl/resources/stormtrooper.obj", _trup);

    _smile_texture = Texture::load_from_file("bin/appl/resources/smile.png");
    _trup_texture = Texture::load_from_file("bin/appl/resources/stormtrooper.png");
}

void draw_quad(Camera& camera, Screen& screen) {

    for(int i=0; i < _quad.triangles.size(); ++i) {
        auto& triangle = _quad.triangles[i];

        Vector3f* wp1 = (Vector3f*)&(triangle.v1.point);
        Vector3f* wp2 = (Vector3f*)&(triangle.v2.point);
        Vector3f* wp3 = (Vector3f*)&(triangle.v3.point);

        wp1->z = -4;
        wp2->z = -4;
        wp3->z = -4;

        Vector2i sp1 = camera.world_to_screen_space(*wp1);
        Vector2i sp2 = camera.world_to_screen_space(*wp2);
        Vector2i sp3 = camera.world_to_screen_space(*wp3);
        
        Color green{0, 255, 0, 255};
        ShapeRasterizer::bbox_triangle_raster(sp1, sp2, sp3, green, screen);
    }
}

void draw_suzanne(Camera& camera, Screen& screen, float delta_time, bool wireframe) {

    static float rotation = 0.f;
    rotation += 10.f * delta_time;

    for(int i=0; i < _suzanne.triangles.size(); ++i) {
        auto& triangle = _suzanne.triangles[i];

        Vector3f* lp1 = (Vector3f*)&(triangle.v1.point);
        Vector3f* lp2 = (Vector3f*)&(triangle.v2.point);
        Vector3f* lp3 = (Vector3f*)&(triangle.v3.point);

        //scale -> rotate -> translate
        Vector3f wp1 = *lp1 * 2;
        Vector3f wp2 = *lp2 * 2;
        Vector3f wp3 = *lp3 * 2;

        wp1 = wp1.rotate_y(rotation);
        wp2 = wp2.rotate_y(rotation);
        wp3 = wp3.rotate_y(rotation);

        wp1.z -= 5;
        wp2.z -= 5;
        wp3.z -= 5;        

        Vector2i sp1 = camera.world_to_screen_space(wp1);
        Vector2i sp2 = camera.world_to_screen_space(wp2);
        Vector2i sp3 = camera.world_to_screen_space(wp3);
        
        Color green{0, 255, 0, 255};

        if (wireframe) {
            ShapeRasterizer::dda_line_raster(sp1.x, sp1.y, sp2.x, sp2.y, green, screen);
            ShapeRasterizer::dda_line_raster(sp1.x, sp1.y, sp3.x, sp3.y, green, screen);
            ShapeRasterizer::dda_line_raster(sp2.x, sp2.y, sp3.x, sp3.y, green, screen);
        } else {
            ShapeRasterizer::bbox_triangle_raster(sp1, sp2, sp3, green, screen);
        }
    }
}

void draw_suzanne_scanline(Camera& camera, Screen& screen, float delta_time) {

    VGpu gpu;
    gpu.blend_mode = BlendMode::COLOR;

    static float rotation = 0.f;
    rotation += 10.f * delta_time;

    for(int i=0; i < _suzanne.triangles.size(); ++i) {
        auto& triangle = _suzanne.triangles[i];

        Vector3f& lp1 = reinterpret_cast<Vector3f&>(triangle.v1.point);
        Vector3f& lp2 = reinterpret_cast<Vector3f&>(triangle.v2.point);
        Vector3f& lp3 = reinterpret_cast<Vector3f&>(triangle.v3.point);

        //scale -> rotate -> translate
        Vector3f wp1 = lp1 * 2;
        Vector3f wp2 = lp2 * 2;
        Vector3f wp3 = lp3 * 2;

        wp1 = wp1.rotate_y(rotation);
        wp2 = wp2.rotate_y(rotation);
        wp3 = wp3.rotate_y(rotation);

        wp1.z -= 5;
        wp2.z -= 5;
        wp3.z -= 5;        

        Vector2i sp1 = camera.world_to_screen_space(wp1);
        Vector2i sp2 = camera.world_to_screen_space(wp2);
        Vector2i sp3 = camera.world_to_screen_space(wp3);
        
        Vector3f cp1 = camera.world_to_camera_space(wp1);
        Vector3f cp2 = camera.world_to_camera_space(wp2);
        Vector3f cp3 = camera.world_to_camera_space(wp3);

        GpuVertex v1;
        v1.screen_pos = sp1;
        v1.color = Color{255, 0, 0, 255};
        v1.z_pos = cp1.z;

        GpuVertex v2;
        v2.screen_pos = sp2;
        v2.color = Color{0, 255, 0, 255};
        v2.z_pos = cp2.z;

        GpuVertex v3;
        v3.screen_pos = sp3;
        v3.color = Color{0, 0, 255, 255};
        v3.z_pos = cp3.z;

        ScanlineAlgo::rasterize(gpu, v1, v2, v3, screen);
    }
}

void draw_quad_texturized(Camera& camera, Screen& screen) {

    VGpu gpu;
    gpu.blend_mode = BlendMode::TEXTURE;
    gpu.texture = _smile_texture;

    for(int i=0; i < _quad.triangles.size(); ++i) {
        auto& triangle = _quad.triangles[i];

        Vector3f& lp1 = reinterpret_cast<Vector3f&>(triangle.v1.point);
        Vector3f& lp2 = reinterpret_cast<Vector3f&>(triangle.v2.point);
        Vector3f& lp3 = reinterpret_cast<Vector3f&>(triangle.v3.point);

        Vector3f wp1 = lp1;
        Vector3f wp2 = lp2;
        Vector3f wp3 = lp3;

        wp1.z -= 4;
        wp2.z -= 4;
        wp3.z -= 4;

        Vector2i sp1 = camera.world_to_screen_space(wp1);
        Vector2i sp2 = camera.world_to_screen_space(wp2);
        Vector2i sp3 = camera.world_to_screen_space(wp3);
        
        Vector3f cp1 = camera.world_to_camera_space(wp1);
        Vector3f cp2 = camera.world_to_camera_space(wp2);
        Vector3f cp3 = camera.world_to_camera_space(wp3);

        GpuVertex v1;
        v1.screen_pos = sp1;
        //v1.color = Color{255, 0, 0, 255};
        v1.z_pos = cp1.z;
        v1.uv = Vector2f(triangle.v1.uv.x, triangle.v1.uv.y);

        GpuVertex v2;
        v2.screen_pos = sp2;
        //v2.color = Color{0, 255, 0, 255};
        v2.z_pos = cp2.z;
        v2.uv = Vector2f(triangle.v2.uv.x, triangle.v2.uv.y);

        GpuVertex v3;
        v3.screen_pos = sp3;
        //v3.color = Color{0, 0, 255, 255};
        v3.z_pos = cp3.z;
        v3.uv = Vector2f(triangle.v3.uv.x, triangle.v3.uv.y);

        ScanlineAlgo::rasterize(gpu, v1, v2, v3, screen);
    }
}

void draw_trup_texturized(Camera& camera, Screen& screen, float delta_time) {

    VGpu gpu;
    gpu.blend_mode = BlendMode::TEXTURE;
    gpu.texture = _trup_texture;
    gpu.point_light_pos = {3.f, 0.f, -4.f};
    gpu.camera_pos = camera._position;

    static float rotation = 0.f;
    rotation += 10.f * delta_time;

    for(int i=0; i < _trup.triangles.size(); ++i) {
        auto& triangle = _trup.triangles[i];

        Vector3f& lp1 = reinterpret_cast<Vector3f&>(triangle.v1.point);
        Vector3f& lp2 = reinterpret_cast<Vector3f&>(triangle.v2.point);
        Vector3f& lp3 = reinterpret_cast<Vector3f&>(triangle.v3.point);

        //scale -> rotate -> translate
        float scale = 1.f;
        Vector3f wp1 = lp1 * scale;
        Vector3f wp2 = lp2 * scale;
        Vector3f wp3 = lp3 * scale;

        wp1 = wp1.rotate_y(rotation);
        wp2 = wp2.rotate_y(rotation);
        wp3 = wp3.rotate_y(rotation);

        static Vector3f transl = {0.f, 2.f, 4.f};

        wp1 = wp1 - transl;
        wp2 = wp2 - transl;
        wp3 = wp3 - transl;

        //Compute world normal
        Vector3f ln1 = Vector3f(triangle.v1.normal.x, triangle.v1.normal.y, triangle.v1.normal.z);
        Vector3f ln2 = Vector3f(triangle.v2.normal.x, triangle.v2.normal.y, triangle.v2.normal.z);
        Vector3f ln3 = Vector3f(triangle.v3.normal.x, triangle.v3.normal.y, triangle.v3.normal.z);

        Vector3f wn1 = ln1.rotate_y(rotation);
        Vector3f wn2 = ln2.rotate_y(rotation);
        Vector3f wn3 = ln3.rotate_y(rotation);


        Vector2i sp1 = camera.world_to_screen_space(wp1);
        Vector2i sp2 = camera.world_to_screen_space(wp2);
        Vector2i sp3 = camera.world_to_screen_space(wp3);
        
        Vector3f cp1 = camera.world_to_camera_space(wp1);
        Vector3f cp2 = camera.world_to_camera_space(wp2);
        Vector3f cp3 = camera.world_to_camera_space(wp3);

        GpuVertex v1;
        v1.screen_pos = sp1;
        v1.z_pos = cp1.z;
        v1.uv = Vector2f(triangle.v1.uv.x, triangle.v1.uv.y);
        v1.world_norm = wn1;
        v1.world_pos = wp1;

        GpuVertex v2;
        v2.screen_pos = sp2;
        v2.z_pos = cp2.z;
        v2.uv = Vector2f(triangle.v2.uv.x, triangle.v2.uv.y);
        v2.world_norm = wn2;
        v2.world_pos = wp2;
        
        GpuVertex v3;
        v3.screen_pos = sp3;
        v3.z_pos = cp3.z;
        v3.uv = Vector2f(triangle.v3.uv.x, triangle.v3.uv.y);
        v3.world_norm = wn3;
        v3.world_pos = wp3;

        ScanlineAlgo::rasterize(gpu, v1, v2, v3, screen);
    }
}


void Scene::update(float delta_time) 
{   
    _screen.clear({128, 128, 128, 255});
    /*
    static float x1 = 50;
    static float y1 = 50;

    static float x2 = 200;
    static float y2 = 250;

    Color red{255, 0, 0, 255};

    float speed = 10;
    x1 += (speed * delta_time);
    y1 += (speed * delta_time);

    x2 += (speed * delta_time);
    y2 += (speed * delta_time);

    ShapeRasterizer::dda_line_raster((int)x1, (int)y1, (int)x2, (int)y2, red, _screen);

    static Color green{0, 255, 0, 255};
    ShapeRasterizer::bbox_triangle_raster({50, 200}, {150, 200}, {100, 50}, green, _screen);

    //triangle by 3 edges
    ShapeRasterizer::dda_line_raster(50, 200, 150, 200, red, _screen);
    ShapeRasterizer::dda_line_raster(50, 200, 100, 50, red, _screen);
    ShapeRasterizer::dda_line_raster(150, 200, 100, 50, red, _screen);
    */

    //triangle in world space
    /*
    Vector3f wp1{ 0.f, 0.f, -10.f};
    Vector3f wp2{-4.f, -4.f, -10.f};
    Vector3f wp3{ 4.f, -4.f, -10.f};

    Vector2i sp1 = _camera.world_to_screen_space(wp1);
    Vector2i sp2 = _camera.world_to_screen_space(wp2);
    Vector2i sp3 = _camera.world_to_screen_space(wp3);

    ShapeRasterizer::bbox_triangle_raster(sp1, sp2, sp3, green, _screen);
    */

    //draw_quad(_camera, _screen);

    //draw_suzanne(_camera, _screen, delta_time, true);

    //draw_suzanne_scanline(_camera, _screen, delta_time);
    //draw_quad_texturized(_camera, _screen);

    draw_trup_texturized(_camera, _screen, delta_time);

    _screen.blit();
}

void Scene::destroy() 
{ 
    _screen.destroy();
}


