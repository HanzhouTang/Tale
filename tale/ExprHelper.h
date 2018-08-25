#pragma once
#include"NumberExpr.h"
#include"StringExpr.h"
#include"ClosureExpr.h"
#include"VariableExpr.h"
#include"AssignExpr.h"
#include"AddExpr.h"
#include"ReturnExpr.h"
#include"FunctionExpr.h"
#include"GreaterThanExpr.h"
#include"ConditionExpr.h"
#include"EqualExpr.h"
#include"NullExpr.h"
#include"CallExpr.h"
#include"BooleanExpr.h"
#include"ReverseExpr.h"
#include"ExprHelper.h"

std::shared_ptr<NumberExpr> operator ""_expr(long double value) {
	return NumberExpr::createNumberExpr(value);
}
std::shared_ptr<NumberExpr> operator ""_expr(unsigned long long int value) {
	return NumberExpr::createNumberExpr(value);
}

std::shared_ptr<StringExpr> operator ""_expr(const wchar_t* value, std::size_t) {
	return StringExpr::createStringExpr(value);
}

std::shared_ptr<VariableExpr> operator ""_variableExpr(const wchar_t* value, std::size_t) {
	return VariableExpr::createVariableExpr(value);
}

std::shared_ptr<AssignExpr> operator <<(const std::shared_ptr<VariableExpr>& l, const std::shared_ptr<Expr>& r) {
	return AssignExpr::createAssignExpr(l, r);
}
std::shared_ptr<ReverseExpr> operator -(const std::shared_ptr<Expr>& expr) {
	return ReverseExpr::createReverseExpr(expr);
}

std::shared_ptr<AddExpr> operator + (const std::shared_ptr<Expr>& left, const std::shared_ptr<Expr>& right) {
	return AddExpr::createAddExpr(left, right);
}

std::shared_ptr<AddExpr> operator -(const std::shared_ptr<Expr>& left, const std::shared_ptr<Expr>& right) {
	return left + -right;
}

std::shared_ptr<EqualExpr> operator <<=(const std::shared_ptr<Expr>& left, const std::shared_ptr<Expr>& right) {
	return EqualExpr::createEqualExpr(left, right);
}
