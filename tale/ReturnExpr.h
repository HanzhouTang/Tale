#pragma once
#include"Expr.h"
namespace expr {
	struct ReturnExpr :Expr {
		std::shared_ptr<Expr> content;
		ReturnExpr(const std::shared_ptr<Expr>& runtime, const std::shared_ptr<Expr>& c) :
			Expr(runtime), content(c) {
			setType(TYPE_RETURN);
		}

		std::shared_ptr<Expr> getContent() { return content; }
		static std::shared_ptr<ReturnExpr> createReturnExpr(const std::shared_ptr<Expr>& runtime, const std::shared_ptr<Expr>& content) {
			auto ret = std::make_shared<ReturnExpr>(runtime, content);
			ret->getContent()->setRunTime(ret);
			return ret;
		}
		static std::shared_ptr<ReturnExpr> createReturnExpr(const std::shared_ptr<Expr>& content) {
			return createReturnExpr(nullptr, content);
		}

		virtual std::shared_ptr<Expr> getValue() override;
		virtual std::shared_ptr<Expr> clone() override {
			return createReturnExpr(getRunTime(), content->clone());
		}
		virtual std::wstring toString() override {
			return L"return " + content->toString();
		}
	};
}