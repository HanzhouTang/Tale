#include"ReturnExpr.h"
std::shared_ptr<Expr> ReturnExpr::getValue() {
	if (content->getType() == TYPE_VARIABLE || content->getType() == TYPE_BINARYOPERATION || content->getType() == TYPE_CALL)
		return content->getValue();
	return content;
}