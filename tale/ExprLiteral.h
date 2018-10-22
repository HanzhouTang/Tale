#pragma once
#include"Expr.h"
namespace expr {
	struct AssignExpr;
	struct ReverseExpr;
	struct AddExpr;
	struct EqualExpr;
	struct VariableExpr;
	struct StringExpr;
	struct NumberExpr;
	std::shared_ptr<AssignExpr> operator <<(const std::shared_ptr<VariableExpr>&, const std::shared_ptr<Expr>&);
	std::shared_ptr<ReverseExpr> operator -(const std::shared_ptr<Expr>& expr);
	std::shared_ptr<AddExpr> operator + (const std::shared_ptr<Expr>& left, const std::shared_ptr<Expr>& right);
	std::shared_ptr<AddExpr> operator -(const std::shared_ptr<Expr>& left, const std::shared_ptr<Expr>& right);
	std::shared_ptr<EqualExpr> operator <<=(const std::shared_ptr<Expr>& left, const std::shared_ptr<Expr>& right);
	std::shared_ptr<StringExpr> operator ""_expr(const wchar_t* value, std::size_t);
	std::shared_ptr<VariableExpr> operator ""_variableExpr(const wchar_t* value, std::size_t);
	std::shared_ptr<NumberExpr> operator ""_expr(long double value);
	std::shared_ptr<NumberExpr> operator ""_expr(unsigned long long int value);
	std::wostream& operator<<(std::wostream& os, std::shared_ptr<expr::Expr>);
}