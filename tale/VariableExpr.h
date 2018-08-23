#pragma once
#include"Expr.h"
struct VariableExpr : Expr {
	std::wstring name;
	VariableExpr(const std::shared_ptr<Expr>& runtime, const std::wstring& str)
		:Expr(runtime), name(str) {
		setType(TYPE_VARIABLE);
	}
	static std::shared_ptr<VariableExpr> createVariable(const std::shared_ptr<Expr>& runtime, const std::wstring& name) {
		return std::make_shared<VariableExpr>(runtime, name);
	}
	std::shared_ptr<Expr> clone() override {
		return createVariable(runtime, name);
	}
	std::wstring getName() {
		return name;
	}
	void setName(const std::wstring& name) {
		this->name = name;
	}
	virtual std::shared_ptr<Expr> getValue() override;

	std::wstring toString() override {
		std::wostringstream ret;
		ret << Expr::toString();
		ret << "\tname : " << getName()<<"\n";
		return ret.str();
	}
};