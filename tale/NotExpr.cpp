#include"NotExpr.h"
#include"Utility.h"
#include"BooleanExpr.h"
#include"NumberExpr.h"
#include"StringExpr.h"
namespace expr {
	using namespace Utility;
	std::shared_ptr<Expr> NotExpr::getValue() {
		auto value = expr->getValue();
		bool ret = true;
		if (value->getType() == TYPE_BOOLEAN) {
			ret = std::dynamic_pointer_cast<BooleanExpr>(value)->getBoolValue();
		}
		else if (value->getType() == TYPE_NULL) {
			ret = false;
		}
		else if (value->getType() == TYPE_NUMBER) {
			if (std::dynamic_pointer_cast<NumberExpr>(value)->getNumber() == 0) {
				ret = false;
			}
		}
		else if (value->getType() == TYPE_STRING) {
			if (std::dynamic_pointer_cast<StringExpr>(value)->getString().empty()) {
				ret = false;
			}
		}
		return expr::BooleanExpr::createBooleanExpr(!ret);
	}
}