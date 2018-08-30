#include"Libarary.h"
#include"NullExpr.h"
namespace expr {
	std::shared_ptr<Expr> printExpr(const std::vector <std::shared_ptr<Expr>>& args)
	{
		for (auto& x : args) {
			std::wcout << x->toString() << ",\t";
		}
		std::wcout << std::endl;
		return NullExpr::createNullExpr();
	}
}