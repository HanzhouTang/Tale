#include"ArithmeticExpr.h"
#include"NumberExpr.h"
#include"NullExpr.h"
#include"Utility.h"
namespace expr {
	using namespace Utility;
	std::shared_ptr<Expr> ArithmeticExpr::getValue() {
		auto l = getLeft()->getValue();
		auto r = getRight()->getValue();
		if (l->getType() == TYPE_NUMBER && r->getType() == TYPE_NUMBER) {
			auto left = std::dynamic_pointer_cast<NumberExpr>(l);
			auto right = std::dynamic_pointer_cast<NumberExpr>(r);
			if (op == L'-')
				return NumberExpr::createNumberExpr(getRunTime(), left->getNumber() - right->getNumber());
			else if(op ==L'*')
				return NumberExpr::createNumberExpr(getRunTime(), left->getNumber() * right->getNumber());
			else if(op ==L'/')
				return NumberExpr::createNumberExpr(getRunTime(), left->getNumber() / right->getNumber());
			else {
				quitWithError(__LINE__, __FILE__, L"only *, /, - are validated arithmetic operator");
				return NullExpr::createNullExpr();
			}
		}
		else {
			quitWithError(__LINE__, __FILE__, L"only numbers support arithmetic operators");
			return NullExpr::createNullExpr();
		}

	}
}