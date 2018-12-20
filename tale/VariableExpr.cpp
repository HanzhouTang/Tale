#include"VariableExpr.h"
#include"Utility.h"
#include"NullExpr.h"
namespace expr {
	std::shared_ptr<Expr> VariableExpr::getValue() {
		using namespace std;
		wcout << "in varaible expr getvalue()" << endl;
		if (getRunTime() == nullptr) {
			Utility::quitWithError(__LINE__, __FILE__, L"variable "+this->getName()+L" must be declared inside a closure");
			return NullExpr::createNullExpr();
		}
		wcout << "type "<<getTypeString()<<" runtime type " << getRunTime()->getTypeString() << endl;;
		return getRunTime()->getVariable(std::dynamic_pointer_cast<VariableExpr>(shared_from_this()));
	}
}