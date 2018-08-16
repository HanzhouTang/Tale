#pragma once
#include"BinaryOperatorExpr.h"
struct AssignExpr :BinaryOperatorExpr {
	AssignExpr(const std::shared_ptr<Expr>& runtime, const std::shared_ptr<Expr>& l, const std::shared_ptr<Expr>& r) :
		BinaryOperatorExpr(runtime,l,r){}
	virtual std::shared_ptr<Expr> getValue() override;
};
