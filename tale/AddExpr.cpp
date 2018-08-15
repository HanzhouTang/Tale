#include"AddExpr.h"
#include"NumberExpr.h"
#include"StringExpr.h"
std::shared_ptr<Expr> AddExpr::operate(std::shared_ptr<Expr> l, std::shared_ptr<Expr> r) {
	if (l->getType() == number && r->getType() == number) {
		std::shared_ptr<NumberExpr> lNumber = std::dynamic_pointer_cast<NumberExpr>(l);
		std::shared_ptr<NumberExpr> rNmuber = std::dynamic_pointer_cast<NumberExpr>(r);
		return std::make_shared<NumberExpr>(shared_from_this(),lNumber->getNumber()+rNmuber->getNumber());
	}
	if (l->getType() == string && r->getType() == string) {
		std::shared_ptr<StringExpr> lString = std::dynamic_pointer_cast<StringExpr>(l);
		std::shared_ptr<StringExpr> rString = std::dynamic_pointer_cast<StringExpr>(r);
		return std::make_shared<StringExpr>(shared_from_this(),lString->getString() + rString->getString());
	}
	Utility::quitWithError(__LINE__, __FILE__, L"only number or string support addtion");
	return std::make_shared<Expr>(shared_from_this());
}