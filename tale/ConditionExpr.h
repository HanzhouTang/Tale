#pragma once
#include"Expr.h"
namespace expr{
	struct ConditionExpr : Expr {
		std::shared_ptr<Expr> ifStatement;
		std::shared_ptr<Expr> elseStatement;
		std::shared_ptr<Expr> condition;
		ConditionExpr(const std::shared_ptr<Expr>& runtime, const std::shared_ptr<Expr>& con,
			const std::shared_ptr<Expr>& ifs, const std::shared_ptr<Expr>& els)
			:Expr(runtime), condition(con), ifStatement(ifs), elseStatement(els) {
			setType(TYPE_CONDITION);
		}
		static std::shared_ptr<ConditionExpr> createConditionExpr(const std::shared_ptr<Expr>& runtime, const std::shared_ptr<Expr>& con,
			const std::shared_ptr<Expr>& ifs, const std::shared_ptr<Expr>& els);
		std::shared_ptr<Expr> getCondition() { return condition; }
		std::shared_ptr<Expr> getIfStatement() { return ifStatement; }
		std::shared_ptr<Expr> getElseStatement() { return elseStatement; }
		virtual std::shared_ptr<Expr> clone() override {
			return createConditionExpr(getRunTime(), getCondition()->clone(), getIfStatement()->clone(), getElseStatement()->clone());
		}
		static std::shared_ptr<ConditionExpr> createConditionExpr(const std::shared_ptr<Expr>& con,
			const std::shared_ptr<Expr>& ifs, const std::shared_ptr<Expr>& els) {
			return createConditionExpr(nullptr, con, ifs, els);
		}
		virtual std::shared_ptr<Expr> getValue() override;
		virtual std::wstring toString() override {
			std::wostringstream buffer;
			buffer << L"if( " << getCondition()->toString() << L") {\n";
			buffer << getIfStatement()->toString() << L"}\n";
			buffer << "else {\n" << getElseStatement()->toString() << "}";
			return buffer.str();
		}
	};
}