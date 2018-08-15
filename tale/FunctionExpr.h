#pragma once
#include"Expr.h"
#include"SequenceExpr.h"
struct FunctionExpr :Expr {
	std::unordered_map<std::wstring, std::shared_ptr<Expr>> parameters;
	std::shared_ptr<SequenceExpr> sequence;
	virtual std::shared_ptr<Expr> getValue() override {
		return sequence->getValue();
	}
	void setSequence(std::shared_ptr<SequenceExpr> se) {
		sequence = se;
	}
	FunctionExpr(const std::shared_ptr<Expr>& runtime):Expr(runtime){}
};