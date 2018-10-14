#include"SimpleParserTest.h"
#include"SimpleParser.h"

using namespace parser;
using namespace std;
TEST_F(SimpleParserTest, ParserNumber) {
	wstring content = L"123.45";
	SimpleParser parser(content);
	parser.init();

}