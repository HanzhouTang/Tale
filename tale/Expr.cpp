#include"Expr.h"
#include"VariableExpr.h"


const std::vector<std::wstring> Expr::TypeList = {
	L"null", L"string", L"number", L"variable",L"function", L"boolean",L"sequence",L"ifElse",L"binaryOperator"
};

std::shared_ptr<Expr> Expr::getVariable(const std::shared_ptr<VariableExpr>& variable) {
	if (getType() == null || getRunTime() == nullptr) {
		Utility::quitWithError(__LINE__, __FILE__, L"the variable " + variable->getVariableName() + L" is not defined");
		return std::make_shared<Expr>(shared_from_this());
	}
	else {
		return getRunTime()->getVariable(variable);
	}
}