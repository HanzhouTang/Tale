#pragma once
#include"BinaryOperatorExpr.h"
struct AddExpr :BinaryOperatorExpr {
	AddExpr(const std::shared_ptr<Expr>& runtime, const std::shared_ptr<Expr>& l, const std::shared_ptr<Expr>& r) :
		BinaryOperatorExpr(runtime, l, r) {}
	virtual std::shared_ptr<Expr> getValue() override;
};