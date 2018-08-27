#include"MapExpr.h"
#include"Utility.h"
#include"NullExpr.h"
#include"StringExpr.h"
#include"FunctionExpr.h"
#include"ClosureExpr.h"
#include"ExprLiteral.h"
#include"ReturnExpr.h"
#include"CallExpr.h"
#include"VariableExpr.h"
namespace expr {
	using namespace Utility;
	std::shared_ptr<Expr> MapExpr::get(const KeyType& key) {
		if (map.count(key)) {
			return map[key];
		}
		else {
			quitWithError(__LINE__, __FILE__, L"the key " + key + L" doesn't exist");
			return NullExpr::createNullExpr();
		}

	}


	std::shared_ptr<Expr> MapExpr::getValue(const std::vector<std::shared_ptr<Expr>>& params) {
		if (params.size() == 1) {
			auto expr = params[0]->getValue();
			if (expr->getType() != TYPE_STRING) {
				quitWithError(__LINE__, __FILE__, L"now only support string as key");
				return NullExpr::createNullExpr();
			}
			return get(std::dynamic_pointer_cast<StringExpr>(expr)->getString());
		
		}
		else if(params.size()==2){
			auto expr = params[0]->getValue();
			if (expr->getType() != TYPE_STRING) {
				quitWithError(__LINE__, __FILE__, L"now only support string as key");
				return NullExpr::createNullExpr();
			}
			return add(std::dynamic_pointer_cast<StringExpr>(expr)->getString(), params[1]);
		}
		else {
			quitWithError(__LINE__, __FILE__, L"can only use one key to retrive from map ");
			return NullExpr::createNullExpr();
		}
	}

	std::wstring MapExpr::toString() {
		std::wostringstream buf;
		buf << Expr::toString()<<L"\n{";
		for (auto& x : map) {
			buf << x.first <<L" : "<< x.second->toString() << L"\n";
		}
		buf << L"}";
		return buf.str();

	}

	 std::shared_ptr<MapExpr> MapExpr::createMapExpr(const std::shared_ptr<Expr>& runtime, const std::unordered_map<KeyType, std::shared_ptr<Expr>>& map) {
		auto ret =  std::make_shared<MapExpr>(runtime, map);
		//=============initialize getter ==================
		ret->getter = FunctionExpr::createFunctionExpr();
		auto closure_getter = ClosureExpr::createClosureExpr();
		closure_getter->addVarable(L"map", ret);
		closure_getter->addExpression(
			ReturnExpr::createReturnExpr(
					(*closure_getter->getVariable(L"map"_variableExpr))({ L"key"_variableExpr })		
			));
		ret->getter->setSignature({ L"key" });
		ret->getter->setClosure(closure_getter);
		//===============initialize setter =================
		ret->setter = FunctionExpr::createFunctionExpr();
		auto closure_setter = ClosureExpr::createClosureExpr();
		closure_setter->addVarable(L"map", ret);
		closure_setter->addExpression(
			ReturnExpr::createReturnExpr(
			(*closure_setter->getVariable(L"map"_variableExpr))({ L"key"_variableExpr,L"value"_variableExpr })
			));
		ret->setter->setSignature({ L"key",L"value" });
		ret->setter->setClosure(closure_setter);
		return ret;
	}
}