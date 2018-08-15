#pragma once
#include"VariableExpr.h"
struct SequenceExpr :Expr {
	std::unordered_map<std::wstring, std::shared_ptr<Expr>> context;
	std::vector<std::shared_ptr<Expr>> sentences;
	SequenceExpr(const std::shared_ptr<Expr>& runtime) :Expr(runtime) {
		setType(sequence);
	}
	virtual std::shared_ptr<Expr> getValue() override {
		std::shared_ptr<Expr> ret = std::make_shared<Expr>(shared_from_this());
		for (auto x : sentences) {
			ret = x->getValue();
		}
		return ret;
	}
	void addSentence(const std::shared_ptr<Expr>& s) {
		sentences.emplace_back(s);
	}
	void setVariable(const std::shared_ptr<VariableExpr>& variable, const std::shared_ptr<Expr>& value) {
		if (context.find(variable->getVariableName()) == context.end()) {
			context.emplace(variable->getVariableName(), value);
		}
		else {
			context[variable->getVariableName()] = value;
		}
	}
	virtual std::shared_ptr<Expr> getVariable(const std::shared_ptr<VariableExpr>& variable) override{
		if (context.find(variable->getVariableName()) != context.end()) {
			return context[variable->getVariableName()];
		}
		else {
			return getRunTime()->getVariable(variable);
		}
	}

};