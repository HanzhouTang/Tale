#pragma once
#include"Expr.h"
struct StringExpr : Expr {
	std::wstring expressions;
	StringExpr(const std::shared_ptr<Expr>&r,const std::wstring& str):
		Expr(r), expressions(str) {
		setType(TYPE_STRING);
	}
	static std::shared_ptr<StringExpr> createStringExpr(const std::shared_ptr<Expr>& r, const std::wstring& str) {
		return std::make_shared<StringExpr>(r, str);
	}
	virtual std::shared_ptr<Expr> clone() override {
		return createStringExpr(getRunTime(), expressions);
	}

	virtual std::wstring toString() {
		std::wostringstream ret;
		ret << Expr::toString() << " : " << expressions;
		return ret.str();
	}

};
