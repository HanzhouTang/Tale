#include"ReverseExpr.h"
#include"NumberExpr.h"
#include"NullExpr.h"
#include"Utility.h"
namespace expr {
	using namespace Utility;
	std::shared_ptr<Expr> ReverseExpr::getValue() {
		auto result = expr->getValue();
		if (result->getType() == TYPE_NUMBER) {
			return NumberExpr::createNumberExpr(result->getRunTime(), -std::dynamic_pointer_cast<NumberExpr>(result)->getNumber());
		}
		else {
			quitWithError(__LINE__, __FILE__, L"only number can be reserved");
			return NullExpr::createNullExpr();
		}
	}
}