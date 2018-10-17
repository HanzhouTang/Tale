#include"SimpleParserTest.h"
#include"SimpleParser.h"
#include"NumberExpr.h"
#include"VariableExpr.h"
#include"ClosureExpr.h"
#include"AssignExpr.h"
#include"VariableExpr.h"
#include"StringExpr.h"
#include"ExprLiteral.h"
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

TEST_F(SimpleParserTest, ParseAddAndTimes2) {
	wstring content = L"(123+a)*24+a*10";
	SimpleParser parser(content);
	parser.init();
	auto expr = parser.expr();
	EXPECT_EQ(Expr::ExprType::TYPE_BINARYOPERATION, expr->getType());
	auto closure = ClosureExpr::createClosureExpr();
	auto assign = L"a"_variableExpr << 10_expr;
	closure->addExpression(assign);
	closure->addExpression(expr);
	auto ret = closure->getValue();
	EXPECT_EQ(Expr::ExprType::TYPE_NUMBER, ret->getType());
	auto number = std::dynamic_pointer_cast<NumberExpr>(ret);
	EXPECT_EQ(3292, number->getNumber());
}

TEST_F(SimpleParserTest, ParseAddAndTimes3) {
	wstring content = L"(123-a)*24+a*10";
	SimpleParser parser(content);
	parser.init();
	auto expr = parser.expr();
	EXPECT_EQ(Expr::ExprType::TYPE_BINARYOPERATION, expr->getType());
	auto closure = ClosureExpr::createClosureExpr();
	auto assign = L"a"_variableExpr << 10_expr;
	closure->addExpression(assign);
	closure->addExpression(expr);
	auto ret = closure->getValue();
	EXPECT_EQ(Expr::ExprType::TYPE_NUMBER, ret->getType());
	auto number = std::dynamic_pointer_cast<NumberExpr>(ret);
	EXPECT_EQ(2812, number->getNumber());
}


TEST_F(SimpleParserTest, ParseSubTest) {
	wstring content = L"1+2+3";
	SimpleParser parser(content);
	parser.init();
	auto expr = parser.expr();
	EXPECT_EQ(Expr::ExprType::TYPE_BINARYOPERATION, expr->getType());
	auto value = std::dynamic_pointer_cast<expr::NumberExpr>(expr->getValue());
	EXPECT_EQ(6, value->getNumber());
}

TEST_F(SimpleParserTest, Parse2Times) {
	wstring content = L"1*2*3";
	SimpleParser parser(content);
	parser.init();
	auto expr = parser.expr();
	EXPECT_EQ(Expr::ExprType::TYPE_BINARYOPERATION, expr->getType());
	auto value = std::dynamic_pointer_cast<expr::NumberExpr>(expr->getValue());
	EXPECT_EQ(6, value->getNumber());
}


TEST_F(SimpleParserTest, InvalidTest) {
	wstring content = L"(123)[]";
	SimpleParser parser(content);
	parser.init();
	auto expr = parser.expr();
	auto token = parser.lexer.getNextToken();
	EXPECT_EQ(SimpleLexer::Token::LBrace, token);
}

TEST_F(SimpleParserTest, StringTest) {
	wstring content = L"\"hello world\"";
	SimpleParser parser(content);
	parser.init();
	auto str = parser.str();
	EXPECT_EQ(Expr::ExprType::TYPE_STRING, str->getType());
	auto ret = std::dynamic_pointer_cast<expr::StringExpr>(str);
	EXPECT_EQ(L"hello world", ret->getString());
}

TEST_F(SimpleParserTest, AdvanceStringTest) {
	wstring content = L"\"I can find job in\" +\" US\"";
	SimpleParser parser(content);
	parser.init();
	auto ret = parser.str();
	EXPECT_EQ(Expr::ExprType::TYPE_BINARYOPERATION, ret->getType());
	auto str = std::dynamic_pointer_cast<expr::StringExpr>(ret->getValue());
	EXPECT_EQ(L"I can find job in US", str->getString());
}