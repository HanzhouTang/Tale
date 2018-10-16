#pragma once
#include"SimpleLexer.h"
#include "Expr.h"

namespace parser {

	struct SimpleParser {
		parser::SimpleLexer lexer;
		//std::deque<SimpleLexer::LexerNode> lookahead;
		std::wstring content;
		//std::shared_ptr<expr::Expr> parse();
		SimpleParser(const std::wstring& content) : lexer(content) {}
		SimpleParser(std::wstring&& content) :lexer(std::move(content)){}
		//SimpleLexer::LexerNode consume();
		std::shared_ptr<expr::Expr> expr();
		std::shared_ptr<expr::Expr> term();
		std::shared_ptr<expr::Expr> moreterms();
		std::shared_ptr<expr::Expr> factor();
		std::shared_ptr<expr::Expr> morefactors();
		void init();
		static void throwError(const std::vector<SimpleLexer::Token>& expectedToken, SimpleLexer::Token realToken,int lineNumber = __LINE__);
		bool match(SimpleLexer::Token t);
	};
}