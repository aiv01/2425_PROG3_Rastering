#define CLOVE_SUITE_NAME ObjParserTest
#include "clove-unit.h"
#include "ObjParser.h"

CLOVE_TEST(Tokenizer4Token) {
   auto tokens = Tokenizer::Split("v -1.000000 1.000000 0.000000",' ');
   CLOVE_SIZET_EQ(4, tokens.size());
   CLOVE_STRING_EQ("v", tokens[0].c_str());
   CLOVE_STRING_EQ("-1.000000", tokens[1].c_str());
   CLOVE_STRING_EQ("1.000000", tokens[2].c_str());
   CLOVE_STRING_EQ("0.000000", tokens[3].c_str());
}
