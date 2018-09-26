#pragma once
#include"Expr.h"
namespace expr {
	struct BooleanExpr : Expr {
		bool value;
		BooleanExpr(const std::shared_ptr<Expr>& runtime, bool v) :
			Expr(runtime), value(v) {
			setType(TYPE_BOOLEAN);
		}
		static std::shared_ptr<BooleanExpr> createBooleanExpr(const std::shared_ptr<Expr>& runtime, bool value) {
			return std::make_shared<BooleanExpr>(runtime, value);
		}
		static std::shared_ptr<BooleanExpr> createBooleanExpr(bool value) {
			return std::make_shared<BooleanExpr>(nullptr, value);
		}
		virtual std::shared_ptr<Expr> clone() override {
			return createBooleanExpr(getRunTime(), value);
		}
		bool getBoolValue() { return value; }


	};
}