#pragma once
#pragma once
#include"BinaryOperatorExpr.h"
namespace expr {
	struct OrExpr :BinaryOperatorExpr {
		std::shared_ptr<Expr> getValue() override;
		virtual std::wstring toString() override {
			std::wostringstream ret;
			ret << L"(" << getLeft()->toString() << L")";
			ret << L" or (" << getRight()->toString() << L")";
			return ret.str();
		}
		OrExpr(const std::shared_ptr<Expr>& runtime, const std::shared_ptr<Expr>& l, const std::shared_ptr<Expr>& r)
			:BinaryOperatorExpr(runtime, l, r) {
			setType(TYPE_BINARYOPERATION);
		}
		static std::shared_ptr<OrExpr> createOrExpr(const std::shared_ptr<Expr>& runtime, \
			const std::shared_ptr<Expr>& left, const std::shared_ptr<Expr>& right) {
			auto ret = std::make_shared<OrExpr>(runtime, left, right);
			ret->getLeft()->setRunTime(ret);
			ret->getRight()->setRunTime(ret);
			return ret;
		}
		static std::shared_ptr<OrExpr> createOrExpr(const std::shared_ptr<Expr>& left, const std::shared_ptr<Expr>& right) {
			return createOrExpr(nullptr, left, right);
		}

		virtual std::shared_ptr<Expr> clone() override {
			auto ret = createOrExpr(runtime, getLeft()->clone(), getRight()->clone());
			ret->getLeft()->setRunTime(ret);
			ret->getRight()->setRunTime(ret);
			return ret;
		}
	};
}