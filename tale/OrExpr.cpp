#include"OrExpr.h"
#include"Utility.h"
#include"BooleanExpr.h"
#include"NumberExpr.h"
#include"StringExpr.h"
namespace expr {
	std::shared_ptr<Expr> OrExpr::getValue() {
		auto l = getLeft()->getValue();
		auto r = getRight()->getValue();
		bool lvalue = true;
		if (l->getType() == TYPE_BOOLEAN) {
			lvalue = std::dynamic_pointer_cast<BooleanExpr>(l)->getBoolValue();
		}
		else if (l->getType() == TYPE_NULL) {
			lvalue = false;
		}
		else if (l->getType() == TYPE_NUMBER) {
			if (std::dynamic_pointer_cast<NumberExpr>(l)->getNumber() == 0) {
				lvalue = false;
			}
		}
		else if (l->getType() == TYPE_STRING) {
			if (std::dynamic_pointer_cast<StringExpr>(l)->getString().empty()) {
				lvalue = false;
			}
		}
		bool rvalue = true;
		if (r->getType() == TYPE_BOOLEAN) {
			rvalue = std::dynamic_pointer_cast<BooleanExpr>(r)->getBoolValue();
		}
		else if (r->getType() == TYPE_NULL) {
			rvalue = false;
		}
		else if (r->getType() == TYPE_NUMBER) {
			if (std::dynamic_pointer_cast<NumberExpr>(r)->getNumber() == 0) {
				rvalue = false;
			}
		}
		else if (r->getType() == TYPE_STRING) {
			if (std::dynamic_pointer_cast<StringExpr>(r)->getString().empty()) {
				rvalue = false;
			}
		}
		return expr::BooleanExpr::createBooleanExpr(lvalue||rvalue);
	}

}