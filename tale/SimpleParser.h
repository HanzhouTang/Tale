#pragma once
#include"SimpleLexer.h"
#include "Expr.h"
#include<deque>
namespace parser {

	struct SimpleParser {
		parser::SimpleLexer lexer;
		//std::deque<SimpleLexer::LexerNode> lookahead;
		std::wstring content;
		//std::shared_ptr<expr::Expr> parse();
		SimpleParser(const std::wstring& content) : lexer(content) {}
		SimpleParser(std::wstring&& content) :lexer(std::move(content)){}
		//SimpleLexer::LexerNode consume();
		//===============for math expression =====================
		std::shared_ptr<expr::Expr> expr();
		void term(std::deque<std::shared_ptr<expr::Expr>>&);
		void moreterms(std::deque<std::shared_ptr<expr::Expr>>&);
		void factor(std::deque<std::shared_ptr<expr::Expr>>&);
		void morefactors(std::deque<std::shared_ptr<expr::Expr>>&);
		//===============for string =============================
		std::shared_ptr<expr::Expr> str();
		std::shared_ptr<expr::Expr> substr();
		std::shared_ptr<expr::Expr> moresubstrs();

		void init();
		static void throwError(const std::vector<SimpleLexer::Token>& expectedToken, SimpleLexer::Token realToken,int lineNumber = __LINE__);
		bool match(SimpleLexer::Token t);
	};
}