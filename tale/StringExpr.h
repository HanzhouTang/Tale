#pragma once
#include"Expr.h"
struct StringExpr : Expr {
	std::wstring value;
	StringExpr(std::wstring v) : value(v) {
		setType(string);
	}
	virtual std::shared_ptr<Expr> getValue() override {
		return std::shared_ptr<StringExpr>(this);
	}
};