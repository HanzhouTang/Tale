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



std::shared_ptr<Expr> ClosureExpr::setVariable(const std::shared_ptr<VariableExpr>& variable, const std::shared_ptr<Expr>& value) {
	auto name = variable->getName();
	if (getContext().count(name)) {
		addVarable(name, value);
		return shared_from_this();
	}
	else if (getRunTime() == nullptr) {
		return NullExpr::createNullExpr();
	}
	return getRunTime()->setVariable(variable,value);
}

std::shared_ptr<Expr> ClosureExpr::getValue()
{
	std::shared_ptr<Expr> ret = NullExpr::createNullExpr();
	for (const auto& x : expressions) {
		if (x->getType() == TYPE_RETURN) {
			auto retValue = x->getValue();
			if (retValue->getType() == TYPE_VARIABLE) {
				return retValue->getValue();
			}
			return retValue;
		}
		ret = x->getValue();
	}
	return ret;
}