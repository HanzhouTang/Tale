#include"Expr.h"
#include"VariableExpr.h"
std::shared_ptr<Expr> Expr::getVariable(const std::shared_ptr<VariableExpr>& variable) {
	if (getType() == null || getRunTime() == nullptr) {
		Utility::quitWithError(__LINE__, __FILE__, L"the variable " + variable->getVariableName() + L" is not defined");
		return std::make_shared<Expr>(shared_from_this());
	}
	else {
		return getRunTime()->getVariable(variable);
	}
}