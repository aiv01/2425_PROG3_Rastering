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

struct Obj
{

};


class ObjParser
{
public:
    bool TryParseObj(const std::string& objPath, Obj& outObj)
    {
        std::ifstream file(objPath);
        if (!file.is_open()) return false;
        std::string line;
        while(std::getline(file, line)) {
            if(line.starts_with("v "))
            {

            }
        }
        return false;
    }

private:

};