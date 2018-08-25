#pragma once
#include"Expr.h"
#include<unordered_map>
struct VariableExpr;
struct ClosureExpr :Expr {
	std::vector<std::shared_ptr<Expr>> expressions;
	std::unordered_map<std::wstring, std::shared_ptr<Expr>> context;
	std::vector<std::unordered_map<std::wstring, std::shared_ptr<Expr>>> stack;
	ClosureExpr(const std::shared_ptr<Expr>&r) :
		Expr(r) {
		setType(TYPE_CLOSURE);
	}
	void addExpression(const std::shared_ptr<Expr>& expr) {
		auto e = expr;
		e->setRunTime(shared_from_this());
		expressions.emplace_back(e);
	}
	static std::shared_ptr<ClosureExpr> createClosureExpr(const std::shared_ptr<Expr>& r) {
		return std::make_shared<ClosureExpr>(r);
	}

	static std::shared_ptr<ClosureExpr> createClosureExpr() {
		return std::make_shared<ClosureExpr>(nullptr);
	}

	virtual std::shared_ptr<Expr> clone() override {
		auto closure = createClosureExpr(getRunTime());
		std::cout << "clone clousre" << std::endl;
		for (const auto& x : expressions) {
			closure->addExpression(x);
			x->setRunTime(closure);
		}
		for (auto x : getContext()) {
			auto name = x.first;
			auto expr = x.second;
			closure->context[name] = expr;
			closure->context[name]->setRunTime(closure);
		}
		return closure;
	}

	void addVarable(const std::wstring& var, const std::shared_ptr<Expr> val) {
		context[var] = val;
	}

	virtual std::shared_ptr<Expr> getValue() override;

	virtual std::wstring toString() {
		std::wostringstream ret;
		ret << L"================variables================\n";
		for (const auto& x : getContext()) {
			ret << x.first << L" : " << x.second->toString()<<"\n";
		}
		ret<< L"=========================================\n";
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
	std::unordered_map<std::wstring, std::shared_ptr<Expr>>& getContext() {
		return context;
	}
	std::shared_ptr<Expr> getIthSentence(std::size_t i) {
		return expressions[i];
	}
	virtual std::shared_ptr<Expr> getVariable(const std::shared_ptr<VariableExpr>& variable) override;
	virtual std::shared_ptr<Expr> setVariable(const std::shared_ptr<VariableExpr>& variable, const std::shared_ptr<Expr>& value) override;

	virtual void store(const std::shared_ptr<Expr>& begin) override;
	virtual void restore(const std::shared_ptr<Expr>& begin) override;

};
