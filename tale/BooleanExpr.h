#pragma once
#include"Expr.h"
struct BooleanExpr :Expr {
	bool value;
	BooleanExpr(const std::shared_ptr<Expr>& runtime, bool v) : value(v),Expr(runtime) {
		setType(boolean);
	}
	std::shared_ptr<Expr> getValue() override {
		return shared_from_this();
	}
};
