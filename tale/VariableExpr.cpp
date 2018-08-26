#include"VariableExpr.h"
#include"Utility.h"
#include"NullExpr.h"
namespace expr {
	std::shared_ptr<Expr> VariableExpr::getValue() {
		if (getRunTime() == nullptr) {
			Utility::quitWithError(__LINE__, __FILE__, L"variable must be declared inside a closure");
			return NullExpr::createNullExpr();
		}
		return getRunTime()->getVariable(std::dynamic_pointer_cast<VariableExpr>(shared_from_this()));
	}
}