#pragma once
#include"Expr.h"
namespace expr {
	struct NumberExpr :Expr {
		using NumberType = long double;
		NumberType value;
		NumberExpr(const std::shared_ptr<Expr>& runtime, NumberType v) :
			Expr(runtime), value(v) {
			setType(TYPE_NUMBER);
		}
		static std::shared_ptr<NumberExpr> createNumberExpr(const std::shared_ptr<Expr> runtime, NumberType expressions) {
			auto ret = std::make_shared<NumberExpr>(runtime, expressions);
			return ret;
		}
		static std::shared_ptr<NumberExpr> createNumberExpr(NumberType expressions) {
			return createNumberExpr(nullptr, expressions);
		}
		virtual std::shared_ptr<Expr> clone() override {
			return createNumberExpr(getRunTime(), value);
		}
		virtual std::wstring toString() {
			std::wostringstream ret;
			ret << Expr::toString() << " : " << value;
			return ret.str();
		}

		NumberType getNumber() { return value; }


	};
}