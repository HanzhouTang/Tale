#pragma once
#include<memory>
#include<iostream>
#include<string>
#include<sstream>
#include<unordered_map>
#include<vector>
#include"Utility.h"
struct VariableExpr;
struct Expr: std::enable_shared_from_this<Expr> {
	enum ExprType { null=0, string, number, variable, function, boolean,sequence,ifElse,binaryOperator };
	static const std::vector<std::wstring> TypeList;
	ExprType type;
	std::shared_ptr<Expr> runtime;
	void setType(ExprType t) { type = t; }
	void setRunTime(const std::shared_ptr<Expr>& r) {
		runtime = r;
	}
	std::shared_ptr<Expr> getRunTime() { return runtime; }
	ExprType getType() { return type; }
	Expr(std::shared_ptr<Expr> p):runtime(p) {
		setType(null);
	}
	virtual std::shared_ptr<Expr> getValue(){
		return shared_from_this();
	}
	virtual std::shared_ptr<Expr> getVariable(const std::shared_ptr<VariableExpr>& variable);
	virtual std::wstring toString() { return TypeList[getType()]; }
};