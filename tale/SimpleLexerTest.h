#pragma once
#include<gtest/gtest.h>
#include<gmock/gmock.h>
#include"SimpleLexer.h"
using namespace parser;
using namespace std;
struct SimpleLexerTest : testing::Test {

};

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
