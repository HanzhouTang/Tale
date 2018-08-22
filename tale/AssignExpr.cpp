#include"AssignExpr.h"
#include"VariableExpr.h"
#include"ClosureExpr.h"
#include"NullExpr.h"
#include"Utility.h"
using namespace Utility;
std::shared_ptr<Expr> AssignExpr::getValue() {
	auto runtime = getRunTime();
	if (runtime->getType() != TYPE_CLOSURE) {
		quitWithError(__LINE__, __FILE__, L"assign can only happens inside a closure");
		return NullExpr::createNullExpr();
	}
	auto closure = std::dynamic_pointer_cast<ClosureExpr>(runtime);
	auto variable = std::dynamic_pointer_cast<VariableExpr>(getLeft());
	closure->addVarable(variable->getName(), getRight()->getValue());
	return closure->getContext()[variable->getName()];
}
