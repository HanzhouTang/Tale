#include"AssignExpr.h"
#include"VariableExpr.h"
#include"ClosureExpr.h"
#include"NullExpr.h"
#include"Utility.h"
namespace expr {
	using namespace Utility;
	std::shared_ptr<Expr> AssignExpr::getValue() {
		using namespace std;
		wcout << "in assignexpr getValue " << endl;
		auto runtime = getRunTime();
		auto variable = std::dynamic_pointer_cast<VariableExpr>(getLeft());
		std::shared_ptr<expr::Expr> result;
		if (getRight()->getType() != Expr::TYPE_FUNCTION) {
			result = getRight()->getValue();
		}
		else {
			result = getRight();
			wcout << "right hand is a variable" << endl;
		}
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