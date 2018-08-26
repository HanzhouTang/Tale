#include"ClosureExpr.h"
#include"VariableExpr.h"
#include"NullExpr.h"
#include"Utility.h"
namespace expr {
	using namespace Utility;

	std::shared_ptr<Expr> ClosureExpr::getVariable(const std::shared_ptr<VariableExpr>& variable) {
		auto name = variable->getName();

		if (getContext().count(name)) {
			return getContext()[name];
		}
		else if (getRunTime() == nullptr) {
			quitWithError(__LINE__, __FILE__, L"the variable " + variable->getName() + L" doesn't exist ");
			return NullExpr::createNullExpr();
		}
		return getRunTime()->getVariable(variable);
	}



	std::shared_ptr<Expr> ClosureExpr::setVariable(const std::shared_ptr<VariableExpr>& variable, const std::shared_ptr<Expr>& value) {
		auto name = variable->getName();
		if (getContext().count(name)) {
			addVarable(name, value);
			return shared_from_this();
		}
		else if (getRunTime() == nullptr) {
			return NullExpr::createNullExpr();
		}
		return getRunTime()->setVariable(variable, value);
	}

	std::shared_ptr<Expr> ClosureExpr::getValue()
	{
		std::shared_ptr<Expr> ret = NullExpr::createNullExpr();
		for (const auto& x : expressions) {
			if (x->getType() == TYPE_RETURN) {
				return x->getValue();
			}
			ret = x->getValue();
		}
		return ret;
	}

	void ClosureExpr::store(const std::shared_ptr<Expr>& start) {
		if (getRunTime() == start || getRunTime() == nullptr) {
			return;
		}
		if (stack.size() > maximumRecursionDepth()) {
			quitWithError(__LINE__, __FILE__, L"maximum recursion depth" + std::to_wstring(maximumRecursionDepth()) + L" exceeded");
		}
		auto map = context;
		//std::wcout << "add stack" << std::endl;
		for (auto& x : map) {
			//std::wcout << x.first << " : " << x.second->toString() << std::endl;
			map[x.first] = map[x.first]->clone();
		}

		stack.emplace_back(map);
		getRunTime()->store(start);
	}

	void ClosureExpr::restore(const std::shared_ptr<Expr>& start) {
		if (getRunTime() == start || getRunTime() == nullptr) {
			return;
		}
		if (stack.empty()) {
			quitWithError(__LINE__, __FILE__, L" unbalance restore");
			return;
		}
		auto map = stack.back();
		context = map;
		//std::wcout << " pop stack" << std::endl;
		stack.pop_back();
		getRunTime()->restore(start);
	}
}