#pragma once
#include <string>
#include <fstream>
#include <vector>

class Tokenizer
{
public:

    static std::vector<std::string> Split(const std::string& stringToSplit, char separator)
    {
        std::vector<std::string> tokens;
        size_t end;
        size_t start = 0;
        while((end = stringToSplit.find(separator, start)) != std::string::npos)
        {
            tokens.push_back(stringToSplit.substr(start, end-start));
            start = end + 1;
        }
        tokens.push_back(stringToSplit.substr(start));
        return tokens;
    }
};

struct Float3
{
    float x;
    float y;
    float z;
};

struct Float2
{
    float x;
    float y;
};

struct Vertex
{
    Float3 point;
    Float2 uv;
    Float3 normal;
};

struct Triangle
{
    Vertex v1;
    Vertex v2;
    Vertex v3;
};

struct Obj
{
    std::vector<Triangle> triangles;
};


class ObjParser
{
public:
    bool TryParseObj(const std::string& objPath, Obj& outObj)
    {
        std::ifstream file(objPath);
        if (!file.is_open()) return false;
        std::string line;
        std::vector<Float3> points;
        std::vector<Float2> uvs;
        std::vector<Float3> normals;
        while(std::getline(file, line)) {
            if(line.starts_with("v "))
            {
                auto tokens = Tokenizer::Split(line, ' ');
                Float3 v;

                v.x = std::stof(tokens[1]);
                v.y = std::stof(tokens[2]);
                v.z = std::stof(tokens[3]);
                points.push_back(v);
            }
            if(line.starts_with("vt "))
            {
                auto tokens = Tokenizer::Split(line, ' ');
                Float2 vt;

                vt.x = std::stof(tokens[1]);
                vt.y = std::stof(tokens[2]);
                uvs.push_back(vt);
            }
            if(line.starts_with("vn "))
            {
                auto tokens = Tokenizer::Split(line, ' ');
                Float3 vn;

                vn.x = std::stof(tokens[1]);
                vn.y = std::stof(tokens[2]);
                vn.z = std::stof(tokens[3]);
                normals.push_back(vn);
            }
            if(line.starts_with("f "))
            {
                auto spacedTokens = Tokenizer::Split(line, ' ');
                Triangle triangle;
                for(size_t i=1; i<=3; ++i)
                {
                    auto tokens = Tokenizer::Split(spacedTokens[i], '/');
                    int pointIndex = std::stoi(tokens[0]);
                    int uvIndex = std::stoi(tokens[1]);
                    int normalIndex = std::stoi(tokens[2]);
                    Vertex v;
                    v.point = points[pointIndex - 1];
                    v.uv = uvs[uvIndex - 1];
                    v.normal = normals[normalIndex - 1];
                }
                outObj.triangles.push_back(triangle);
            }

        }
        return false;
    }

private:

};