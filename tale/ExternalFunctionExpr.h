#pragma once
#include<functional>
#include"FunctionExpr.h"
namespace expr {
	struct ExternalFunctionExpr : FunctionExpr {
		using FuncType = std::function<std::shared_ptr<Expr>(std::vector<std::shared_ptr<Expr>>)>;
		FuncType _f;
		ExternalFunctionExpr(const std::shared_ptr<Expr>& runtime, const FuncType& f);
		static std::shared_ptr<ExternalFunctionExpr> createExternalFunctionExpr(const std::shared_ptr<Expr>& runtime, 
			const FuncType& f);
		static std::shared_ptr<ExternalFunctionExpr> createExternalFunctionExpr(const FuncType& f);
		virtual std::shared_ptr<Expr> getValue(const std::vector<std::shared_ptr<Expr>>& args) override;
		virtual std::shared_ptr<Expr> getValue() override;
		virtual std::shared_ptr<Expr> clone() override;

	};
}
