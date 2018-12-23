#pragma once
#include"BinaryOperatorExpr.h"
namespace expr {
	struct ArithmeticExpr :BinaryOperatorExpr {
		wchar_t op;
		std::shared_ptr<Expr> getValue() override;
		std::shared_ptr<Expr> getValueFromTwoNumber(const std::shared_ptr<Expr>& l, const std::shared_ptr<Expr>& r);
		std::shared_ptr<Expr> getValueFromTwoString(const std::shared_ptr<Expr>& l, const std::shared_ptr<Expr>& r);
		virtual std::wstring toString() override {
			std::wostringstream ret;
			ret << L"(" << getLeft()->toString() << L") ";
			ret << op<<L" (" << getRight()->toString() << L")";
			return ret.str();
		}

		ArithmeticExpr(const std::shared_ptr<Expr>& runtime, const std::shared_ptr<Expr>& l, const std::shared_ptr<Expr>& r,wchar_t OP)
			:BinaryOperatorExpr(runtime, l, r), op(OP){
			setType(TYPE_BINARYOPERATION);
		}

		static std::shared_ptr<ArithmeticExpr> createArithmeticExpr(const std::shared_ptr<Expr>& runtime, \
			const std::shared_ptr<Expr>& left, const std::shared_ptr<Expr>& right,wchar_t op) {
			using namespace std;
			auto ret = std::make_shared<ArithmeticExpr>(runtime, left, right,op);
			ret->getLeft()->setRunTime(ret);
			ret->getRight()->setRunTime(ret);
			return ret;
		}


		static std::shared_ptr<ArithmeticExpr> createArithmeticExpr(const std::shared_ptr<Expr>& left, const std::shared_ptr<Expr>& right,wchar_t op) {
			return createArithmeticExpr(nullptr, left, right,op);
		}

		virtual std::shared_ptr<Expr> clone() override {
			auto ret = createArithmeticExpr(runtime, getLeft()->clone(), getRight()->clone(),op);
			ret->getLeft()->setRunTime(ret);
			ret->getRight()->setRunTime(ret);
			return ret;
		}
	};
}