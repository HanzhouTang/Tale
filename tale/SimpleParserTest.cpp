#include"SimpleParserTest.h"
#include"SimpleParser.h"
#include"NumberExpr.h"
#include"VariableExpr.h"
#include"ClosureExpr.h"
#include"AssignExpr.h"
#include"VariableExpr.h"
#include"StringExpr.h"
#include"ExprLiteral.h"
#include"FunctionExpr.h"
#include"BooleanExpr.h"
#include"CallExpr.h"
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

TEST_F(SimpleParserTest, NumberTest) {
	wstring content = L"-123";
	SimpleParser parser(content);
	parser.init();
	auto expr = parser.expr();
	EXPECT_EQ(Expr::ExprType::TYPE_UNARY_OPERATOR, expr->getType());
	auto value = std::dynamic_pointer_cast<expr::NumberExpr>(expr->getValue());
	EXPECT_EQ(-123, value->getNumber());
}

TEST_F(SimpleParserTest, NumberTest1) {
	wstring content = L"-123 - 12345";
	SimpleParser parser(content);
	parser.init();
	auto expr = parser.expr();
	EXPECT_EQ(Expr::ExprType::TYPE_BINARYOPERATION, expr->getType());
	auto value = std::dynamic_pointer_cast<expr::NumberExpr>(expr->getValue());
	EXPECT_EQ(-12468, value->getNumber());
}

TEST_F(SimpleParserTest, NumberTest2) {
	wstring content = L"-123 - -12345";
	SimpleParser parser(content);
	parser.init();
	auto expr = parser.expr();
	EXPECT_EQ(Expr::ExprType::TYPE_BINARYOPERATION, expr->getType());
	auto value = std::dynamic_pointer_cast<expr::NumberExpr>(expr->getValue());
	EXPECT_EQ(12222, value->getNumber());
}

TEST_F(SimpleParserTest, NumberTest3) {
	wstring content = L"-123--12345";
	SimpleParser parser(content);
	parser.init();
	auto expr = parser.expr();
	EXPECT_EQ(Expr::ExprType::TYPE_BINARYOPERATION, expr->getType());
	auto value = std::dynamic_pointer_cast<expr::NumberExpr>(expr->getValue());
	EXPECT_EQ(12222, value->getNumber());
}


TEST_F(SimpleParserTest, ParseAddAndTimes) {
	using namespace std;
	wstring content = L"123+456*23";
	SimpleParser parser(content);
	parser.init();
	auto expr = parser.expr();
	//wcout << expr->toString() << endl;
	EXPECT_EQ(Expr::ExprType::TYPE_BINARYOPERATION, expr->getType());
	auto value = std::dynamic_pointer_cast<expr::NumberExpr>(expr->getValue());
	EXPECT_EQ(10611, value->getNumber());
}

TEST_F(SimpleParserTest, ParenthesesTest) {
	wstring content = L"(123+456)";
	SimpleParser parser(content);
	parser.init();
	auto expr = parser.expr();
	EXPECT_EQ(Expr::ExprType::TYPE_BINARYOPERATION, expr->getType());
	auto value = std::dynamic_pointer_cast<expr::NumberExpr>(expr->getValue());
	EXPECT_EQ(579, value->getNumber());
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
	using namespace std;
	wstring content = L"1-2-3-4";
	SimpleParser parser(content);
	parser.init();
	auto expr = parser.expr();
	EXPECT_EQ(Expr::ExprType::TYPE_BINARYOPERATION, expr->getType());
	auto value = std::dynamic_pointer_cast<expr::NumberExpr>(expr->getValue());
	EXPECT_EQ(-8, value->getNumber());
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
	using namespace std;
	wstring content = L"(123+567)[]";
	SimpleParser parser(content);
	parser.init();
	auto expr = parser.expr();
	auto token = parser.lexer.getNextToken();
	EXPECT_EQ(SimpleLexer::Token::LBrace, token);
}



