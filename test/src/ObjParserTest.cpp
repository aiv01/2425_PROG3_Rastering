#define CLOVE_SUITE_NAME ObjParserTest
#include "clove-unit.h"
#include "ObjParser.h"

CLOVE_TEST(TokenizerWithFourTokens) {

   std::string testString{"v -1.000000 1.000000 0.000000"};

   auto tokens = Tokenizer::Split(testString, ' ');

   CLOVE_SIZET_EQ(4, tokens.size());
   CLOVE_STRING_EQ(tokens[0].c_str(), "v");
   CLOVE_STRING_EQ(tokens[1].c_str(), "-1.000000");
   CLOVE_STRING_EQ(tokens[2].c_str(), "1.000000");
   CLOVE_STRING_EQ(tokens[3].c_str(), "0.000000");

   CLOVE_PASS();
}
