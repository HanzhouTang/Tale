#pragma once
#include"Expr.h"
struct FunctionExpr;
struct CallExpr :Expr{
	std::vector<std::shared_ptr<Expr>> parameters;
	std::shared_ptr<FunctionExpr> function;
	void setFucntion(const std::shared_ptr<FunctionExpr>& f) {
		function = f;
	}
	void setParameters(const std::vector<std::shared_ptr<Expr>>& p) {
		parameters = p;
	}
	std::shared_ptr<FunctionExpr> getFunction() {
		return function;
	}
	std::vector<std::shared_ptr<Expr>> getParameters() { return parameters; }
	CallExpr(const std::shared_ptr<Expr>& runtime, const std::shared_ptr<FunctionExpr>& f,
		const std::vector<std::shared_ptr<Expr>>& param) : Expr(runtime),parameters(param), function(f) {
		setType(TYPE_CALL);
	}
	static std::shared_ptr<CallExpr> createCallExpr(const std::shared_ptr<Expr>& runtime, const std::shared_ptr<FunctionExpr>& f,
		const std::vector<std::shared_ptr<Expr>>& param);
	static std::shared_ptr<CallExpr> createCallExpr(const std::shared_ptr<Expr>& runtime, const std::shared_ptr<FunctionExpr>& f) {
		return createCallExpr(runtime, f, std::vector<std::shared_ptr<Expr>>());
	}
	static std::shared_ptr<CallExpr> createCallExpr( const std::shared_ptr<FunctionExpr>& f,
		const std::vector<std::shared_ptr<Expr>>& param) {
		return createCallExpr(nullptr, f, param);
	}
	static std::shared_ptr<CallExpr> createCallExpr(const std::shared_ptr<FunctionExpr>& f) {
		return createCallExpr(nullptr, f, std::vector<std::shared_ptr<Expr>>());
	}
	virtual std::shared_ptr<Expr> clone() override;

	virtual std::wstring toString() override;
	virtual std::shared_ptr<Expr> getValue() override;
};
