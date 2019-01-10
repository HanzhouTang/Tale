#pragma once
#include"Expr.h"
#include"Element.h"
namespace expr {
	std::shared_ptr<Expr> printExpr(const std::vector <std::shared_ptr<Expr>>& args);
	std::shared_ptr<Expr> debugExpr(const std::vector <std::shared_ptr<Expr>>& args);
	std::shared_ptr<Expr> setOnClickEvent(const std::vector <std::shared_ptr<Expr>>& args);
	std::shared_ptr<Expr> helloWorld(const std::vector <std::shared_ptr<Expr>>& args);
	std::shared_ptr<Expr> get(const std::vector <std::shared_ptr<Expr>>& args);
	std::shared_ptr<Expr> set(const std::vector <std::shared_ptr<Expr>>& args);
	std::shared_ptr<Expr> setRuntimeEnv(const std::shared_ptr<Expr>& e);
	std::shared_ptr<Expr> setRootElement(const std::shared_ptr<Expr>& e, const std::shared_ptr<Element>& root);
	std::shared_ptr<Expr> getElementByID(const std::vector <std::shared_ptr<Expr>>& args);
	std::shared_ptr<Expr> setElementAttribute(const std::vector<std::shared_ptr<Expr>>& args);
	std::shared_ptr<Expr> setElementValue(const std::vector<std::shared_ptr<Expr>>& args);
	std::shared_ptr<Expr> str(const std::vector<std::shared_ptr<Expr>>& args);
}