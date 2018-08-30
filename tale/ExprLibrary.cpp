#include"ExprLibrary.h"
#include"NullExpr.h"
#include"Utility.h"
#include"FunctionExpr.h"
#include"ElementExpr.h"
#include"Element.h"
#include"Button.h"
namespace expr {
	using namespace Utility;
	std::shared_ptr<Expr> printExpr(const std::vector <std::shared_ptr<Expr>>& args)
	{
		for (auto& x : args) {
			std::wcout << x->toString() << ",\t";
		}
		std::wcout << std::endl;
		return NullExpr::createNullExpr();
	}
	std::shared_ptr<Expr> setOnClickEvent(const std::vector<std::shared_ptr<Expr>>& args)
	{
		if (args.size() != 2) {
			quitWithError(__LINE__, __FILE__, L"setOnClickEvent function must have two input args");
			return NullExpr::createNullExpr();
		}
		auto first = args[0];
		auto second = args[1];
		if (first->getType() != Expr::TYPE_ELEMENT_WRAPPER) {
			quitWithError(__LINE__, __FILE__, L"the first parameter of setOnClickEvent function must be a element");
			return NullExpr::createNullExpr();
		}
		else if (second->getType() != Expr::TYPE_FUNCTION) {
			quitWithError(__LINE__, __FILE__, L"the second parameter of setOnClickEvent function must be a function");
			return NullExpr::createNullExpr();
		}
		auto element = std::dynamic_pointer_cast<ElementExpr>(first)->getElement();
		if (element->getType() != Element::TYPE_BUTTON) {
			quitWithError(__LINE__, __FILE__, L"the only button can be set onclick function");
			return NullExpr::createNullExpr();
		}
		auto button = std::dynamic_pointer_cast<Button>(element);

		


		return std::shared_ptr<Expr>();
	}
	std::shared_ptr<Expr> helloWorld(const std::vector<std::shared_ptr<Expr>>& args)
	{
		std::wcout << "hello world" << std::endl;
		return NullExpr::createNullExpr();
	}
}