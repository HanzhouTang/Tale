#pragma once
#include<iostream>
#include<memory>
#include<vector>
struct Expr {
	virtual std::shared_ptr<Expr> clone() = 0;
	virtual std::wstring toString() = 0;

	template< typename... Arguments >
	virtual std::shared_ptr<Expr> getValue(std::shared_ptr<Expr> value, Arguments... args);

	virtual std::shared_ptr<Expr> getValue();
};