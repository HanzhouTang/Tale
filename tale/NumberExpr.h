#pragma once
#include"Expr.h"
struct NumberExpr : Expr {
	float value;
	NumberExpr(float v) :value(v) {
		setType(number);
	}
	virtual std::shared_ptr<Expr> getValue() override {
		return std::shared_ptr<NumberExpr>(this);
	}
};