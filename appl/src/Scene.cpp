#include "Scene.h"
#include "SDL.h"
#include "Color.h"
#include "ShapeRasterizer.h"
#include "Screen.h"
#include "Camera.h"
#include <iostream>
#include "ObjParser.h"
#include "ScanlineAlgo.h"

Obj _quad;
Obj _suzanne;

Scene::Scene(int w, int h, SDL_Renderer* r) 
    : _renderer(r), 
      _screen(Screen(w, h, r)), 
      _camera(Camera(w, h, 60.f))
{ 
    ObjParser::TryParseObj("bin/appl/resources/quad.obj", _quad);
    ObjParser::TryParseObj("bin/appl/resources/suzanne.obj", _suzanne);
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

        wp1.z = -5;
        wp2.z = -5;
        wp3.z = -5;        

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

        wp1.z = -5;
        wp2.z = -5;
        wp3.z = -5;        

        Vector2i sp1 = camera.world_to_screen_space(wp1);
        Vector2i sp2 = camera.world_to_screen_space(wp2);
        Vector2i sp3 = camera.world_to_screen_space(wp3);
        
        ScanlineAlgo::rasterize(sp1, sp2, sp3, screen);
    }
}


void Scene::update(float delta_time) 
{ 
    _screen.clear();
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

    draw_suzanne_scanline(_camera, _screen, delta_time);

    _screen.blit();
}

void Scene::destroy() 
{ 
    _screen.destroy();
}


