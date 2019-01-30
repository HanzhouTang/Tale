#include"DialogLexerTest.h"
using namespace dialogParser;
using namespace std;
TEST_F(DialogLexerTest, SimpleString) {
	wstring content = L"test";
	DialogLexer lexer(content);
	lexer.init();
	auto token = lexer.getNextToken();
	EXPECT_EQ(DialogLexer::DialogToken::STRING, token);
	EXPECT_EQ(L"test", lexer.currentLexeme);
	token = lexer.getNextToken();
	EXPECT_EQ(DialogLexer::DialogToken::EndOfContent, token);
}
TEST_F(DialogLexerTest, ComplexString) {
	wstring content = L"test:\"this is a simple test\"";
	DialogLexer lexer(content);
	lexer.init();
	auto token = lexer.getNextToken();
	EXPECT_EQ(DialogLexer::DialogToken::STRING, token);
	EXPECT_EQ(L"test", lexer.currentLexeme);
	token = lexer.getNextToken();
	EXPECT_EQ(DialogLexer::DialogToken::Colon, token);
	token = lexer.getNextToken();
	EXPECT_EQ(DialogLexer::DialogToken::Quote, token);
	token = lexer.getNextToken();
	EXPECT_EQ(DialogLexer::DialogToken::STRING, token);
	EXPECT_EQ(L"this is a simple test", lexer.currentLexeme);
	token = lexer.getNextToken();
	EXPECT_EQ(DialogLexer::DialogToken::Quote, token);
	token = lexer.getNextToken();
	EXPECT_EQ(DialogLexer::DialogToken::EndOfContent, token);
}
