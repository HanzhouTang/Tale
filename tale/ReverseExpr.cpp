#include"ReverseExpr.h"
#include"NumberExpr.h"
#include"StringExpr.h"
#include"NullExpr.h"
#include"Utility.h"
namespace expr {
	using namespace Utility;
	std::shared_ptr<Expr> ReverseExpr::getValue() {
		auto result = expr->getValue();
		if (result->getType() == TYPE_NUMBER) {
			return NumberExpr::createNumberExpr(result->getRunTime(), -std::dynamic_pointer_cast<NumberExpr>(result)->getNumber());
		}
		else if (result->getType() == TYPE_STRING) {
			auto str = std::dynamic_pointer_cast<StringExpr>(result)->getString();
			auto reversedStr = std::wstring(str.rbegin(), str.rend());
			return StringExpr::createStringExpr(reversedStr);
		}
		else {
			quitWithError(__LINE__, __FILE__, L"only number can be reserved");
			return NullExpr::createNullExpr();
		}
	}
}