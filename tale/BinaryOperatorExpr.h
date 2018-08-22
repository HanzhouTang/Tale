#pragma once
#include"Expr.h"
struct BinaryOperatorExpr : Expr {
	std::shared_ptr<Expr> left;
	std::shared_ptr<Expr> right;
	virtual std::shared_ptr<Expr> getValue() = 0;
	BinaryOperatorExpr(const std::shared_ptr<Expr>& runtime, const std::shared_ptr<Expr>& l, const std::shared_ptr<Expr>& r)
		: Expr(runtime), left(l), right(r) {
		setType(TYPE_BINARYOPERATION);
	}

	static std::shared_ptr<BinaryOperatorExpr> createBinaryOperatorExpr(const std::shared_ptr<Expr>& runtime, const std::shared_ptr<Expr>& l, const std::shared_ptr<Expr>& r) {
		return std::make_shared<BinaryOperatorExpr>(runtime, l, r);
	}
	virtual std::shared_ptr<Expr> clone() override{
		return createBinaryOperatorExpr(runtime, getLeft()->clone(), getRigth()->clone());
	}
	std::shared_ptr<Expr> getLeft() { return left; }
	std::shared_ptr<Expr> getRigth() { return right; }
	void setLeft(const std::shared_ptr<Expr>& l) { left = l; }
	void setRight(const std::shared_ptr<Expr>& r) { right = r; }
};
