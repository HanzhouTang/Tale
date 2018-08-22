#pragma once
#include"Expr.h"
#include"VariableExpr.h"
#include"NullExpr.h"
#include"Utility.h"
#include<iostream>
#include<memory>
#include<vector>
using namespace Utility;
const std::vector<std::wstring> Expr::TypeList = {
	L"TYPE_NULL", L"TYPE_STRING", L"TYPE_NUMBER", L"TYPE_VARIABLE",L"TYPE_BOOLEAN", L"TYPE_SEQUENCE", L"TYPE_FUNCTION", L"TYPE_BINARYOPERATION"
};

std::shared_ptr<Expr> Expr::getVariable(const std::shared_ptr<VariableExpr>& variable) {
	if (getRunTime() == nullptr) {
		quitWithError(__LINE__, __FILE__, L"the variable " + variable->getName() + L" doesn't exist ");
		return NullExpr::createNullExpr();
	}
	return getRunTime()->getVariable(variable);
}