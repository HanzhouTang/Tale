#include"SimpleLexerTest.h"
using namespace parser;
using namespace std;


TEST_F(SimpleLexerTest, LexerNumber) {
	wstring content = L"123.45";
	SimpleLexer lexer(content);
	lexer.init();
	auto token = lexer.getNextToken();
	EXPECT_EQ(SimpleLexer::Token::Number, token);
	EXPECT_EQ(L"123.45", lexer.currentLexeme);
	token = lexer.getNextToken();
	EXPECT_EQ(SimpleLexer::Token::EndofContent, token);
}

TEST_F(SimpleLexerTest, LexerString) {
	wstring content = L"\"Hanzhou said : \\\"this is a string\\\"\"";
	SimpleLexer lexer(content);
	lexer.init();
	auto token = lexer.getNextToken();
	EXPECT_EQ(SimpleLexer::Token::Quote, token);
	EXPECT_EQ(L"\"", lexer.currentLexeme);
	token = lexer.getNextToken();
	EXPECT_EQ(SimpleLexer::Token::String, token);
	EXPECT_EQ(L"Hanzhou said : \"this is a string\"", lexer.currentLexeme);
	token = lexer.getNextToken();
	EXPECT_EQ(SimpleLexer::Token::Quote, token);
	EXPECT_EQ(L"\"", lexer.currentLexeme);
}

TEST_F(SimpleLexerTest, ArithmeticTest) {
	wstring content = L" 1.245 +67*76 < 110   ";
	SimpleLexer lexer(content);
	lexer.init();
	auto token = lexer.getNextToken();
	EXPECT_EQ(SimpleLexer::Token::Number, token);
	EXPECT_EQ(L"1.245", lexer.currentLexeme);
	token = lexer.getNextToken();
	EXPECT_EQ(SimpleLexer::Token::Add, token);
	EXPECT_EQ(L"+", lexer.currentLexeme);
	token = lexer.getNextToken();
	EXPECT_EQ(SimpleLexer::Token::Number, token);
	EXPECT_EQ(L"67", lexer.currentLexeme);
	token = lexer.getNextToken();
	EXPECT_EQ(SimpleLexer::Token::Times, token);
	EXPECT_EQ(L"*", lexer.currentLexeme);
	token = lexer.getNextToken();
	EXPECT_EQ(SimpleLexer::Token::Number, token);
	EXPECT_EQ(L"76", lexer.currentLexeme);
	token = lexer.getNextToken();
	EXPECT_EQ(SimpleLexer::Token::Less, token);
	EXPECT_EQ(L"<", lexer.currentLexeme);
	token = lexer.getNextToken();
	EXPECT_EQ(SimpleLexer::Token::Number, token);
	EXPECT_EQ(L"110", lexer.currentLexeme);
	token = lexer.getNextToken();
	EXPECT_EQ(SimpleLexer::Token::EndofContent, token);
}

TEST_F(SimpleLexerTest, ParenthesesTest) {
	wstring content = L"(123)";
	SimpleLexer lexer(content);
	lexer.init();
	auto token = lexer.getNextToken();
	EXPECT_EQ(SimpleLexer::Token::LParen, token);
	EXPECT_EQ(L"(", lexer.currentLexeme);
	token = lexer.getNextToken();
	EXPECT_EQ(SimpleLexer::Token::Number, token);
	EXPECT_EQ(L"123", lexer.currentLexeme);
	token = lexer.getNextToken();
	EXPECT_EQ(SimpleLexer::Token::RParen, token);
	EXPECT_EQ(L")", lexer.currentLexeme);
	token = lexer.getNextToken();
	EXPECT_EQ(SimpleLexer::Token::EndofContent, token);
	lexer.init();
}

