#pragma once
#include"Expr.h"
struct BinaryOperatorExpr :Expr {
	std::shared_ptr<Expr> left;
	std::shared_ptr<Expr> right;
	virtual std::shared_ptr<Expr> operate(std::shared_ptr<Expr> l, std::shared_ptr<Expr> r) { return std::make_shared<Expr>(shared_from_this()); }
	virtual std::shared_ptr<Expr> getValue() {
		auto l = left->getValue();
		auto r = right->getValue();
		operate(l, r);
	}
	BinaryOperatorExpr(const std::shared_ptr<Expr>& runtime, const std::shared_ptr<Expr>& l, const std::shared_ptr<Expr>& r) :
		left(l), right(r),Expr(runtime) {}
};