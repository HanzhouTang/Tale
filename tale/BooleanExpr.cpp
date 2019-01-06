#include"BooleanExpr.h"
namespace expr {
	std::wstring BooleanExpr::toString()
	{
		std::wostringstream ret;
		ret << TypeList[getType()] << L"@" << shared_from_this() << L" value " << (value ? L"true" : L"false");
		return ret.str();
	}
	BooleanExpr::BooleanExpr(const std::shared_ptr<Expr>& runtime, bool v) :
		Expr(runtime), value(v) {
		setType(TYPE_BOOLEAN);
	}
	std::shared_ptr<BooleanExpr> BooleanExpr::createBooleanExpr(const std::shared_ptr<Expr>& runtime, bool value)
	{
		return std::make_shared<BooleanExpr>(runtime, value);
	}
	std::shared_ptr<BooleanExpr> BooleanExpr::createBooleanExpr(bool value)
	{
		return std::make_shared<BooleanExpr>(nullptr, value);
	}
	std::shared_ptr<Expr> BooleanExpr::clone()
	{
		return createBooleanExpr(getRunTime(), value);
	}
	bool BooleanExpr::getBoolValue()
	{
		return value;
	}
}