TEST_F(SimpleParserTest, InvalidTest1) {
	using namespace std;
	wstring content = L"[]";
	SimpleParser parser(content);
	parser.init();
	auto expr = parser.expr();
	EXPECT_EQ(expr::Expr::TYPE_NULL, expr->getType());
	auto token = parser.lexer.getNextToken();
	EXPECT_EQ(SimpleLexer::Token::LBrace, token);
}


TEST_F(SimpleParserTest, InvalidTest2) {
	using namespace std;
	wstring content = L"[]";
	SimpleParser parser(content);
	parser.init();
	auto str = parser.str();
	EXPECT_EQ(expr::Expr::TYPE_NULL, str->getType());
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

TEST_F(SimpleParserTest, ElementTest) {
	wstring content = L"\"hello world\"";
	SimpleParser parser(content);
	parser.init();
	auto str = parser.element();
	EXPECT_EQ(Expr::ExprType::TYPE_STRING, str->getType());
	auto ret = std::dynamic_pointer_cast<expr::StringExpr>(str);
	EXPECT_EQ(L"hello world", ret->getString());
}

TEST_F(SimpleParserTest, ElementTes1) {
	wstring content = L"(123+456)*23";
	SimpleParser parser(content);
	parser.init();
	auto expr = parser.element();
	EXPECT_EQ(Expr::ExprType::TYPE_BINARYOPERATION, expr->getType());
	auto value = std::dynamic_pointer_cast<expr::NumberExpr>(expr->getValue());
	EXPECT_EQ(13317, value->getNumber());
}

TEST_F(SimpleParserTest, ElementTes2) {
	wstring content = L"\"hello world\" 1234";
	SimpleParser parser(content);
	parser.init();
	auto str = parser.element();
	EXPECT_EQ(Expr::ExprType::TYPE_STRING, str->getType());
	auto ret = std::dynamic_pointer_cast<expr::StringExpr>(str);
	EXPECT_EQ(L"hello world", ret->getString());
	auto number = parser.element();
	EXPECT_EQ(Expr::ExprType::TYPE_NUMBER, number->getType());
	auto value = std::dynamic_pointer_cast<expr::NumberExpr>(number->getValue());
	EXPECT_EQ(1234, value->getNumber());
}


TEST_F(SimpleParserTest, InvalidTest_AddStrAndNumber) {
	wstring content = L"\"hello world\"+1234";
	SimpleParser parser(content);
	parser.init();
	auto first = parser.element();
	EXPECT_EQ(Expr::ExprType::TYPE_STRING, first->getType());
	auto second = parser.element();
	EXPECT_EQ(Expr::ExprType::TYPE_NULL, second->getType());
}

TEST_F(SimpleParserTest, InvalidTest_AddNumberAndStr) {
	wstring content = L"1234+ \"hello world\"";
	SimpleParser parser(content);
	parser.init();
	auto first = parser.element();
	EXPECT_EQ(Expr::ExprType::TYPE_NUMBER, first->getType());
	auto second = parser.element();
	EXPECT_EQ(Expr::ExprType::TYPE_NULL, second->getType());
}


TEST_F(SimpleParserTest, TestStrAddNumber) {
	wstring content = L"1234+ 123 + \"hello world\"";
	SimpleParser parser(content);
	parser.init();
	auto first = parser.element();
	EXPECT_EQ(Expr::ExprType::TYPE_BINARYOPERATION, first->getType());
	auto result = std::dynamic_pointer_cast<expr::NumberExpr>(first->getValue());
	EXPECT_EQ(1357, result->getNumber());
	auto second = parser.element();
	EXPECT_EQ(Expr::ExprType::TYPE_NULL, second->getType());
}


TEST_F(SimpleParserTest, AssignTest) {
	wstring content = L"a = 1234";
	SimpleParser parser(content);
	parser.init();
	auto assign = parser.assign();
	EXPECT_EQ(Expr::ExprType::TYPE_BINARYOPERATION, assign->getType());
	//wcout << assign->toString() << endl;
}

TEST_F(SimpleParserTest, AssignTest1) {
	wstring content = L"a = 1234 +1234+ 40";
	SimpleParser parser(content);
	parser.init();
	auto assign = parser.assign();
	EXPECT_EQ(Expr::ExprType::TYPE_BINARYOPERATION, assign->getType());
	//wcout << assign->toString() << endl;
}

TEST_F(SimpleParserTest, AssignTest2) {
	wstring content = L"a = \"fda\"";
	SimpleParser parser(content);
	parser.init();
	auto assign = parser.assign();
	EXPECT_EQ(Expr::ExprType::TYPE_BINARYOPERATION, assign->getType());
	//wcout << assign->toString() << endl;
}

TEST_F(SimpleParserTest, MapTest) {
	wstring content = L"[\"hello\":\"world\",]";
	SimpleParser parser(content);
	parser.init();
	auto map = parser.map();
	
	EXPECT_EQ(Expr::ExprType::TYPE_MAP, map->getType());
	auto MAP = std::dynamic_pointer_cast<MapExpr>(map);
	auto x = MAP->get(L"hello");
	EXPECT_EQ(Expr::ExprType::TYPE_STRING, x->getType());
	auto str = std::dynamic_pointer_cast<StringExpr>(x);
	EXPECT_EQ(L"world" , str->getString());
}

TEST_F(SimpleParserTest, EmptyMapTest) {
	wstring content = L"[]";
	SimpleParser parser(content);
	parser.init();
	auto map = parser.map();
	EXPECT_EQ(Expr::ExprType::TYPE_MAP, map->getType());
}

// after this send my resume to lyft
TEST_F(SimpleParserTest, AdvanceMapTest_InnerMap) {
	wstring content = L"[\"hello\":[\"beautiful\":\"life\"],\"math\": 1+2+ 3 ]";
	SimpleParser parser(content);
	parser.init();
	auto map = parser.map();
	EXPECT_EQ(Expr::ExprType::TYPE_MAP, map->getType());
	auto MAP = std::dynamic_pointer_cast<MapExpr>(map);
	auto x = MAP->get(L"math");
	EXPECT_EQ(Expr::ExprType::TYPE_BINARYOPERATION, x->getType());
	auto number = std::dynamic_pointer_cast<NumberExpr>(x->getValue());
	EXPECT_EQ(6,number->getNumber());
	auto y = MAP->get(L"hello");
	EXPECT_EQ(Expr::ExprType::TYPE_MAP, y->getType());
	auto MAPY = std::dynamic_pointer_cast<MapExpr>(y);
	auto life = MAPY->get(L"beautiful");
	EXPECT_EQ(Expr::ExprType::TYPE_STRING, life->getType());
	EXPECT_EQ(L"life", std::dynamic_pointer_cast<expr::StringExpr>(life)->getString());
}

TEST_F(SimpleParserTest, StatesTest) {
	wstring content = L"[\"hello\":[\"beautiful\":\"life\"],\"math\": 1+2+ 3 ]";
	SimpleParser parser(content);
	parser.init();
	auto map = parser.states()[0];
	EXPECT_EQ(Expr::ExprType::TYPE_MAP, map->getType());
	auto MAP = std::dynamic_pointer_cast<MapExpr>(map);
	auto x = MAP->get(L"math");
	EXPECT_EQ(Expr::ExprType::TYPE_BINARYOPERATION, x->getType());
	auto number = std::dynamic_pointer_cast<NumberExpr>(x->getValue());
	EXPECT_EQ(6, number->getNumber());
	auto y = MAP->get(L"hello");
	EXPECT_EQ(Expr::ExprType::TYPE_MAP, y->getType());
	auto MAPY = std::dynamic_pointer_cast<MapExpr>(y);
	auto life = MAPY->get(L"beautiful");
	EXPECT_EQ(Expr::ExprType::TYPE_STRING, life->getType());
	EXPECT_EQ(L"life", std::dynamic_pointer_cast<expr::StringExpr>(life)->getString());

}

TEST_F(SimpleParserTest, StatesTest1) {
	wstring content = L"[\"hello\":[\"beautiful\":\"life\"],\"math\": 1+2+ 3 ]1+2+3";
	SimpleParser parser(content);
	parser.init();
	auto states = parser.states();
	EXPECT_EQ(0, states.size());
}

TEST_F(SimpleParserTest, StatesTest2) {
	wstring content = L"[\"hello\":[\"beautiful\":\"life\"],\"math\": 1+2+ 3 ];1+2+3";
	SimpleParser parser(content);
	parser.init();
	auto states = parser.states();
	EXPECT_EQ(2, states.size());
}
TEST_F(SimpleParserTest, StatesTest3) {
	wstring content = L"[\"hello\":[\"beautiful\":\"life\"],\"math\": 1+2+ 3 ];1+2+3[]";
	SimpleParser parser(content);
	parser.init();
	auto states = parser.states();
	EXPECT_EQ(1, states.size());
}

TEST_F(SimpleParserTest, ClosureTest) {
	wstring content = L"{1+2+3;}";
	SimpleParser parser(content);
	parser.init();
	auto closure = parser.element();
	EXPECT_EQ(expr::Expr::TYPE_CLOSURE, closure->getType());
	auto ret = closure->getValue();
	EXPECT_EQ(expr::Expr::TYPE_NUMBER, ret->getType());
	auto number = std::dynamic_pointer_cast<expr::NumberExpr>(ret);
	EXPECT_EQ(6, number->getNumber());
}

TEST_F(SimpleParserTest, ClosureTest1) {
	wstring content = L"{a=6;a+3;}";
	SimpleParser parser(content);
	parser.init();
	auto closure = parser.element();
	EXPECT_EQ(expr::Expr::TYPE_CLOSURE, closure->getType());
	auto ret = closure->getValue();
	EXPECT_EQ(expr::Expr::TYPE_NUMBER, ret->getType());
	auto number = std::dynamic_pointer_cast<expr::NumberExpr>(ret);
	EXPECT_EQ(9, number->getNumber());
}

TEST_F(SimpleParserTest, ClosureTest2) {
	wstring content = L"{b={a=6;{a+3;};};}";
	SimpleParser parser(content);
	parser.init();
	auto closure = parser.element();
	//wcout << closure << endl;
	EXPECT_EQ(expr::Expr::TYPE_CLOSURE, closure->getType());
	auto ret = closure->getValue();
	EXPECT_EQ(expr::Expr::TYPE_CLOSURE, ret->getType());
	auto number = std::dynamic_pointer_cast<expr::NumberExpr>(ret->getValue());
	EXPECT_EQ(9, number->getNumber());
}



TEST_F(SimpleParserTest, FunctionTest) {
	wstring content = L"{def add(a,b){\"\" + a +b +\"!\" ;};}";
	SimpleParser parser(content);
	parser.init();
	auto closure = parser.element();
	//wcout << closure << endl;
	EXPECT_EQ(expr::Expr::TYPE_CLOSURE, closure->getType());
	auto function = closure->getValue();
	EXPECT_EQ(expr::Expr::TYPE_FUNCTION, function->getType());
	auto f = std::dynamic_pointer_cast<expr::FunctionExpr>(function);
	auto result = f->getValue({ L"hello"_expr,L" world"_expr });
}


TEST_F(SimpleParserTest, FunctionTest1) {
	wstring content = L"{def add(a,b){a+b;};}";
	SimpleParser parser(content);
	parser.init();
	auto closure = parser.closure();
	EXPECT_EQ(expr::Expr::TYPE_CLOSURE, closure->getType());
	auto function = closure->getValue();
	EXPECT_EQ(expr::Expr::TYPE_FUNCTION, function->getType());
	auto f = std::dynamic_pointer_cast<expr::FunctionExpr>(function);
	auto result = f->getValue({ 1_expr,2_expr });
	EXPECT_EQ(expr::Expr::TYPE_NUMBER, result->getType());
	EXPECT_EQ(3, std::dynamic_pointer_cast<expr::NumberExpr>(result)->getNumber());
}

TEST_F(SimpleParserTest, CallTest) {
	wstring content = L"add(1,2)";
	SimpleParser parser(content);
	parser.init();
	auto call = parser.callable();
	EXPECT_EQ(expr::Expr::TYPE_CALL, call->getType());
	auto callable = std::dynamic_pointer_cast<expr::CallExpr>(call)->callable;
	EXPECT_EQ(expr::Expr::TYPE_VARIABLE, callable->getType());
	auto variableName = std::dynamic_pointer_cast<expr::VariableExpr>(callable)->getName();
	EXPECT_EQ(L"add", variableName);
}

TEST_F(SimpleParserTest, BASICTEST) {
	wstring content = L"a";
	SimpleParser parser(content);
	parser.init();
	auto result = parser.element();
	//wcout << result->toString() << endl;
}


TEST_F(SimpleParserTest, CallTest1) {
	wstring content = L"def (a,b){a+b;}(1,2)";
	SimpleParser parser(content);
	parser.init();
	auto call = parser.callable();
	EXPECT_EQ(expr::Expr::TYPE_CALL, call->getType());
	auto callable = std::dynamic_pointer_cast<expr::CallExpr>(call)->callable;
	EXPECT_EQ(expr::Expr::TYPE_FUNCTION, callable->getType());
	auto result = call->getValue();
	EXPECT_EQ(expr::Expr::TYPE_NUMBER, result->getType());
	auto answer = std::dynamic_pointer_cast<expr::NumberExpr>(result)->getNumber();
	EXPECT_EQ(3, answer);

}

TEST_F(SimpleParserTest, CallTest2) {
	wstring content = L"def (a,b){a+b;}(1,2)";
	SimpleParser parser(content);
	parser.init();
	auto call = parser.element();
	//wcout << call->toString() << endl;
	EXPECT_EQ(expr::Expr::TYPE_CALL, call->getType());
	auto callable = std::dynamic_pointer_cast<expr::CallExpr>(call)->callable;
	EXPECT_EQ(expr::Expr::TYPE_FUNCTION, callable->getType());
	auto result = call->getValue();
	EXPECT_EQ(expr::Expr::TYPE_NUMBER, result->getType());
	auto answer = std::dynamic_pointer_cast<expr::NumberExpr>(result)->getNumber();
	EXPECT_EQ(3, answer);
}

TEST_F(SimpleParserTest, CallTest3) {
	wstring content = L"def (){}()";
	SimpleParser parser(content);
	parser.init();
	auto call = parser.element();
	EXPECT_EQ(expr::Expr::TYPE_CALL, call->getType());
	auto callable = std::dynamic_pointer_cast<expr::CallExpr>(call)->callable;
	EXPECT_EQ(expr::Expr::TYPE_FUNCTION, callable->getType());
}

TEST_F(SimpleParserTest, CallTest4) {
	wstring content = L"def (a){}(1)";
	SimpleParser parser(content);
	parser.init();
	auto call = parser.element();
	EXPECT_EQ(expr::Expr::TYPE_CALL, call->getType());
	auto callable = std::dynamic_pointer_cast<expr::CallExpr>(call)->callable;
	EXPECT_EQ(expr::Expr::TYPE_FUNCTION, callable->getType());
}

TEST_F(SimpleParserTest, CallTest5) {
	wstring content = L"def (){}()()";
	SimpleParser parser(content);
	parser.init();
	auto call = parser.element();
	EXPECT_EQ(expr::Expr::TYPE_CALL, call->getType());
	auto callable = std::dynamic_pointer_cast<expr::CallExpr>(call)->callable;
	EXPECT_EQ(expr::Expr::TYPE_CALL, callable->getType());
}

TEST_F(SimpleParserTest, CallTest6) {
	wstring content = L"-def (a,b){a+b;}(1,2)";
	SimpleParser parser(content);
	parser.init();
	auto call = parser.element();
	//wcout << call->toString() << endl;
	EXPECT_EQ(expr::Expr::TYPE_UNARY_OPERATOR, call->getType());
	auto result = call->getValue();
	EXPECT_EQ(expr::Expr::TYPE_NUMBER, result->getType());
	auto answer = std::dynamic_pointer_cast<expr::NumberExpr>(result)->getNumber();
	EXPECT_EQ(-3, answer);
}

TEST_F(SimpleParserTest, CallTest7) {
	wstring content = L"-def (a,b){a+b;}(1,2)*def (a){a+3;}(3)";
	SimpleParser parser(content);
	parser.init();
	auto call = parser.element();
	//wcout << call->toString() << endl;
	EXPECT_EQ(expr::Expr::TYPE_BINARYOPERATION, call->getType());
	auto result = call->getValue();
	EXPECT_EQ(expr::Expr::TYPE_NUMBER, result->getType());
	auto answer = std::dynamic_pointer_cast<expr::NumberExpr>(result)->getNumber();
	EXPECT_EQ(-18, answer);
}


// f(1==2) it's possible 
// thus compare should be element

TEST_F(SimpleParserTest, CompareTest) {
	wstring content = L"123==234";
	SimpleParser parser(content);
	parser.init();
	auto call = parser.compare();
	EXPECT_EQ(expr::Expr::TYPE_BINARYOPERATION, call->getType());
	auto result = call->getValue();
	EXPECT_EQ(expr::Expr::TYPE_BOOLEAN, result->getType());
	auto answer = std::dynamic_pointer_cast<expr::BooleanExpr>(result)->getBoolValue();
	EXPECT_EQ(false, answer);
}

TEST_F(SimpleParserTest, CompareInvalidTest) {
	wstring content = L"123";
	SimpleParser parser(content);
	parser.init();
	auto call = parser.compare();
	EXPECT_EQ(expr::Expr::TYPE_NULL, call->getType());
}


TEST_F(SimpleParserTest, BooleanTest) {
	wstring content = L"123==123";
	SimpleParser parser(content);
	parser.init();
	auto call = parser.boolean();
	EXPECT_EQ(expr::Expr::TYPE_BINARYOPERATION, call->getType());
	auto result = call->getValue();
	EXPECT_EQ(expr::Expr::TYPE_BOOLEAN, result->getType());
	auto answer = std::dynamic_pointer_cast<expr::BooleanExpr>(result)->getBoolValue();
	EXPECT_EQ(true, answer);
}

TEST_F(SimpleParserTest, BooleanTest1) {
	wstring content = L"123<123";
	SimpleParser parser(content);
	parser.init();
	auto call = parser.boolean();
	EXPECT_EQ(expr::Expr::TYPE_BINARYOPERATION, call->getType());
	auto result = call->getValue();
	EXPECT_EQ(expr::Expr::TYPE_BOOLEAN, result->getType());
	auto answer = std::dynamic_pointer_cast<expr::BooleanExpr>(result)->getBoolValue();
	EXPECT_EQ(false, answer);
}

TEST_F(SimpleParserTest, BooleanTest2) {
	wstring content = L"123<123 or true";
	SimpleParser parser(content);
	parser.init();
	auto call = parser.boolean();
	wcout << call->toString() << endl;
	EXPECT_EQ(expr::Expr::TYPE_BINARYOPERATION, call->getType());
	auto result = call->getValue();
	EXPECT_EQ(expr::Expr::TYPE_BOOLEAN, result->getType());
	auto answer = std::dynamic_pointer_cast<expr::BooleanExpr>(result)->getBoolValue();
	EXPECT_EQ(true, answer);
}

TEST_F(SimpleParserTest, BooleanTest3) {
	wstring content = L"123<123 or not 123>456";
	SimpleParser parser(content);
	parser.init();
	auto call = parser.boolean();
	wcout << endl<<call->toString() << endl;
	EXPECT_EQ(expr::Expr::TYPE_BINARYOPERATION, call->getType());
	auto result = call->getValue();
	EXPECT_EQ(expr::Expr::TYPE_BOOLEAN, result->getType());
	auto answer = std::dynamic_pointer_cast<expr::BooleanExpr>(result)->getBoolValue();
	EXPECT_EQ(true, answer);
}

// support number and string
// assign and str should belong to element
// at least, str should belong to element