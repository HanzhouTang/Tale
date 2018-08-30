#include"ElementExpr.h"
#include"Element.h"
namespace expr {
	ElementExpr::ElementExpr(const std::shared_ptr<Expr>& runtime, 
		const std::shared_ptr<Element>& element) :Expr(runtime),_element(element){
		setType(TYPE_ELEMENT_WRAPPER);
	}

	std::shared_ptr<ElementExpr> ElementExpr::createElementExpr(const std::shared_ptr<Expr>& runtime, const std::shared_ptr<Element>& element)
	{
		return std::make_shared<ElementExpr>(runtime, element);
	}

	std::shared_ptr<ElementExpr> ElementExpr::createElementExpr(const std::shared_ptr<Element>& element)
	{
		return createElementExpr(nullptr, element);
	}

	std::shared_ptr<Expr> ElementExpr::clone()
	{
		return shared_from_this();
	}

	std::shared_ptr<Element> ElementExpr::getElement()
	{
		return _element;
	}

}