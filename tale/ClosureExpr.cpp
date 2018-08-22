#include"ClosureExpr.h"
#include"VariableExpr.h"
#include"NullExpr.h"
#include"Utility.h"
using namespace Utility;

std::shared_ptr<Expr> ClosureExpr::getVariable(const std::shared_ptr<VariableExpr>& variable) {
	auto name = variable->getName();
	if (getContext().count(name)) {
		return getContext()[name];
	}
	else if (getRunTime() == nullptr) {
		quitWithError(__LINE__, __FILE__, L"the variable " + variable->getName() + L" doesn't exist ");
		return NullExpr::createNullExpr();
	}
	return getRunTime()->getVariable(variable);
}