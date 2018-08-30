#include"ExternalFunctionExpr.h"
namespace expr {
	ExternalFunctionExpr::ExternalFunctionExpr(const std::shared_ptr<Expr>& runtime, const FuncType& f) :
		FunctionExpr(runtime), _f(f) {}

	std::shared_ptr<ExternalFunctionExpr> ExternalFunctionExpr::createExternalFunctionExpr(const std::shared_ptr<Expr>& runtime, const FuncType & f)
	{
		return std::make_shared<ExternalFunctionExpr>(runtime, f);
	}
	std::shared_ptr<ExternalFunctionExpr> ExternalFunctionExpr::createExternalFunctionExpr(const FuncType & f)
	{
		return createExternalFunctionExpr(nullptr, f);
	}

	std::shared_ptr<Expr> ExternalFunctionExpr::getValue(const std::vector<std::shared_ptr<Expr>>& parameters) {
		std::vector<std::shared_ptr<Expr>> args;
		for (int i = 0; i < parameters.size(); i++) {
			auto x = parameters[i]->getValue();
			args.emplace_back(x);
		}
		return _f(args);
	}
	std::shared_ptr<Expr> ExternalFunctionExpr::clone() {
		return createExternalFunctionExpr(getRunTime(), _f);
	}
}