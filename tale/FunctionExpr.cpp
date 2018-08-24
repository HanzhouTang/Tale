#include"FunctionExpr.h"
#include"ClosureExpr.h"
#include"Utility.h"
#include"NullExpr.h"
using namespace Utility;
FunctionExpr::FunctionExpr(const std::shared_ptr<Expr>& runtime) :Expr(runtime){
	setType(TYPE_FUNCTION);
	closure = ClosureExpr::createClosureExpr(runtime);
}
std::shared_ptr<FunctionExpr> FunctionExpr::createFunctionExpr(const std::shared_ptr<Expr>& runtime) {
	return std::make_shared<FunctionExpr>(runtime);
}
std::shared_ptr<Expr> FunctionExpr::clone() {
	auto ret = createFunctionExpr(getRunTime());
	ret->setClosure(std::dynamic_pointer_cast<ClosureExpr>(getClosure()->clone()));
	std::wcout << "?????" << std::endl;
	ret->closure->setRunTime(ret);
	ret->setSignature(getSignature());
	return ret;
 }

std::shared_ptr<Expr> FunctionExpr::getValue(std::vector<std::shared_ptr<Expr>> args) {
	if (signature.size() == args.size()) {
		for (int i = 0; i < signature.size(); i++) {
			getClosure()->addVarable(signature[i], args[i]);
		}
		std::wcout << "===================variable ====================\n";
		for (const auto& x : getClosure()->getContext()) {
			std::wcout << x.first << L" : " << x.second->toString() << "\n";
		}
		std::wcout << "============================= ====================\n";

		return getClosure()->getValue();
	}
	else {
		quitWithError(__LINE__, __FILE__, L" the signature of function doesn't match");
		return NullExpr::createNullExpr();
	}
}

std::shared_ptr<Expr> FunctionExpr::getValue() {
	if (signature.size() == 0) {
		return getClosure()->getValue();
	}
	else {
		quitWithError(__LINE__, __FILE__, L" the signature of function doesn't match");
		return NullExpr::createNullExpr();
	}
 }

void FunctionExpr::setClosure(const std::shared_ptr<ClosureExpr>& closure) {
	this->closure = closure;
	this->closure->setRunTime(shared_from_this());
}

std::wstring FunctionExpr::toString() 
{
	std::wostringstream ret;
	ret << L"f(";
	for (auto& x : signature) {
		ret << x << L",";
	}
	ret << L")";
	ret << getClosure()->toString();
	return ret.str();
}