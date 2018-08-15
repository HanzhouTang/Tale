#pragma once
#include"Expr.h"
struct SequenceExpr :Expr {
	std::unordered_map<std::wstring, std::shared_ptr<Expr>> context;
	std::vector<std::shared_ptr<Expr>> sentences;
	SequenceExpr(const std::shared_ptr<Expr>& runtime):Expr(runtime) {
		setType(sequence);
	}
	virtual std::shared_ptr<Expr> getValue() override {
		std::shared_ptr<Expr> ret = std::make_shared<Expr>(shared_from_this());
		for (auto x : sentences) {
			ret = x->getValue();
		}
		return ret;
	}
	void addSentence(std::shared_ptr<Expr> s) {
		sentences.emplace_back(s);
	}

};