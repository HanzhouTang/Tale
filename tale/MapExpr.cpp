#include"MapExpr.h"
#include"Utility.h"
#include"NullExpr.h"
#include"StringExpr.h"
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
	if (params.size() != 1) {
		quitWithError(__LINE__, __FILE__, L"can only use one key to retrive from map ");
		return NullExpr::createNullExpr();
	}
	else {
		auto expr = params[0];
		if (expr->getType() != TYPE_STRING) {
			quitWithError(__LINE__, __FILE__, L"now only support string as key");
			return NullExpr::createNullExpr();
		}
		return get(std::dynamic_pointer_cast<StringExpr>(expr)->getString());
	}
}

std::wstring MapExpr::toString(){
	std::wostringstream buf;
	buf << Expr::toString();
	for (auto& x : map) {
		buf << x.first << x.second->toString() << L"\n";
	}
	return buf.str();
}