TEST_F(SimpleLexerTest, EqlEqlTest) {
	wstring content = L"a==b";
	SimpleLexer lexer(content);
	lexer.init();
	auto token = lexer.getNextToken();
	EXPECT_EQ(SimpleLexer::Token::Variable, token);
	EXPECT_EQ(L"a", lexer.currentLexeme);
	token = lexer.getNextToken();
	EXPECT_EQ(SimpleLexer::Token::EqlEql, token);
	EXPECT_EQ(L"==", lexer.currentLexeme);
	token = lexer.getNextToken();
	EXPECT_EQ(SimpleLexer::Token::Variable, token);
	EXPECT_EQ(L"b", lexer.currentLexeme);
	token = lexer.getNextToken();
	EXPECT_EQ(SimpleLexer::Token::EndofContent, token);
	lexer.init();
}

TEST_F(SimpleLexerTest, EqlTestNew) {
	wstring content = L"a=b";
	SimpleLexer lexer(content);
	lexer.init();
	auto token = lexer.getNextToken();
	EXPECT_EQ(SimpleLexer::Token::Variable, token);
	EXPECT_EQ(L"a", lexer.currentLexeme);
	token = lexer.getNextToken();
	EXPECT_EQ(SimpleLexer::Token::Eql, token);
	EXPECT_EQ(L"=", lexer.currentLexeme);
	token = lexer.getNextToken();
	EXPECT_EQ(SimpleLexer::Token::Variable, token);
	EXPECT_EQ(L"b", lexer.currentLexeme);
	token = lexer.getNextToken();
	EXPECT_EQ(SimpleLexer::Token::EndofContent, token);
	lexer.init();
}

TEST_F(SimpleLexerTest, TrueTest) {
	wstring content = L"a true";
	SimpleLexer lexer(content);
	lexer.init();
	auto token = lexer.getNextToken();
	EXPECT_EQ(SimpleLexer::Token::Variable, token);
	EXPECT_EQ(L"a", lexer.currentLexeme);
	token = lexer.getNextToken();
	EXPECT_EQ(SimpleLexer::Token::True, token);
	EXPECT_EQ(L"true", lexer.currentLexeme);
	token = lexer.getNextToken();
	EXPECT_EQ(SimpleLexer::Token::EndofContent, token);
	lexer.init();
}

TEST_F(SimpleLexerTest, FalseTest) {
	wstring content = L"a false";
	SimpleLexer lexer(content);
	lexer.init();
	auto token = lexer.getNextToken();
	EXPECT_EQ(SimpleLexer::Token::Variable, token);
	EXPECT_EQ(L"a", lexer.currentLexeme);
	token = lexer.getNextToken();
	EXPECT_EQ(SimpleLexer::Token::False, token);
	EXPECT_EQ(L"false", lexer.currentLexeme);
	token = lexer.getNextToken();
	EXPECT_EQ(SimpleLexer::Token::EndofContent, token);
	lexer.init();
}

TEST_F(SimpleLexerTest, MinusNumberTest) {
	wstring content = L"-123";
	SimpleLexer lexer(content);
	lexer.init();
	auto token = lexer.getNextToken();
	EXPECT_EQ(SimpleLexer::Token::Minus, token);
	EXPECT_EQ(L"-", lexer.currentLexeme);
	token = lexer.getNextToken();
	EXPECT_EQ(SimpleLexer::Token::Number, token);
	EXPECT_EQ(L"123", lexer.currentLexeme);
	token = lexer.getNextToken();
	EXPECT_EQ(SimpleLexer::Token::EndofContent, token);
	lexer.init();
}


TEST_F(SimpleLexerTest, TrueFalseTest) {
	wstring content = L"true false";
	SimpleLexer lexer(content);
	lexer.init();
	auto token = lexer.getNextToken();
	EXPECT_EQ(SimpleLexer::Token::True, token);
	EXPECT_EQ(L"true", lexer.currentLexeme);
	token = lexer.getNextToken();
	EXPECT_EQ(SimpleLexer::Token::False, token);
	EXPECT_EQ(L"false", lexer.currentLexeme);
	token = lexer.getNextToken();
	EXPECT_EQ(SimpleLexer::Token::EndofContent, token);
	lexer.init();
}

