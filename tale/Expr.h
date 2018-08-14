#pragma once
#include<memory>
#include<iostream>
#include<string>
#include<sstream>
#include<unordered_map>
#include<vector>
struct Expr {
	enum ExprType { null, string, number, variable, function, boolean,sequence };
	ExprType type;
	void setType(ExprType t) { type = t; }
	ExprType getType() { return type; }
	Expr() {
		setType(null);
	}
	virtual std::shared_ptr<Expr> getValue(){
		return std::shared_ptr<Expr>(this);
	}
};