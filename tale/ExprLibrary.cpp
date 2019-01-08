#include"ExprLibrary.h"
#include"NullExpr.h"
#include"Utility.h"
#include"FunctionExpr.h"
#include"ElementExpr.h"
#include"Element.h"
#include"Button.h"
#include"MapExpr.h"
#include"ElementExpr.h"
#include"StringExpr.h"
#include"ClosureExpr.h"
#include"CallExpr.h"
#include"ExternalFunctionExpr.h"
#include"Utility.h"
#include<map>

namespace expr {
	using namespace Utility;
	std::shared_ptr<Expr> printExpr(const std::vector <std::shared_ptr<Expr>>& args)
	{
		for (int i = 0; i < args.size() - 1; i++) {
			std::wcout << args[i]->repr() << " ";
		}
		std::wcout << args.back()->repr() << endl;
		return NullExpr::createNullExpr();
	}

	std::shared_ptr<Expr> debugExpr(const std::vector<std::shared_ptr<Expr>>& args)
	{
		for (int i = 0; i < args.size() - 1; i++) {
			std::wcout << args[i]->toString() << " ";
		}
		std::wcout << args.back()->toString() << endl;
		return NullExpr::createNullExpr();
	}

	std::shared_ptr<Expr> setOnClickEvent(const std::vector<std::shared_ptr<Expr>>& args)
	{
		if (args.size() != 2) {
			quitWithError(__LINE__, __FILE__, Utility::str2wstr(__func__) + L" function must have two input args");
			return NullExpr::createNullExpr();
		}
		auto first = args[0];
		auto second = args[1];
		if (first->getType() != Expr::TYPE_ELEMENT_WRAPPER) {
			quitWithError(__LINE__, __FILE__, L"the first parameter of " + Utility::str2wstr(__func__) + L" function must be a element");
			return NullExpr::createNullExpr();
		}
		else if (second->getType() != Expr::TYPE_FUNCTION) {
			quitWithError(__LINE__, __FILE__, L"the second parameter of " + Utility::str2wstr(__func__) + L" function must be a function");
			return NullExpr::createNullExpr();
		}
		auto element = std::dynamic_pointer_cast<ElementExpr>(first)->getElement();
		if (element->getType() != Element::TYPE_BUTTON) {
			quitWithError(__LINE__, __FILE__, L"only button can be set onclick function");
			return NullExpr::createNullExpr();
		}
		auto button = std::dynamic_pointer_cast<Button>(element);
		auto runtime = second->getRunTime();
		auto call = CallExpr::createCallExpr(runtime, second, {});
		button->setOnClickFunction(call);
		return first;
	}

	std::shared_ptr<Expr> helloWorld(const std::vector<std::shared_ptr<Expr>>& args)
	{
		std::wcout << "hello world" << std::endl;
		return NullExpr::createNullExpr();
	}

	std::shared_ptr<Expr> get(const std::vector<std::shared_ptr<Expr>>& args)
	{
		if (args.size() != 2) {
			quitWithError(__LINE__, __FILE__, L"the parameter size of " + Utility::str2wstr(__func__) + L" method is 2");
		}
		auto map = args[0];
		auto key = args[1];
		if (map->getType() != Expr::TYPE_MAP) {
			quitWithError(__LINE__, __FILE__, L"the " + Utility::str2wstr(__func__) + L" method only works on map");
		}
		if (key->getType() != Expr::TYPE_STRING) {
			quitWithError(__LINE__, __FILE__, L"the key of " + Utility::str2wstr(__func__) + L" method must be string");
		}

		auto getter = std::dynamic_pointer_cast<expr::MapExpr>(map)->getGetter();
		return getter->getValue({ key });
	}

