#pragma once

#include <string>
#include <fstream>
#include <vector>
#include "Maths.h"

constexpr const char* points_prefix = "v ";
constexpr const char* uvs_prefix = "vt ";
constexpr const char* normals_prefix = "vn ";
constexpr const char* faces_prefix = "f ";
constexpr const char white_space = ' ';
constexpr const char front_slash = '/';
constexpr uint16_t vertexes_of_triangle = 3;

/// @brief Vertex structure, with a point (x, y, z), an uv (x, y) and a normal (x, y, z).
struct Vertex
{
    Vector3f point;
    Vector2f uv;
    Vector3f normal;
};

/// @brief Triangle structure, with three vertexes.
struct Triangle
{
    Vertex v1;
    Vertex v2;
    Vertex v3;
};

/// @brief List of triangles, that makes a mesh.
struct Obj
{
    std::vector<Triangle> triangles;
};

class Tokenizer;
class ObjParser;

#ifdef OBJPARSER_IMPLEMENTATION

class Tokenizer
{
public:
    static std::vector<std::string> split(const std::string& string_to_split, char separator)
    {
        size_t start = 0, end = 0;

        std::vector<std::string> tokens;
        while ((end = string_to_split.find(separator, start)) != std::string::npos)
        {
            tokens.push_back(string_to_split.substr(start, end - start));   
            start = end + 1;
        }

        tokens.push_back(string_to_split.substr(start));

        return tokens;
    }

};

class ObjParser
{
public:
    static bool try_parse_obj(const std::string& file_path, Obj& obj_out)
    {
        std::ifstream file(file_path);
        if (!file.is_open()) return false;

        std::vector<Vector3f> points; // v
        std::vector<Vector2f> uvs; // vt
        std::vector<Vector3f> normals; // vn

        std::string line;
        while (std::getline(file, line))
        {
            if (line.starts_with(points_prefix))
            {
                auto tokens = Tokenizer::split(line, white_space);
                
                Vector3f v;
                v.x = std::stof(tokens[1]);
                v.y = std::stof(tokens[2]);
                v.z = std::stof(tokens[3]);

                points.push_back(v);
            }
            else if (line.starts_with(uvs_prefix))
            {
                auto tokens = Tokenizer::split(line, white_space);

                Vector2f vt;
                vt.x = std::stof(tokens[1]);
                vt.y = std::stof(tokens[2]);

                uvs.push_back(vt);
            }
            else if (line.starts_with(normals_prefix))
            {
                auto tokens = Tokenizer::split(line, white_space);

                Vector3f vn;
                vn.x = std::stof(tokens[1]);
                vn.y = std::stof(tokens[2]);
                vn.z = std::stof(tokens[3]);

                normals.push_back(vn);
            }
            else if (line.starts_with(faces_prefix))
            {
                Triangle triangle;

                auto spaced_tokens = Tokenizer::split(line, white_space);

                for (size_t index = 0; index < vertexes_of_triangle; ++index)
                {
                    const size_t current_token_index = index + 1;
                    auto tokens = Tokenizer::split(spaced_tokens[current_token_index], front_slash);
                    
                    int point_index = std::stoi(tokens[0]);
                    int uv_index = std::stoi(tokens[1]);
                    int normal_index = std::stoi(tokens[2]);

                    Vertex vertex;
                    vertex.point = points[point_index - 1];
                    vertex.uv = uvs[uv_index - 1];
                    vertex.normal = normals[normal_index - 1];

                    std::memcpy(reinterpret_cast<Vertex*>(&triangle) + index, &vertex, sizeof(Vertex));
                    // Alternative: Triangle { std::array<Vertex, 3> triangles; }
                }

                obj_out.triangles.push_back(triangle);
            }
        }

        return true;
    }

};

#endif // OBJPARSER_IMPLEMENTATION