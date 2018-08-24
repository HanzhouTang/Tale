#include"ReturnExpr.h"
std::shared_ptr<Expr> ReturnExpr::getValue() {
	if (content->getType() == TYPE_VARIABLE || content->getType() == TYPE_BINARYOPERATION)
		return content->getValue();
	return content;
}