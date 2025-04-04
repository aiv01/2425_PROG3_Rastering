#define CLOVE_SUITE_NAME ObjParserTest
#include "clove-unit.h"
#include <string>

#include "ObjParser.h"

CLOVE_TEST(TokenizerFourToken)
{
   auto tokens = Tokenizer::split("v -1.000000 1.000000 0.000000", ' ');

   CLOVE_SIZET_EQ(4, tokens.size());
   CLOVE_STRING_EQ("v", tokens[0].c_str());
   CLOVE_STRING_EQ("-1.000000", tokens[1].c_str());
   CLOVE_STRING_EQ("1.000000", tokens[2].c_str());
   CLOVE_STRING_EQ("0.000000", tokens[3].c_str());
}

CLOVE_TEST(ObjParsingQuadShape)
{
   Obj quad_obj;
   std::string quad_obj_path(CLOVE_EXEC_BASE_PATH() + std::string("/resources/quad.obj"));
   
   CLOVE_IS_TRUE(ObjParser::try_parse_obj(quad_obj_path, quad_obj));
   CLOVE_SIZET_EQ(2, quad_obj.triangles.size());

   // 1° Face - Points
   CLOVE_FLOAT_EQ(1.000000, quad_obj.triangles[0].v1.point.x);
   CLOVE_FLOAT_EQ(-1.000000, quad_obj.triangles[0].v1.point.y);
   CLOVE_FLOAT_EQ(0.000000, quad_obj.triangles[0].v1.point.z);

   CLOVE_FLOAT_EQ(-1.000000, quad_obj.triangles[0].v2.point.x);
   CLOVE_FLOAT_EQ(-1.000000, quad_obj.triangles[0].v2.point.y);
   CLOVE_FLOAT_EQ(0.000000, quad_obj.triangles[0].v2.point.z);

   CLOVE_FLOAT_EQ(-1.000000, quad_obj.triangles[0].v3.point.x);
   CLOVE_FLOAT_EQ(1.000000, quad_obj.triangles[0].v3.point.y);
   CLOVE_FLOAT_EQ(0.000000, quad_obj.triangles[0].v3.point.z);

   // 2° Face - Points
   CLOVE_FLOAT_EQ(1.000000, quad_obj.triangles[1].v1.point.x);
   CLOVE_FLOAT_EQ(1.000000, quad_obj.triangles[1].v1.point.y);
   CLOVE_FLOAT_EQ(0.000000, quad_obj.triangles[1].v1.point.z);

   CLOVE_FLOAT_EQ(1.000000, quad_obj.triangles[1].v2.point.x);
   CLOVE_FLOAT_EQ(-1.000000, quad_obj.triangles[1].v2.point.y);
   CLOVE_FLOAT_EQ(0.000000, quad_obj.triangles[1].v2.point.z);

   CLOVE_FLOAT_EQ(-1.000000, quad_obj.triangles[1].v3.point.x);
   CLOVE_FLOAT_EQ(1.000000, quad_obj.triangles[1].v3.point.y);
   CLOVE_FLOAT_EQ(0.000000, quad_obj.triangles[1].v3.point.z);

   // 1° Face - Uvs
   CLOVE_FLOAT_EQ(1.000000, quad_obj.triangles[0].v1.uv.x);
   CLOVE_FLOAT_EQ(0.000000, quad_obj.triangles[0].v1.uv.y);

   CLOVE_FLOAT_EQ(0.000000, quad_obj.triangles[0].v2.uv.x);
   CLOVE_FLOAT_EQ(0.000000, quad_obj.triangles[0].v2.uv.y);

   CLOVE_FLOAT_EQ(0.000000, quad_obj.triangles[0].v3.uv.x);
   CLOVE_FLOAT_EQ(1.000000, quad_obj.triangles[0].v3.uv.y);

   // 2° Face - Uvs
   CLOVE_FLOAT_EQ(1.000000, quad_obj.triangles[1].v1.uv.x);
   CLOVE_FLOAT_EQ(1.000000, quad_obj.triangles[1].v1.uv.y);

   CLOVE_FLOAT_EQ(1.000000, quad_obj.triangles[1].v2.uv.x);
   CLOVE_FLOAT_EQ(0.000000, quad_obj.triangles[1].v2.uv.y);

   CLOVE_FLOAT_EQ(0.000000, quad_obj.triangles[1].v3.uv.x);
   CLOVE_FLOAT_EQ(1.000000, quad_obj.triangles[1].v3.uv.y);

   // 1° Face - Normals
   CLOVE_FLOAT_EQ(0.000000, quad_obj.triangles[0].v1.normal.x);
   CLOVE_FLOAT_EQ(0.000000, quad_obj.triangles[0].v1.normal.y);
   CLOVE_FLOAT_EQ(1.000000, quad_obj.triangles[0].v1.normal.z);

   CLOVE_FLOAT_EQ(0.000000, quad_obj.triangles[0].v2.normal.x);
   CLOVE_FLOAT_EQ(0.000000, quad_obj.triangles[0].v2.normal.y);
   CLOVE_FLOAT_EQ(1.000000, quad_obj.triangles[0].v2.normal.z);

   CLOVE_FLOAT_EQ(0.000000, quad_obj.triangles[0].v3.normal.x);
   CLOVE_FLOAT_EQ(0.000000, quad_obj.triangles[0].v3.normal.y);
   CLOVE_FLOAT_EQ(1.000000, quad_obj.triangles[0].v3.normal.z);

   // 2° Face - Normals
   CLOVE_FLOAT_EQ(0.000000, quad_obj.triangles[1].v1.normal.x);
   CLOVE_FLOAT_EQ(0.000000, quad_obj.triangles[1].v1.normal.y);
   CLOVE_FLOAT_EQ(1.000000, quad_obj.triangles[1].v1.normal.z);

   CLOVE_FLOAT_EQ(0.000000, quad_obj.triangles[1].v2.normal.x);
   CLOVE_FLOAT_EQ(0.000000, quad_obj.triangles[1].v2.normal.y);
   CLOVE_FLOAT_EQ(1.000000, quad_obj.triangles[1].v2.normal.z);

   CLOVE_FLOAT_EQ(0.000000, quad_obj.triangles[1].v3.normal.x);
   CLOVE_FLOAT_EQ(0.000000, quad_obj.triangles[1].v3.normal.y);
   CLOVE_FLOAT_EQ(1.000000, quad_obj.triangles[1].v3.normal.z);
}