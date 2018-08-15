#pragma once
#include"SequenceExpr.h"
struct IfExpr :Expr {
	std::shared_ptr<Expr> condition;
	std::shared_ptr<SequenceExpr> ifStatement;
	std::shared_ptr<SequenceExpr> elseStatement;
	IfExpr(const std::shared_ptr<Expr>& runtime) :Expr(runtime) {}
};