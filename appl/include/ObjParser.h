#pragma once

#include <string>
#include <fstream>
#include <vector>

constexpr const char* points_prefix = "v ";
constexpr const char* uvs_prefix = "vt ";
constexpr const char* normals_prefix = "vn ";
constexpr const char* faces_prefix = "f ";
constexpr const char white_space = ' ';
constexpr const char front_slash = '/';
constexpr uint16_t vertexes_of_triangle = 3;

// (x, y)
struct Float2
{
    float x;
    float y;
};

// (x, y, z)
struct Float3
{
    float x;
    float y;
    float z;
};

/// @brief Vertex structure, with a point (x, y, z), an uv (x, y) and a normal (x, y, z).
struct Vertex
{
    Float3 point;
    Float2 uv;
    Float3 normal;
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

        std::vector<Float3> points; // v
        std::vector<Float2> uvs; // vt
        std::vector<Float3> normals; // vn

        std::string line;
        while (std::getline(file, line))
        {
            if (line.starts_with(points_prefix))
            {
                auto tokens = Tokenizer::split(line, white_space);
                
                Float3 v;
                v.x = std::stof(tokens[1]);
                v.y = std::stof(tokens[2]);
                v.z = std::stof(tokens[3]);

                points.push_back(v);
            }
            else if (line.starts_with(uvs_prefix))
            {
                auto tokens = Tokenizer::split(line, white_space);

                Float2 vt;
                vt.x = std::stof(tokens[1]);
                vt.y = std::stof(tokens[2]);

                uvs.push_back(vt);
            }
            else if (line.starts_with(normals_prefix))
            {
                auto tokens = Tokenizer::split(line, white_space);

                Float3 vn;
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

                    auto* vertex_ptr = reinterpret_cast<Vertex*>(&triangle) + index;
                    *vertex_ptr = vertex;
                }

                obj_out.triangles.push_back(triangle);
            }
        }

        return true;
    }

};