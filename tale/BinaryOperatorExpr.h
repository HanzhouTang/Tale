#pragma once
#include"Expr.h"
 struct BinaryOperatorExpr :Expr {
	std::shared_ptr<Expr> left;
	std::shared_ptr<Expr> right;
	virtual std::shared_ptr<Expr> getValue() = 0;
	BinaryOperatorExpr(const std::shared_ptr<Expr>& runtime, const std::shared_ptr<Expr>& l, const std::shared_ptr<Expr>& r) :
		left(l), right(r),Expr(runtime) {}
};