#pragma once
#include"Expr.h"
struct NullExpr : Expr {
	NullExpr() :
		Expr(nullptr) {
		setType(TYPE_NULL);
	}
	const static std::shared_ptr<NullExpr> NONE;
	static std::shared_ptr<NullExpr> createNullExpr() {
		return NONE;
	}
	virtual std::shared_ptr<Expr> clone() override {
		return NONE;
	}
	virtual std::wstring toString() {
		return L"TYPE_NULL";
	}
};
