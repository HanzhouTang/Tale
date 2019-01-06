#pragma once
#include"Expr.h"
namespace expr {
	struct BooleanExpr : Expr {
		bool value;
		virtual std::wstring toString() override;
		BooleanExpr(const std::shared_ptr<Expr>& runtime, bool v);
		static std::shared_ptr<BooleanExpr> createBooleanExpr(const std::shared_ptr<Expr>& runtime, bool value);
		static std::shared_ptr<BooleanExpr> createBooleanExpr(bool value);
		virtual std::shared_ptr<Expr> clone() override;
		bool getBoolValue();
	};
}