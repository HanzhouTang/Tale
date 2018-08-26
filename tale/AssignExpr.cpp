#include"AssignExpr.h"
#include"VariableExpr.h"
#include"ClosureExpr.h"
#include"NullExpr.h"
#include"Utility.h"
namespace expr {
	using namespace Utility;
	std::shared_ptr<Expr> AssignExpr::getValue() {
		auto runtime = getRunTime();
		auto variable = std::dynamic_pointer_cast<VariableExpr>(getLeft());
		auto result = getRight()->getValue();
		auto ret = setVariable(variable, result);
		if (ret->getType() == TYPE_NULL) {
			if (runtime->getType() != TYPE_CLOSURE) {
				quitWithError(__LINE__, __FILE__, L"assign can only happens inside a closure");
				return NullExpr::createNullExpr();
			}
			auto closure = std::dynamic_pointer_cast<ClosureExpr>(runtime);
			closure->addVarable(variable->getName(), result);
		}
		return result;
	}
}