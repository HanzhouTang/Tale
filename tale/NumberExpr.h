#pragma once
#include"Expr.h"
struct NumberExpr :Expr {
	float value;
	NumberExpr(const std::shared_ptr<Expr>& runtime, float v) :
		Expr(runtime), value(v) {
		setType(TYPE_NUMBER);
	}
	static std::shared_ptr<NumberExpr> createNumberExpr(const std::shared_ptr<Expr> runtime, float expressions) {
		auto ret = std::make_shared<NumberExpr>(runtime, expressions);
		return ret;
	}
	static std::shared_ptr<NumberExpr> createNumberExpr(float expressions) {
		return createNumberExpr(nullptr, expressions);
	}
	virtual std::shared_ptr<Expr> clone() override {
		return createNumberExpr(getRunTime(), value);
	}
	virtual std::wstring toString() {
		std::wostringstream ret;
		ret << Expr::toString() << " : " << value;
		return ret.str();
	}
	float getNumber() { return value; }
};