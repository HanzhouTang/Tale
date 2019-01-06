#pragma once
#include<iostream>
#include<memory>
#include<string>
#include<sstream>
#include<vector>
#include<initializer_list>
#include<algorithm>
#include<unordered_map>
namespace expr {
	struct VariableExpr;
	struct AssignExpr;
	struct ReverseExpr;
	struct AddExpr;
	struct EqualExpr;
	struct ReturnExpr;
	struct CallExpr;
	struct Expr : std::enable_shared_from_this<Expr> {
		enum ExprType { TYPE_NULL = 0, TYPE_STRING, TYPE_NUMBER, TYPE_VARIABLE, TYPE_BOOLEAN, TYPE_CLOSURE, TYPE_FUNCTION, TYPE_BINARYOPERATION, TYPE_RETURN, TYPE_CONDITION, TYPE_CALL, TYPE_MAP, TYPE_UNARY_OPERATOR,TYPE_ELEMENT_WRAPPER };
		ExprType type;
		std::size_t maximumRecursionDepth() {
			return 1000;
		}
		std::shared_ptr<Expr> runtime;
		static const std::vector<std::wstring> TypeList;
		void setType(ExprType t) { type = t; }
		ExprType getType() { return type; }
		std::shared_ptr<Expr> getRunTime() { return runtime; }
		virtual void setRunTime(const std::shared_ptr<Expr>& r) { runtime = r; }
		Expr(std::shared_ptr<Expr> r) : runtime(r) { setType(TYPE_NULL); }
		virtual ~Expr() {}
		virtual std::shared_ptr<Expr> clone() = 0;
		virtual std::wstring toString() {
			std::wostringstream ret;
			ret << TypeList[getType()] << L"@" << shared_from_this();
			return ret.str();
		}  
		std::wstring getTypeString() {
			return TypeList[getType()];
		}
		virtual std::shared_ptr<Expr> getValue(const std::vector<std::shared_ptr<Expr>>& args) { return shared_from_this(); }
		virtual std::shared_ptr<Expr> getValue() { return shared_from_this(); }
		virtual std::shared_ptr<Expr> getVariable(const std::shared_ptr<VariableExpr>& variable);
		virtual std::shared_ptr<Expr> setVariable(const std::shared_ptr<VariableExpr>& variable, const std::shared_ptr<Expr>& value);
		virtual void store(const std::shared_ptr<Expr>& start);
		virtual void restore(const std::shared_ptr<Expr>& start);
		std::shared_ptr<CallExpr> operator ()(const std::vector<std::shared_ptr<Expr>>&);
	};
}