	std::shared_ptr<Expr> set(const std::vector<std::shared_ptr<Expr>>& args)
	{
		if (args.size() != 3) {
			quitWithError(__LINE__, __FILE__, L"the parameter size of " + Utility::str2wstr(__func__) + L" method is 3");
		}
		auto map = args[0];
		auto key = args[1];
		auto value = args[2];
		if (map->getType() != Expr::TYPE_MAP) {
			quitWithError(__LINE__, __FILE__, L"the " + Utility::str2wstr(__func__) + L" method only works on map");
		}
		if (key->getType() != Expr::TYPE_STRING) {
			quitWithError(__LINE__, __FILE__, L"the key of " + Utility::str2wstr(__func__) + L" method must be string");
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
		funcMap.emplace(L"setOnClickEvent", expr::ExternalFunctionExpr::createExternalFunctionExpr(expr::setOnClickEvent));
		funcMap.emplace(L"getElementByID", expr::ExternalFunctionExpr::createExternalFunctionExpr(expr::getElementByID));
		funcMap.emplace(L"setElementAttribute", expr::ExternalFunctionExpr::createExternalFunctionExpr(expr::setElementAttribute));
		if (e->getType() != Expr::TYPE_CLOSURE) {
			quitWithError(__LINE__, __FILE__, L"only can set runtime env on closure");
			return e;
		}
		auto closure = std::dynamic_pointer_cast<expr::ClosureExpr>(e);
		for (auto& x : funcMap) {
			closure->addVarable(x.first, x.second);
		}
		return closure;
	}

	std::shared_ptr<Expr> setRootElement(const std::shared_ptr<Expr>& e, const std::shared_ptr<Element>& root)
	{
		if (e->getType() != Expr::TYPE_CLOSURE) {
			quitWithError(__LINE__, __FILE__, L"only can set element root on closure");
			return e;
		}
		auto closure = std::dynamic_pointer_cast<expr::ClosureExpr>(e);
		auto elementRoot = expr::ElementExpr::createElementExpr(e, root);
		closure->addVarable(Utility::RootName, elementRoot);
		return closure;
	}
	std::shared_ptr<Expr> getElementByID(const std::vector<std::shared_ptr<Expr>>& args)
	{
		if (args.size() != 2) {
			quitWithError(__LINE__, __FILE__, L"the parameter size of " + Utility::str2wstr(__func__) + L" must be 2");
			return NullExpr::createNullExpr();
		}
		auto element = args[0];
		auto id = args[1];
		if (element->getType() != expr::Expr::TYPE_ELEMENT_WRAPPER) {
			quitWithError(__LINE__, __FILE__, L"the first parameter of " + Utility::str2wstr(__func__) + L" must be a element");
			return NullExpr::createNullExpr();
		}
		else if (id->getType() != Expr::TYPE_STRING) {
			quitWithError(__LINE__, __FILE__, L"the second parameter of " + Utility::str2wstr(__func__) + L" must be a string");
			return NullExpr::createNullExpr();
		}
		auto e = std::dynamic_pointer_cast<expr::ElementExpr>(element)->getElement();
		auto ID = std::dynamic_pointer_cast<expr::StringExpr>(id)->getString();
		auto ret = Element::getElementByID(e, ID);
		if (ret == nullptr) {
			return NullExpr::createNullExpr();
		}
		return expr::ElementExpr::createElementExpr(ret);
	}
	std::shared_ptr<Expr> setElementAttribute(const std::vector<std::shared_ptr<Expr>>& args)
	{
		if (args.size() != 3) {
			quitWithError(__LINE__, __FILE__, L"the parameter size of " + Utility::str2wstr(__func__) + L" must be 3");
			return NullExpr::createNullExpr();
		}
		auto element = args[0];
		auto key = args[1];
		auto value = args[2];
		if (element->getType() != expr::Expr::TYPE_ELEMENT_WRAPPER) {
			quitWithError(__LINE__, __FILE__, L"the first parameter of " + Utility::str2wstr(__func__) + L" must be a element");
			return NullExpr::createNullExpr();
		}
		else if (key->getType() != Expr::TYPE_STRING) {
			quitWithError(__LINE__, __FILE__, L"the second parameter of " + Utility::str2wstr(__func__) + L" must be a string");
			return NullExpr::createNullExpr();
		}
		else if (value->getType() != Expr::TYPE_STRING) {
			quitWithError(__LINE__, __FILE__, L"the third parameter of " + Utility::str2wstr(__func__) + L" must be a string");
			return NullExpr::createNullExpr();
		}
		auto e = std::dynamic_pointer_cast<expr::ElementExpr>(element)->getElement();
		auto KEY = std::dynamic_pointer_cast<expr::StringExpr>(key)->getString();
		auto VALUE = std::dynamic_pointer_cast<expr::StringExpr>(value)->getString();
		e->setAttribute(KEY, VALUE);
		return element;
	}
}