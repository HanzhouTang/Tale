#pragma once
#include"Expr.h"
struct NumberExpr : Expr {
	float value;
	NumberExpr(const std::shared_ptr<Expr>& runtime,float v) :value(v),Expr(runtime) {
		setType(number);
	}
	virtual std::shared_ptr<Expr> getValue() override {
		return shared_from_this();
	}
	float getNumber() { return value; }
};