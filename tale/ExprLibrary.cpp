#include"ExprLibrary.h"
#include"NullExpr.h"
#include"Utility.h"
#include"FunctionExpr.h"
#include"ElementExpr.h"
#include"Element.h"
#include"Button.h"
#include"MapExpr.h"
#include"ClosureExpr.h"
#include"ExternalFunctionExpr.h"
#include<map>

namespace expr {
	using namespace Utility;
	std::shared_ptr<Expr> printExpr(const std::vector <std::shared_ptr<Expr>>& args)
	{
		for (int i = 0; i < args.size()-1; i++) {
			std::wcout << args[i]->repr() << ", ";
		}
		std::wcout << args.back()->repr() << endl;
		return NullExpr::createNullExpr();
	}

	std::shared_ptr<Expr> debugExpr(const std::vector<std::shared_ptr<Expr>>& args)
	{
		for (int i = 0; i < args.size()-1; i++) {
			std::wcout << args[i]->toString() << ", ";
		}
		std::wcout << args.back()->toString() << endl;
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
			quitWithError(__LINE__, __FILE__, L"only button can be set onclick function");
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

	std::shared_ptr<Expr> get(const std::vector<std::shared_ptr<Expr>>& args)
	{
		if (args.size() != 2) {
			quitWithError(__LINE__, __FILE__, L"the parameter size of get method is 2");
		}
		auto map = args[0];
		auto key = args[1];
		if (map->getType() != Expr::TYPE_MAP) {
			quitWithError(__LINE__, __FILE__, L"the get method only works on map");
		}
		if (key->getType() != Expr::TYPE_STRING) {
			quitWithError(__LINE__, __FILE__, L"the key of get method must be string");
		}
		
		auto getter = std::dynamic_pointer_cast<expr::MapExpr>(map)->getGetter();
		return getter->getValue({ key });
	}

	std::shared_ptr<Expr> set(const std::vector<std::shared_ptr<Expr>>& args)
	{
		if (args.size() != 3) {
			quitWithError(__LINE__, __FILE__, L"the parameter size of set method is 3");
		}
		auto map = args[0];
		auto key = args[1];
		auto value = args[2];
		if (map->getType() != Expr::TYPE_MAP) {
			quitWithError(__LINE__, __FILE__, L"the set method only works on map");
		}
		if (key->getType() != Expr::TYPE_STRING) {
			quitWithError(__LINE__, __FILE__, L"the key of set method must be string");
		}
		auto setter = std::dynamic_pointer_cast<expr::MapExpr>(map)->getSetter();
		return setter->getValue({ key,value });
	}

	std::shared_ptr<Expr> setRuntimeEnv(const std::shared_ptr<Expr>& e)
	{
		static std::map<std::wstring, std::shared_ptr<expr::ExternalFunctionExpr>> funcMap;
		funcMap.emplace(L"print", expr::ExternalFunctionExpr::createExternalFunctionExpr(expr::printExpr));
		funcMap.emplace(L"debug", expr::ExternalFunctionExpr::createExternalFunctionExpr(expr::debugExpr));
		funcMap.emplace(L"get", expr::ExternalFunctionExpr::createExternalFunctionExpr(expr::get));
		funcMap.emplace(L"set", expr::ExternalFunctionExpr::createExternalFunctionExpr(expr::set));
		if (e->getType() != Expr::TYPE_CLOSURE) {
			quitWithError(__LINE__, __FILE__, L"only can set runtime env on closure");
			return e;
		}
		auto closure = std::dynamic_pointer_cast<expr::ClosureExpr>(e);
		for (auto& x : funcMap) {
			closure->addVarable(x.first,x.second);
		}
		return closure;
	}
}