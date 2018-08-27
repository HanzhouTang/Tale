#pragma once
#include"Expr.h"
namespace expr {
	struct ClosureExpr;
	struct FunctionExpr : Expr {
		std::vector<std::wstring> signature;
		std::shared_ptr<ClosureExpr> closure;
		virtual std::shared_ptr<Expr> getValue(const std::vector<std::shared_ptr<Expr>>& args) override;
		virtual std::shared_ptr<Expr> getValue() override;
		void setSignature(const std::vector<std::wstring>& parameters) {
			signature = parameters;
		}
		std::vector<std::wstring> getSignature() { return signature; }
		FunctionExpr(const std::shared_ptr<Expr>& runtime);
		static std::shared_ptr<FunctionExpr> createFunctionExpr(const std::shared_ptr<Expr>& runtime);
		static std::shared_ptr<FunctionExpr> createFunctionExpr() {
			return createFunctionExpr(nullptr);
		}
		virtual std::shared_ptr<Expr> clone() override;
		std::shared_ptr<ClosureExpr> getClosure() {
			return closure;
		}
		void setClosure(const std::shared_ptr<ClosureExpr>& closure);
		virtual std::wstring toString() override;

	};
}
//const std::shared_ptr<FunctionExpr>& f,
//