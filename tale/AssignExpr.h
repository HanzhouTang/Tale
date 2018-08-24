#pragma once
#include"BinaryOperatorExpr.h"
struct VariableExpr;
struct AssignExpr : BinaryOperatorExpr{
	virtual std::shared_ptr<Expr> getValue() override;
	virtual std::wstring toString() override{
		std::wostringstream ret;
		ret << L"(" << getLeft()->toString() << L")";
		ret << L" = (" << getRight()->toString() << L")";
		return ret.str();
	}
	AssignExpr(const std::shared_ptr<Expr>& runtime, const std::shared_ptr<Expr>& left, const std::shared_ptr<Expr>& right) :
		BinaryOperatorExpr(runtime, left, right) {}
	
	static std::shared_ptr<AssignExpr> createAssignExpr(const std::shared_ptr<Expr>& runtime, \
		const std::shared_ptr<VariableExpr>& left, const std::shared_ptr<Expr>& right) {
		auto ret = std::make_shared<AssignExpr>(runtime, left, right);
		ret->getLeft()->setRunTime(ret);
		ret->getRight()->setRunTime(ret);
		return ret;
	}

	static std::shared_ptr<AssignExpr> createAssignExpr(const std::shared_ptr<VariableExpr>& left, const std::shared_ptr<Expr>& right) {
		return createAssignExpr(nullptr, left, right);
	}


	virtual std::shared_ptr<Expr> clone() override {
		auto ret = createAssignExpr(runtime, std::dynamic_pointer_cast<VariableExpr>(getLeft()->clone()), getRight()->clone());
		ret->getLeft()->setRunTime(ret);
		ret->getRight()->setRunTime(ret);
		return ret;
	}
};