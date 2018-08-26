#pragma once
#include"Expr.h"
namespace expr {
	struct StringExpr : Expr {
		std::wstring value;
		StringExpr(const std::shared_ptr<Expr>&r, const std::wstring& str) :
			Expr(r), value(str) {
			setType(TYPE_STRING);
		}
		static std::shared_ptr<StringExpr> createStringExpr(const std::shared_ptr<Expr>& r, const std::wstring& str) {
			return std::make_shared<StringExpr>(r, str);
		}

		static std::shared_ptr<StringExpr> createStringExpr(const std::wstring& str) {
			return createStringExpr(nullptr, str);
		}
		virtual std::shared_ptr<Expr> clone() override {
			return createStringExpr(getRunTime(), value);
		}

		virtual std::wstring toString() {
			std::wostringstream ret;
			ret << Expr::toString() << " : " << value;
			return ret.str();
		}
		std::wstring getString() { return value; }
	};
}