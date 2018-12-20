#pragma once
#include"Expr.h"
#include"NullExpr.h"
#include"Utility.h"
namespace expr {
	struct CallExpr :Expr {
		std::vector<std::shared_ptr<Expr>> parameters;
		std::shared_ptr<Expr> callable;
		void setCallable(const std::shared_ptr<Expr>& f) {
			callable = f;
		}
		void setParameters(const std::vector<std::shared_ptr<Expr>>& p) {
			parameters = p;
		}
		std::shared_ptr<Expr> getCallable() {
			using namespace std;
			if (callable->getType() == Expr::TYPE_VARIABLE) {
				
				return callable->getValue();
				
			}
			return callable;
			//Utility::quitWithError(__LINE__, __FILE__, L"type " + callable->getTypeString() + L" cannot be callable");
			//return NullExpr::createNullExpr();
		}
		std::vector<std::shared_ptr<Expr>> getParameters() { return parameters; }
		CallExpr(const std::shared_ptr<Expr>& runtime, const std::shared_ptr<Expr>& f,
			const std::vector<std::shared_ptr<Expr>>& param) : Expr(runtime), parameters(param), callable(f) {
			setType(TYPE_CALL);
		}
		static std::shared_ptr<CallExpr> createCallExpr(const std::shared_ptr<Expr>& runtime, const std::shared_ptr<Expr>& f,
			const std::vector<std::shared_ptr<Expr>>& param);

		/*static std::shared_ptr<CallExpr> createCallExpr(const std::shared_ptr<Expr>& runtime, const std::shared_ptr<Expr>& f) {
			return createCallExpr(runtime, f, std::vector<std::shared_ptr<Expr>>());
		}*/
		static std::shared_ptr<CallExpr> createCallExpr(const std::shared_ptr<Expr>& f,
			const std::vector<std::shared_ptr<Expr>>& param) {
			return createCallExpr(nullptr, f, param);
		}
		static std::shared_ptr<CallExpr> createCallExpr(const std::shared_ptr<Expr>& f) {
			return createCallExpr(nullptr, f, std::vector<std::shared_ptr<Expr>>());
		}
		virtual std::shared_ptr<Expr> clone() override;

		virtual std::wstring toString() override;
		virtual std::shared_ptr<Expr> getValue() override;
	};
}