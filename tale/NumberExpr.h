#pragma once
#include"Expr.h"
struct NumberExpr :Expr {
	float expressions;
	NumberExpr(const std::shared_ptr<Expr>& runtime, float v) :
		Expr(runtime), expressions(v) {
		setType(TYPE_NUMBER);
	}
	static std::shared_ptr<NumberExpr> createNumberExpr(const std::shared_ptr<Expr> runtime, float expressions) {
		auto ret = std::make_shared<NumberExpr>(runtime, expressions);
		return ret;
	}
	virtual std::shared_ptr<Expr> clone() override {
		return createNumberExpr(getRunTime(), expressions);
	}
	virtual std::wstring toString() {
		std::wostringstream ret;
		ret << Expr::toString() << " : " << expressions;
		return ret.str();
	}
};