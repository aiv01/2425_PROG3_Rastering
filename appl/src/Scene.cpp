#include "Scene.h"
#include "SDL.h"
#include "Color.h"
#include "ShapeRasterizer.h"
#include "Screen.h"
#include "Camera.h"
#include <iostream>

#define OBJPARSER_IMPLEMENTATION
#include "ObjParser.h"

const std::string quad_path("bin/appl/resources/quad.obj");

Scene::Scene(int w, int h, SDL_Renderer* r) 
    : _renderer(r), 
      _screen(Screen(w, h, r)), 
      _camera(Camera(w, h, 60.f))
{ 
}

void Scene::update(float delta_time) 
{ 
    _screen.clear();

    // static float x1 = 50;
    // static float y1 = 50;

    // static float x2 = 200;
    // static float y2 = 250;

    // Color red{255, 0, 0, 255};

    // float speed = 10;
    // x1 += (speed * delta_time);
    // y1 += (speed * delta_time);

    // x2 += (speed * delta_time);
    // y2 += (speed * delta_time);

    // ShapeRasterizer::dda_line_raster((int)x1, (int)y1, (int)x2, (int)y2, red, _screen);

    static Color green{0, 255, 0, 255};
    // ShapeRasterizer::bbox_triangle_raster({50, 200}, {150, 200}, {100, 50}, green, _screen);

    // //triangle by 3 edges
    // ShapeRasterizer::dda_line_raster(50, 200, 150, 200, red, _screen);
    // ShapeRasterizer::dda_line_raster(50, 200, 100, 50, red, _screen);
    // ShapeRasterizer::dda_line_raster(150, 200, 100, 50, red, _screen);

    // //triangle in world space
    // Vector3f wp1{ 0.f, 0.f, -10.f};
    // Vector3f wp2{-4.f, -4.f, -10.f};
    // Vector3f wp3{ 4.f, -4.f, -10.f};

    // Vector2i sp1 = _camera.world_to_screen_space(wp1);
    // Vector2i sp2 = _camera.world_to_screen_space(wp2);
    // Vector2i sp3 = _camera.world_to_screen_space(wp3);

    // ShapeRasterizer::bbox_triangle_raster(sp1, sp2, sp3, green, _screen);

    // --------------------------------------------------------------------------------------------------

    Obj quad_obj;

    if (ObjParser::try_parse_obj(quad_path, quad_obj))
    {
        for (const Triangle& triangle : quad_obj.triangles)
        {
            // std::cout << "First vertex 3D: " << "(" << triangle.v1.point.x << ", " << triangle.v1.point.y << ", " << triangle.v1.point.z << ")\n";
            // std::cout << "Second vertex 3D: " << "(" << triangle.v2.point.x << ", " << triangle.v2.point.y << ", " << triangle.v2.point.z << ")\n";
            // std::cout << "Third vertex 3D: " << "(" << triangle.v3.point.x << ", " << triangle.v3.point.y << ", " << triangle.v3.point.z << ")\n";

            Vector2i first_vertex = _camera.world_to_screen_space(triangle.v1.point);
            Vector2i second_vertex = _camera.world_to_screen_space(triangle.v2.point);
            Vector2i third_vertex = _camera.world_to_screen_space(triangle.v3.point);

            // std::cout << "First vertex 2D: " << "(" << first_vertex.x << ", " << first_vertex.y << ")" << "\n";
            // std::cout << "Second vertex 2D: " << "(" << second_vertex.x << ", " << second_vertex.y << ")" << "\n";
            // std::cout << "Third vertex 2D: " << "(" << third_vertex.x << ", " << third_vertex.y << ")" << "\n";

            ShapeRasterizer::bbox_triangle_raster(first_vertex, second_vertex, third_vertex, green, _screen);
        }
    }

    _screen.blit();
}

void Scene::destroy() 
{ 
    _screen.destroy();
}

