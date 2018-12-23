#include"ArithmeticExpr.h"
#include"NumberExpr.h"
#include"NullExpr.h"
#include"Utility.h"
#include"StringExpr.h"
namespace expr {
	using namespace Utility;
	std::shared_ptr<Expr> ArithmeticExpr::getValue() {
		auto l = getLeft()->getValue();
		auto r = getRight()->getValue();
		if (l->getType() == TYPE_NUMBER && r->getType() == TYPE_NUMBER) {
			return getValueFromTwoNumber(l,r);
		}
		else if (l->getType() == TYPE_STRING && r->getType() == TYPE_STRING) {
			return getValueFromTwoString(l, r);
		}

		quitWithError(__LINE__, __FILE__, L"only numbers or string support arithmetic operators");
		return NullExpr::createNullExpr();

	}

	std::shared_ptr<Expr> ArithmeticExpr::getValueFromTwoNumber(
		const std::shared_ptr<Expr>& l, const std::shared_ptr<Expr>& r)
	{

		auto left = std::dynamic_pointer_cast<NumberExpr>(l);
		auto right = std::dynamic_pointer_cast<NumberExpr>(r);
		if (op == L'+')
			return NumberExpr::createNumberExpr(getRunTime(), left->getNumber() + right->getNumber());
		else if (op == L'-')
			return NumberExpr::createNumberExpr(getRunTime(), left->getNumber() - right->getNumber());
		else if (op == L'*')
			return NumberExpr::createNumberExpr(getRunTime(), left->getNumber() * right->getNumber());
		else if (op == L'/')
			return NumberExpr::createNumberExpr(getRunTime(), left->getNumber() / right->getNumber());
		quitWithError(__LINE__, __FILE__, L"only *, /, - are validated arithmetic operator");
		return NullExpr::createNullExpr();

	}

	std::shared_ptr<Expr> ArithmeticExpr::getValueFromTwoString(
		const std::shared_ptr<Expr>& l, const std::shared_ptr<Expr>& r)
	{
		if (op == L'+') {
			auto left = std::dynamic_pointer_cast<StringExpr>(l);
			auto right = std::dynamic_pointer_cast<StringExpr>(r);
			return StringExpr::createStringExpr(getRunTime(), left->getString() + right->getString());
		}
		quitWithError(__LINE__, __FILE__, L"only numbers or string support arithmetic operators");
		return NullExpr::createNullExpr();
	}

	
}