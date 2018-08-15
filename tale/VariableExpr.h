#pragma once
#include"Expr.h"
struct VariableExpr :Expr {
	std::wstring name;
	VariableExpr(const std::shared_ptr<Expr>& runtime,std::wstring n) :
		Expr(runtime),name(n){
		setType(variable);
	}
	std::wstring getVariableName() {
		return name;
	}
};