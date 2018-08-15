#pragma once
#include"BinaryOperatorExpr.h"
struct AddExpr :BinaryOperatorExpr {
	virtual std::shared_ptr<Expr> operate(std::shared_ptr<Expr> l, std::shared_ptr<Expr> r) override;
};
