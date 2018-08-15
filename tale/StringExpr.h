#pragma once
#include"Expr.h"
struct StringExpr : Expr {
	std::wstring value;
	StringExpr(const std::shared_ptr<Expr>& runtime,std::wstring v) : value(v),Expr(runtime) {
		setType(string);
	}
	virtual std::shared_ptr<Expr> getValue() override {
		return shared_from_this();
	}
	std::wstring getString() { return value; }
};