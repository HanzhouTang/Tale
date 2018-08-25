#pragma once
#include "Expr.h"
struct ReverseExpr : Expr {
	std::shared_ptr<Expr> expr;
	ReverseExpr(const std::shared_ptr<Expr>& runtime, const std::shared_ptr<Expr>& e) :
		Expr(runtime), expr(e) {
		setType(UNARY_OPERATOR);
	}
	static std::shared_ptr<ReverseExpr> createReverseExpr(const std::shared_ptr<Expr>& runtime, const std::shared_ptr<Expr>& expr) {
		auto ret = std::make_shared<ReverseExpr>(runtime, expr);
		expr->setRunTime(ret);
		return ret;
	}
	static std::shared_ptr<ReverseExpr> createReverseExpr(const std::shared_ptr<Expr>& expr) {
		return createReverseExpr(nullptr, expr);
	}
	virtual std::wstring toString() override {
		return L"-(" + expr->toString() + L")";
	}
	virtual std::shared_ptr<Expr> clone() override{
		return createReverseExpr(getRunTime(), expr->clone());
	}
	virtual std::shared_ptr<Expr> getValue() override;

};