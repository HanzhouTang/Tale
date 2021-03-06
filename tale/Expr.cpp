#pragma once
#include"Expr.h"
#include"VariableExpr.h"
#include"CallExpr.h"
#include"NullExpr.h"
#include"Utility.h"
#include<iostream>
#include<memory>
#include<vector>
namespace expr {
	using namespace Utility;
	const std::vector<std::wstring> Expr::TypeList = {
		L"TYPE_NULL", L"TYPE_STRING", L"TYPE_NUMBER", L"TYPE_VARIABLE",L"TYPE_BOOLEAN",
		L"TYPE_CLOSURE", L"TYPE_FUNCTION", L"TYPE_BINARYOPERATION",L"TYPE_RETURN" ,L"TYPE_CONDITION",L"TYPE_CALL",L"TYPE_MAP",
		L"TYPE_UNARY_OPERATOR",L"TYPE_ELEMENT_WRAPPER"
	};

	std::wstring Expr::repr()
	{
		return getTypeString();
	}

	std::shared_ptr<Expr> Expr::getVariable(const std::shared_ptr<VariableExpr>& variable) {
		if (getRunTime() == nullptr) {
			quitWithError(__LINE__, __FILE__, L"the variable " + variable->getName() + L" doesn't exist ");
			return NullExpr::createNullExpr();
		}
		return getRunTime()->getVariable(variable);
	}

	std::shared_ptr<Expr> Expr::setVariable(const std::shared_ptr<VariableExpr>& variable, const std::shared_ptr<Expr>& value) {
		if (getRunTime() == nullptr) {
			return NullExpr::createNullExpr();
		}
		return getRunTime()->setVariable(variable, value);
	}

	void Expr::store(const std::shared_ptr<Expr>& start) {
		if (getRunTime() == start || getRunTime() == nullptr) {
			return;
		}
		getRunTime()->store(start);
	}

	void Expr::restore(const std::shared_ptr<Expr>& start) {
		if (getRunTime() == start || getRunTime() == nullptr) {
			return;
		}
		getRunTime()->restore(start);
	}
	std::shared_ptr<CallExpr> Expr::operator ()(const std::vector<std::shared_ptr<Expr>>& param) {
		return CallExpr::createCallExpr(nullptr, shared_from_this(), param);
	}
}