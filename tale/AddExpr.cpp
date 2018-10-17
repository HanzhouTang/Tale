#include"AddExpr.h"
#include"StringExpr.h"
#include"NumberExpr.h"
#include"NullExpr.h"
#include"Utility.h"
namespace expr {
	using namespace Utility;
	std::shared_ptr<Expr> AddExpr::getValue() {
		auto l = getLeft()->getValue();
		auto r = getRight()->getValue();
		if (l->getType() == TYPE_NUMBER && r->getType() == TYPE_NUMBER) {
			auto left = std::dynamic_pointer_cast<NumberExpr>(l);
			auto right = std::dynamic_pointer_cast<NumberExpr>(r);
			return NumberExpr::createNumberExpr(getRunTime(), left->getNumber() + right->getNumber());
		}
		/*
		else if (l->getType() == TYPE_STRING && r->getType() == TYPE_NUMBER) {
			std::wostringstream ret;
			auto left = std::dynamic_pointer_cast<StringExpr>(l);
			auto right = std::dynamic_pointer_cast<NumberExpr>(r);
			ret << left->getString() << right->getNumber();
			return StringExpr::createStringExpr(getRunTime(), ret.str());
		}
		else if (l->getType() == TYPE_NUMBER || r->getType() == TYPE_STRING) {
			std::wostringstream ret;
			auto left = std::dynamic_pointer_cast<NumberExpr>(l);
			auto right = std::dynamic_pointer_cast<StringExpr>(r);
			ret << left->getNumber() << right->getString();
			return StringExpr::createStringExpr(getRunTime(), ret.str());
		}
		*/
		/*else*/ 
		if (l->getType() == TYPE_STRING && r->getType() == TYPE_STRING) {
			auto left = std::dynamic_pointer_cast<StringExpr>(l);
			auto right = std::dynamic_pointer_cast<StringExpr>(r);
			return StringExpr::createStringExpr(getRunTime(), left->getString() + right->getString());
		}
		else {
			quitWithError(__LINE__, __FILE__, L"addition operator doesn't support on type" + l->getTypeString() + L" " + r->getTypeString());
			return NullExpr::createNullExpr();
		}

	}
}

