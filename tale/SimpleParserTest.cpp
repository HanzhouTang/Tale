#include"SimpleParserTest.h"
#include"SimpleParser.h"

using namespace parser;
using namespace std;
using namespace expr;
TEST_F(SimpleParserTest, ParserNumber) {
	wstring content = L"123.45";
	SimpleParser parser(content);
	parser.init();
	auto expr = parser.expr();
	EXPECT_EQ(Expr::TYPE_NUMBER, expr->getType());
	//std::wcout << expr->toString() << std::endl;
}

TEST_F(SimpleParserTest, ParserID) {
	wstring content = L"abcd";
	SimpleParser parser(content);
	parser.init();
	auto expr = parser.expr();
	EXPECT_EQ(Expr::TYPE_VARIABLE, expr->getType());
	//std::wcout << expr->toString() << std::endl;
}