#include"GreaterThanExpr.h"
#include"NumberExpr.h"
#include"StringExpr.h"
#include"BooleanExpr.h"
#include"NullExpr.h"
#include"Utility.h"
using namespace Utility;

std::shared_ptr<Expr> GreaterThanExpr::getValue() {
	auto left = getLeft()->getValue();
	auto right = getRight()->getValue();
	if (left->getType() == TYPE_NUMBER && right->getType() == TYPE_NUMBER) {
		return BooleanExpr::createBooleanExpr(getRunTime(), 
			std::dynamic_pointer_cast<NumberExpr>(left)->getNumber() > 
			std::dynamic_pointer_cast<NumberExpr>(right)->getNumber());
	}
	else if (left->getType() == TYPE_STRING && right->getType() == TYPE_STRING) {
		return BooleanExpr::createBooleanExpr(getRunTime(),
			std::dynamic_pointer_cast<StringExpr>(left)->getString() >
			std::dynamic_pointer_cast<StringExpr>(right)->getString());
	}
	else {
		quitWithError(__LINE__, __FILE__, L"only string or number can be compared");
		return NullExpr::createNullExpr();
	}
}
