#pragma once
#include"SimpleLexer.h"
#include "Expr.h"
namespace parser {
	struct SimpleParser {
		SimpleLexer lexer;
		std::wstring content;
		std::shared_ptr<expr::Expr> parse();
		SimpleParser(const std::wstring& content) : lexer(content) {}
		SimpleParser(std::wstring&& content) :lexer(std::move(content)){}
	};
}