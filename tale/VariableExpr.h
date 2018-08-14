#pragma once
#include"Expr.h"
struct VariableExpr :Expr {
	std::wstring name;
	std::shared_ptr<Expr> value;
	VariableExpr(std::wstring n, std::shared_ptr<Expr> v) :
		name(n), value(v) {
		setType(variable);
	}
	virtual std::shared_ptr<Expr> getValue() override {
		return value->getValue();
	}
};