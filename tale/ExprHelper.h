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