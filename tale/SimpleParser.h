#pragma once
#include"SimpleLexer.h"
#include "Expr.h"
#include"AddExpr.h"
#include"VariableExpr.h"
#include"ArithmeticExpr.h"
#include"NumberExpr.h"
#include<deque>
namespace parser {
	struct SimpleParser {
		SimpleLexer lexer;
		//std::deque<SimpleLexer::LexerNode> lookahead;
		std::wstring content;
		std::shared_ptr<expr::Expr> parse();
		SimpleParser(const std::wstring& content) : lexer(content) {}
		SimpleParser(std::wstring&& content) :lexer(std::move(content)){}
		//SimpleLexer::LexerNode consume();
		std::shared_ptr<expr::Expr> expr();
		std::shared_ptr<expr::AddExpr> add();
		std::shared_ptr<expr::ArithmeticExpr> div();
		std::shared_ptr<expr::ArithmeticExpr> minus();
		std::shared_ptr<expr::ArithmeticExpr> times();
		std::shared_ptr<expr::VariableExpr> id();
		std::shared_ptr<expr::NumberExpr> number();
		std::shared_ptr<expr::Expr> parentheses();
		void init();
		static void throwError(SimpleLexer::Token expectedToken, SimpleLexer::Token realToken);
		bool match(SimpleLexer::Token t);
	};
}