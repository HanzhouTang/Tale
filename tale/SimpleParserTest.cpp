#include"SimpleParserTest.h"
#include"SimpleParser.h"
#include"NumberExpr.h"
using namespace std;
using namespace expr;
using namespace parser;
TEST_F(SimpleParserTest, AddTwoNumber) {
	wstring content = L"1234+325";
	SimpleParser parser(content);
	parser.init();
	auto expr = parser.expr();
	EXPECT_EQ(Expr::ExprType::TYPE_BINARYOPERATION, expr->getType());
	auto value = std::dynamic_pointer_cast<expr::NumberExpr>(expr->getValue());
	EXPECT_EQ(1559, value->getNumber());
}
TEST_F(SimpleParserTest, ParseAddAndTimes) {
	wstring content = L"123+456*23";
	SimpleParser parser(content);
	parser.init();
	auto expr = parser.expr();
	EXPECT_EQ(Expr::ExprType::TYPE_BINARYOPERATION, expr->getType());
	auto value = std::dynamic_pointer_cast<expr::NumberExpr>(expr->getValue());
	EXPECT_EQ(10611, value->getNumber());
}


TEST_F(SimpleParserTest, ParseAddAndTimes1) {
	wstring content = L"(123+456)*23";
	SimpleParser parser(content);
	parser.init();
	auto expr = parser.expr();
	EXPECT_EQ(Expr::ExprType::TYPE_BINARYOPERATION, expr->getType());
	auto value = std::dynamic_pointer_cast<expr::NumberExpr>(expr->getValue());
	EXPECT_EQ(13317, value->getNumber());
}
