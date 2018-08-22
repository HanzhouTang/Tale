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


std::shared_ptr<Expr> ClosureExpr::getValue()
{
	std::shared_ptr<Expr> ret = NullExpr::createNullExpr();
	for (const auto& x : expressions) {
		ret = x->getValue();
	}
	return ret;
}