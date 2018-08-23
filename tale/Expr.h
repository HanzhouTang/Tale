#pragma once
#include<iostream>
#include<memory>
#include<string>
#include<sstream>
#include<vector>
#include<initializer_list>
struct VariableExpr;
struct Expr :std::enable_shared_from_this<Expr> {
	enum ExprType { TYPE_NULL=0, TYPE_STRING, TYPE_NUMBER, TYPE_VARIABLE, TYPE_BOOLEAN, TYPE_CLOSURE, TYPE_FUNCTION, TYPE_BINARYOPERATION,TYPE_RETURN };
	ExprType type;
	std::shared_ptr<Expr> runtime;
	static const std::vector<std::wstring> TypeList;
	void setType(ExprType t) { type = t; }
	ExprType getType() { return type; }
	std::shared_ptr<Expr> getRunTime() { return runtime; }
	void setRunTime(const std::shared_ptr<Expr>& r) { runtime = r; }
	Expr(std::shared_ptr<Expr> r) : runtime(r) { setType(TYPE_NULL); }
	virtual ~Expr(){}
	virtual std::shared_ptr<Expr> clone() = 0;
	virtual std::wstring toString() { 
		std::wostringstream ret;
		ret << TypeList[getType()] << L"@"<<shared_from_this();
		return ret.str();
	}
	std::wstring getTypeString() {
		return TypeList[getType()];
	}
	virtual std::shared_ptr<Expr> getValue(std::initializer_list<std::shared_ptr<Expr>> args) { return shared_from_this(); }
	virtual std::shared_ptr<Expr> getValue() { return shared_from_this(); }
	virtual std::shared_ptr<Expr> getVariable(const std::shared_ptr<VariableExpr>& variable);
	virtual std::shared_ptr<Expr> setVariable(const std::shared_ptr<VariableExpr>& variable, const std::shared_ptr<Expr>& value);

};