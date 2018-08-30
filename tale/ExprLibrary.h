#pragma once
#include"Expr.h"
namespace expr {
	std::shared_ptr<Expr> printExpr(const std::vector <std::shared_ptr<Expr>>& args);
	std::shared_ptr<Expr> setOnClickEvent(const std::vector <std::shared_ptr<Expr>>& args);
	std::shared_ptr<Expr> helloWorld(const std::vector <std::shared_ptr<Expr>>& args);
}