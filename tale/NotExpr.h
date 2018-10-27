#pragma once
#pragma once
#include "Expr.h"
namespace expr {
	struct NotExpr : Expr {
		std::shared_ptr<Expr> expr;
		NotExpr(const std::shared_ptr<Expr>& runtime, const std::shared_ptr<Expr>& e) :
			Expr(runtime), expr(e) {
			setType(TYPE_UNARY_OPERATOR);
		}
		static std::shared_ptr<NotExpr> createNotExpr(const std::shared_ptr<Expr>& runtime, const std::shared_ptr<Expr>& expr) {
			auto ret = std::make_shared<NotExpr>(runtime, expr);
			expr->setRunTime(ret);
			return ret;
		}
		static std::shared_ptr<NotExpr> createNotExpr(const std::shared_ptr<Expr>& expr) {
			return createNotExpr(nullptr, expr);
		}
		virtual std::wstring toString() override {
			return L"not (" + expr->toString() + L")";
		}
		virtual std::shared_ptr<Expr> clone() override {
			return createNotExpr(getRunTime(), expr->clone());
		}
		virtual std::shared_ptr<Expr> getValue() override;
	};
}