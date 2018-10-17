#pragma once
#include"Expr.h"
namespace expr {
	struct BinaryOperatorExpr : Expr {
		std::shared_ptr<Expr> left;
		std::shared_ptr<Expr> right;
		BinaryOperatorExpr(const std::shared_ptr<Expr>& runtime, const std::shared_ptr<Expr>& l, const std::shared_ptr<Expr>& r)
			: Expr(runtime), left(l), right(r) {
			setType(TYPE_BINARYOPERATION);
		}

		static std::shared_ptr<BinaryOperatorExpr> createBinaryOperatorExpr(const std::shared_ptr<Expr>& runtime, const std::shared_ptr<Expr>& l, const std::shared_ptr<Expr>& r) {
			return std::make_shared<BinaryOperatorExpr>(runtime, l, r);
		}

		void setLeft(const std::shared_ptr<Expr>& l) {
			left = l;
			left->setRunTime(shared_from_this());
		}

		void setRight(const std::shared_ptr<Expr>& r) {
			right = r;
			right->setRunTime(shared_from_this());
		}


		virtual std::shared_ptr<Expr> clone() override {
			auto ret = createBinaryOperatorExpr(runtime, getLeft()->clone(), getRight()->clone());
			ret->getLeft()->setRunTime(ret);
			ret->getRight()->setRunTime(ret);
			return ret;
		}
		virtual ~BinaryOperatorExpr() {
			left.reset();
			right.reset();
		}
		std::shared_ptr<Expr> getLeft() { return left; }
		std::shared_ptr<Expr> getRight() { return right; }

	};
}