#pragma once
#pragma once
#include"BinaryOperatorExpr.h"
namespace expr {
	struct LessThanExpr : BinaryOperatorExpr {
		virtual std::wstring toString() override {
			std::wostringstream ret;
			ret << L"(" << getLeft()->toString() << L")";
			ret << L" < (" << getRight()->toString() << L")";
			return ret.str();
		}

		LessThanExpr(const std::shared_ptr<Expr>& runtime, const std::shared_ptr<Expr>& l, const std::shared_ptr<Expr>& r)
			:BinaryOperatorExpr(runtime, l, r) {
			setType(TYPE_BINARYOPERATION);
		}

		static std::shared_ptr<LessThanExpr> createLessThanExpr(const std::shared_ptr<Expr>& runtime, \
			const std::shared_ptr<Expr>& left, const std::shared_ptr<Expr>& right) {
			auto ret = std::make_shared<LessThanExpr>(runtime, left, right);
			ret->getLeft()->setRunTime(ret);
			ret->getRight()->setRunTime(ret);
			return ret;
		}
		static std::shared_ptr<LessThanExpr> createLessThanExpr(const std::shared_ptr<Expr>& left, const std::shared_ptr<Expr>& right) {
			return createLessThanExpr(nullptr, left, right);
		}
		virtual std::shared_ptr<Expr> clone() override {
			auto ret = createLessThanExpr(runtime, getLeft()->clone(), getRight()->clone());
			ret->getLeft()->setRunTime(ret);
			ret->getRight()->setRunTime(ret);
			return ret;
		}
		std::shared_ptr<Expr> getValue() override;
	};
}