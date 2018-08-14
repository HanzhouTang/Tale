#pragma once
#include"Expr.h"
struct BooleanExpr :Expr {
	bool value;
	BooleanExpr(bool v) : value(v) {
		setType(boolean);
	}
	std::shared_ptr<Expr> getValue() override {
		return std::shared_ptr<BooleanExpr>(this);
	}
};
