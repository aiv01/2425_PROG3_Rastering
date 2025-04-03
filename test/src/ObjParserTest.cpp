#define CLOVE_SUITE_NAME ObjParserTest
#include "clove-unit.h"
#include "ObjParser.h"
#include <filesystem>

CLOVE_TEST(Tokenizer4Token) {
   auto tokens = Tokenizer::Split("v -1.000000 1.000000 0.000000",' ');
   CLOVE_SIZET_EQ(4, tokens.size());
   CLOVE_STRING_EQ("v", tokens[0].c_str());
   CLOVE_STRING_EQ("-1.000000", tokens[1].c_str());
   CLOVE_STRING_EQ("1.000000", tokens[2].c_str());
   CLOVE_STRING_EQ("0.000000", tokens[3].c_str());
}

CLOVE_TEST(ParseQuadObj){
   ObjParser parser;
   Obj obj;
   std::string objPath = "test/resources/quad.obj";
    
   // Verifica se il file esiste nel percorso specificato
   CLOVE_IS_TRUE(std::filesystem::exists(objPath));

   bool result = parser.TryParseObj(objPath, obj);

   CLOVE_IS_TRUE(result);
   
   CLOVE_SIZET_EQ(2, obj.triangles.size());
   
   //Tiangle 1

   //Vertex 1
   CLOVE_FLOAT_EQ(1.000000, obj.triangles[0].v1.point.x);
   CLOVE_FLOAT_EQ(-1.000000, obj.triangles[0].v1.point.y);
   CLOVE_FLOAT_EQ(0.000000, obj.triangles[0].v1.point.z);

   CLOVE_FLOAT_EQ(1.000000, obj.triangles[0].v1.uv.x);
   CLOVE_FLOAT_EQ(0.000000, obj.triangles[0].v1.uv.y);

   CLOVE_FLOAT_EQ(0.000000, obj.triangles[0].v1.normal.x);
   CLOVE_FLOAT_EQ(0.000000, obj.triangles[0].v1.normal.y);
   CLOVE_FLOAT_EQ(1.000000, obj.triangles[0].v1.normal.z);

   //Vertex 2
   CLOVE_FLOAT_EQ(-1.000000, obj.triangles[0].v2.point.x);
   CLOVE_FLOAT_EQ(-1.000000, obj.triangles[0].v2.point.y);
   CLOVE_FLOAT_EQ(0.000000, obj.triangles[0].v2.point.z);

   CLOVE_FLOAT_EQ(0.000000, obj.triangles[0].v2.uv.x);
   CLOVE_FLOAT_EQ(0.000000, obj.triangles[0].v2.uv.y);

   CLOVE_FLOAT_EQ(0.000000, obj.triangles[0].v2.normal.x);
   CLOVE_FLOAT_EQ(0.000000, obj.triangles[0].v2.normal.y);
   CLOVE_FLOAT_EQ(1.000000, obj.triangles[0].v2.normal.z);

   //Vertex 3
   CLOVE_FLOAT_EQ(-1.000000, obj.triangles[0].v3.point.x);
   CLOVE_FLOAT_EQ(1.000000, obj.triangles[0].v3.point.y);
   CLOVE_FLOAT_EQ(0.000000, obj.triangles[0].v3.point.z);

   CLOVE_FLOAT_EQ(0.000000, obj.triangles[0].v3.uv.x);
   CLOVE_FLOAT_EQ(1.000000, obj.triangles[0].v3.uv.y);

   CLOVE_FLOAT_EQ(0.000000, obj.triangles[0].v3.normal.x);
   CLOVE_FLOAT_EQ(0.000000, obj.triangles[0].v3.normal.y);
   CLOVE_FLOAT_EQ(1.000000, obj.triangles[0].v3.normal.z);



   //Tiangle 2

   //Vertex 1
   CLOVE_FLOAT_EQ(1.000000, obj.triangles[1].v1.point.x);
   CLOVE_FLOAT_EQ(1.000000, obj.triangles[1].v1.point.y);
   CLOVE_FLOAT_EQ(0.000000, obj.triangles[1].v1.point.z);

   CLOVE_FLOAT_EQ(1.000000, obj.triangles[1].v1.uv.x);
   CLOVE_FLOAT_EQ(1.000000, obj.triangles[1].v1.uv.y);

   CLOVE_FLOAT_EQ(0.000000, obj.triangles[1].v1.normal.x);
   CLOVE_FLOAT_EQ(0.000000, obj.triangles[1].v1.normal.y);
   CLOVE_FLOAT_EQ(1.000000, obj.triangles[1].v1.normal.z);
   
   //Vertex 2
   CLOVE_FLOAT_EQ(1.000000, obj.triangles[1].v2.point.x);
   CLOVE_FLOAT_EQ(-1.000000, obj.triangles[1].v2.point.y);
   CLOVE_FLOAT_EQ(0.000000, obj.triangles[1].v2.point.z);

   CLOVE_FLOAT_EQ(1.000000, obj.triangles[1].v2.uv.x);
   CLOVE_FLOAT_EQ(0.000000, obj.triangles[0].v2.uv.y);

   CLOVE_FLOAT_EQ(0.000000, obj.triangles[1].v2.normal.x);
   CLOVE_FLOAT_EQ(0.000000, obj.triangles[1].v2.normal.y);
   CLOVE_FLOAT_EQ(1.000000, obj.triangles[1].v2.normal.z);

   //Vertex 3
   CLOVE_FLOAT_EQ(-1.000000, obj.triangles[1].v3.point.x);
   CLOVE_FLOAT_EQ(1.000000, obj.triangles[1].v3.point.y);
   CLOVE_FLOAT_EQ(0.000000, obj.triangles[1].v3.point.z);

   CLOVE_FLOAT_EQ(0.000000, obj.triangles[1].v3.uv.x);
   CLOVE_FLOAT_EQ(1.000000, obj.triangles[0].v3.uv.y);

   CLOVE_FLOAT_EQ(0.000000, obj.triangles[1].v3.normal.x);
   CLOVE_FLOAT_EQ(0.000000, obj.triangles[1].v3.normal.y);
   CLOVE_FLOAT_EQ(1.000000, obj.triangles[1].v3.normal.z);
   
}

