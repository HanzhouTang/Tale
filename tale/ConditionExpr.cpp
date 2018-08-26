#include"ConditionExpr.h"
#include"BooleanExpr.h"
#include"NumberExpr.h"
#include"StringExpr.h"
#include"NullExpr.h"
namespace expr {
	std::shared_ptr<Expr> ConditionExpr::getValue() {
		auto con = condition->getValue();
		bool value = true;
		if (con->getType() == TYPE_BOOLEAN) {
			value = std::dynamic_pointer_cast<BooleanExpr>(con)->getBoolValue();
		}
		else if (con->getType() == TYPE_NULL) {
			value = false;
		}
		else if (con->getType() == TYPE_NUMBER) {
			if (std::dynamic_pointer_cast<NumberExpr>(con)->getNumber() == 0) {
				value = false;
			}
		}
		else if (con->getType() == TYPE_STRING) {
			if (std::dynamic_pointer_cast<StringExpr>(con)->getString().empty()) {
				value = false;
			}
		}
		if (value) {
			return getIfStatement()->getValue();
		}
		else {
			return getElseStatement()->getValue();
		}
	}

	std::shared_ptr<ConditionExpr> ConditionExpr::createConditionExpr(const std::shared_ptr<Expr>& runtime, const std::shared_ptr<Expr>& con,
		const std::shared_ptr<Expr>& ifs, const std::shared_ptr<Expr>& els) {
		if (els == nullptr) {
			return  std::make_shared<ConditionExpr>(runtime, con, ifs, NullExpr::createNullExpr());
		}
		auto ret = std::make_shared<ConditionExpr>(runtime, con, ifs, els);
		ret->getCondition()->setRunTime(ret);
		ret->getIfStatement()->setRunTime(ret);
		ret->getElseStatement()->setRunTime(ret);
		return ret;
	}
}