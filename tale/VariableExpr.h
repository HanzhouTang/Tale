#pragma once
#include"Expr.h"
struct VariableExpr :Expr {
	std::wstring name;
	std::shared_ptr<Expr> value;
	VariableExpr(const std::shared_ptr<Expr>& runtime,std::wstring n, std::shared_ptr<Expr> v) :
		Expr(runtime),name(n), value(v) {
		setType(variable);
	}
	virtual std::shared_ptr<Expr> getValue() override {
		return value->getValue();
	}
};