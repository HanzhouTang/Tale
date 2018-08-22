#pragma once
#include"Expr.h"
#include<unordered_map>

struct ClosureExpr :Expr {
	std::vector<std::shared_ptr<Expr>> expressions;
	std::unordered_map<std::wstring, std::shared_ptr<Expr>> context;
	ClosureExpr(const std::shared_ptr<Expr>&r) :
		Expr(r) {
		setType(TYPE_CLOSURE);
	}
	void addExpression(const std::shared_ptr<Expr>& expr) {
		auto e = expr->clone();
		e->setRunTime(shared_from_this());
		expressions.emplace_back(e);
	}
	static std::shared_ptr<ClosureExpr> createClosureExpr(const std::shared_ptr<Expr>& r) {
		return std::make_shared<ClosureExpr>(r);
	}

	virtual std::shared_ptr<Expr> clone() override {
		auto closure = createClosureExpr(getRunTime());
		for (const auto& x : expressions) {
			closure->addExpression(x);
		}
		return closure;
	}

	void addVarable(const std::wstring& var, const std::shared_ptr<Expr> val) {
		context[var] = val->clone();
	}

	virtual std::shared_ptr<Expr> getValue() override { 
		std::shared_ptr<Expr> ret;
		for (const auto& x : expressions) {
			ret = x->getValue();
		}
		return ret;
	}

	virtual std::wstring toString() {
		std::wostringstream ret;
		ret << Expr::toString() << " :{\n";
		for (const auto& x : expressions) {
			ret <<x->toString() << "\n";
		}
		ret << "}";
		return ret.str();
	}
	virtual ~ClosureExpr() {
		for (auto& x : expressions) {
			x.reset();
		}
	}

	std::shared_ptr<Expr> getIthSentence(std::size_t i) {
		return expressions[i];
	}
};
