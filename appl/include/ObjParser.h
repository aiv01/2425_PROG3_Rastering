#pragma once
#include <string>
#include <fstream>
#include <vector>

<<<<<<< Updated upstream
=======
#define VERTEX_PREFIX "v "
#define UV_PREFIX "vt "
#define NORMAL_PREFIX "vn "
#define FACE_PREFIX "f "

struct Float3
{
    float a;
    float b;
    float c;

    bool operator==(const Float3& other) const
    {
        return a == other.a && b == other.b && c == other.c; 
    }
};

struct Float2
{
    float a;
    float b;

    bool operator==(const Float2& other) const
    {
        return a == other.a && b == other.b;
    }
};

>>>>>>> Stashed changes
class Tokenizer
{
public:
    static std::vector<std::string> Split(const std::string& toSplit, char separator)
    {
        std::vector<std::string> tokens;
        size_t end;
        size_t start = 0;
        while((end = toSplit.find(separator, start)) != std::string::npos)
        {
            tokens.push_back(toSplit.substr(start, end - start));
            start = end + 1;
        }
        tokens.push_back(toSplit.substr(start));
        return tokens;
    }
};

<<<<<<< Updated upstream
struct Obj
{

=======
struct Vertex
{
    Float3 position;
    Float3 normal;
    Float2 uv;

    bool operator==(const Vertex& other) const
    {
        return position == other.position && normal == other.normal && uv == other.uv;
    }
};

struct Triangle
{
    Vertex v1;
    Vertex v2;
    Vertex v3;

    bool operator==(const Triangle& other) const 
    {
        return v1 == other.v1 && v2 == other.v2 && v3 == other.v3;
    }
};

struct Obj
{
    std::vector<Triangle> triangles;
>>>>>>> Stashed changes
};

class ObjParser
{
public:
    bool TryParseObj(const std::string& objPath, Obj& outObj)
    {
        std::ifstream file(objPath);
        if(!file.is_open()) return false;

<<<<<<< Updated upstream
        std::string line;
        while(std::getline(file, line))
        {
            if(line.starts_with("v "))
            {

            }
        }

        return false;
=======
        std::vector<Float3> vertexes;
        std::vector<Float2> uvs;
        std::vector<Float3> normals;

        std::string line;

        while(std::getline(file, line))
        {
            if(line.starts_with(VERTEX_PREFIX))
            {
                auto tokens = Tokenizer::Split(line, ' ');
                Float3 v;
                v.a = std::stof(tokens[1]);
                v.b = std::stof(tokens[2]);
                v.c = std::stof(tokens[3]);

                vertexes.push_back(v);
            }
            else if(line.starts_with(UV_PREFIX))
            {
                auto tokens = Tokenizer::Split(line, ' ');
                Float2 uv;
                uv.a = std::stof(tokens[1]);
                uv.b = std::stof(tokens[2]);

                uvs.push_back(uv);
            }
            else if(line.starts_with(NORMAL_PREFIX))
            {
                auto tokens = Tokenizer::Split(line, ' ');
                Float3 normal;
                normal.a = std::stof(tokens[1]);
                normal.b = std::stof(tokens[2]);
                normal.c = std::stof(tokens[3]);

                normals.push_back(normal);
            }
            else if(line.starts_with(FACE_PREFIX))
            {
                Triangle t;
                // get pointer to first triangle struct field
                Vertex* currVertex = &t.v1;
                auto spacedTokens = Tokenizer::Split(line, ' ');
                for(size_t i = 1; i < spacedTokens.size(); ++i)
                {
                    auto tabbedTokens = Tokenizer::Split(spacedTokens[i], '/');
                    size_t vertexIndex = std::stoi(tabbedTokens[0]);
                    size_t uvIndex = std::stoi(tabbedTokens[1]);
                    size_t normalIndex = std::stoi(tabbedTokens[2]);

                    Vertex v{vertexes[vertexIndex - 1], normals[normalIndex - 1], uvs[uvIndex - 1]};
                    // assign currVertex and increment ptr
                    *currVertex++ = v;
                }

                outObj.triangles.push_back(t);
            }
        }

        return true;
>>>>>>> Stashed changes
    }

private:

};