CLOVE_TEST(FileDoesNotExist)
{
   ObjParser parser;
   Obj obj;
   bool result = parser.TryParseObj("non_existent_file.obj", obj);

   CLOVE_IS_FALSE(result);
   CLOVE_SIZET_EQ(0, obj.triangles.size());

}

CLOVE_TEST(EmptyFile)
{
   ObjParser parser;
   Obj obj;
   std::string objPath = "test/resources/empty.obj";
   bool result = parser.TryParseObj(objPath, obj); 

   CLOVE_IS_FALSE(result);
   CLOVE_SIZET_EQ(0, obj.triangles.size());
}

CLOVE_TEST(WrongFile)
{
   ObjParser parser;
   Obj obj;
   std::string objPath = "test/resources/wrong.obj";

   bool result = parser.TryParseObj(objPath, obj);
   CLOVE_IS_FALSE(result);
   CLOVE_SIZET_EQ(0, obj.triangles.size());
}

CLOVE_TEST(MissingNormals)
{
   ObjParser parser;
   Obj obj;
   std::string objPath = "test/resources/missingNormals.obj";

   bool result = parser.TryParseObj(objPath, obj); // facce che fanno riferimento a normali, ma non ci sono dichiarazioni 'vn'
   CLOVE_IS_FALSE(result);
   CLOVE_SIZET_EQ(0, obj.triangles.size());
}

CLOVE_TEST(WrongFace)
{
   ObjParser parser;
   Obj obj;
   std::string objPath = "test/resources/wrongFace.obj";

   bool result = parser.TryParseObj(objPath, obj);
   CLOVE_IS_FALSE(result);
   CLOVE_SIZET_EQ(0, obj.triangles.size());
}