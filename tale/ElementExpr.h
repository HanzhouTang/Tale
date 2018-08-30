#pragma once
#include"Expr.h"
struct Element;
namespace expr {
	struct ElementExpr : Expr {
		std::shared_ptr<Element> _element;
		ElementExpr(const std::shared_ptr<Expr>& runtime, const std::shared_ptr<Element>& element);
		static std::shared_ptr<ElementExpr> createElementExpr(const std::shared_ptr<Expr>& runtime,
			const std::shared_ptr<Element>& element);
		static std::shared_ptr<ElementExpr> createElementExpr(const std::shared_ptr<Element>& element);
		virtual std::shared_ptr<Expr> clone() override;
		std::shared_ptr<Element> getElement();
	};
}