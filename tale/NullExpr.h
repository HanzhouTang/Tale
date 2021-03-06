#pragma once
#include"Expr.h"
namespace expr {
	struct NullExpr : Expr {
		NullExpr() :
			Expr(nullptr) {
			setType(TYPE_NULL);
		}
		const static std::shared_ptr<NullExpr> NONE;
		static std::shared_ptr<NullExpr> createNullExpr() {
			return NONE;
		}
		virtual void setRunTime(const std::shared_ptr<Expr>& r) override { return; }

		virtual std::shared_ptr<Expr> clone() override {
			return NONE;
		}
		virtual std::wstring toString() override{
			return L"TYPE_NULL";
		}
		virtual std::wstring repr() override {
			return L"null";
		}
	};
}