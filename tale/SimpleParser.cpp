#include"SimpleParser.h"
#include"NullExpr.h"
#include"AddExpr.h"
#include"VariableExpr.h"
#include"ArithmeticExpr.h"
#include"NumberExpr.h"
#include"Utility.h"
#include"AddExpr.h"
#include"BinaryOperatorExpr.h"
#include"StringExpr.h"
#include<stack>
using namespace Utility;
namespace parser {
	bool SimpleParser::match(parser::SimpleLexer::Token t) {
		auto token = lexer.getNextToken();
		if (token == t) return true;
		throwError({ t }, token, __LINE__);
		return false;
	}

	void SimpleParser::throwError(const std::vector<parser::SimpleLexer::Token>& expectedToken, parser::SimpleLexer::Token realToken, int lineNumber) {
		std::wostringstream buffer;
		buffer << L"Expected Token type < ";
		for (auto x : expectedToken) {
			buffer << SimpleLexer::getTokenName(x) << " ";
		}
		buffer << L"> however, we got the token < " <<
			SimpleLexer::getTokenName(realToken) << L" >" << std::endl;
		quitWithError(lineNumber, __FILE__, buffer.str());
	}


	std::shared_ptr<expr::Expr> SimpleParser::expr() {
		using namespace std;
		std::deque<std::shared_ptr<expr::Expr>> queue;
		term(queue);
		moreterms(queue);
		std::stack<std::shared_ptr<expr::Expr>> stack;
		for (auto& x : queue) {
			//wcout << x->toString() << endl;
			if (x->getType() != expr::Expr::TYPE_BINARYOPERATION) {
				stack.push(x);
			}
			else {
				auto l = std::dynamic_pointer_cast<expr::BinaryOperatorExpr>(x)->getLeft();
				if (l->getType() == expr::Expr::TYPE_NULL) {
					auto right = stack.top();
					stack.pop();
					auto left = stack.top();
					stack.pop();
					std::dynamic_pointer_cast<expr::BinaryOperatorExpr>(x)->setLeft(left);
					std::dynamic_pointer_cast<expr::BinaryOperatorExpr>(x)->setRight(right);
				}
				stack.push(x);
			}
		}
		if (stack.size() == 1) {
			return stack.top();
		}
		quitWithError(__LINE__, __FILE__, L"stack has more than one element");
	}

	void  SimpleParser::moreterms(std::deque<std::shared_ptr<expr::Expr>>& queue) {
		auto nodes = lexer.lookAheadK(1);
		using namespace std;
		wchar_t oper = 0;
		if (nodes[0] == SimpleLexer::Token::Add) {
			match(SimpleLexer::Token::Add);
			oper = L'+';
		}
		else if (nodes[0] == SimpleLexer::Token::Minus) {
			match(SimpleLexer::Token::Minus);
			oper = L'-';
		}
		if (oper != 0) {
			term(queue);
			queue.push_back(expr::ArithmeticExpr::createArithmeticExpr(expr::NullExpr::createNullExpr(),
				expr::NullExpr::createNullExpr(), oper));
			moreterms(queue);
			return;
		}
	}

	void SimpleParser::term(std::deque<std::shared_ptr<expr::Expr>>& queue) {
		factor(queue);
		morefactors(queue);
	}

	void SimpleParser::morefactors(std::deque<std::shared_ptr<expr::Expr>>& queue) {
		auto tokens = lexer.lookAheadK(1);
		wchar_t oper = 0;
		if (tokens[0] == SimpleLexer::Token::Times) {
			match(SimpleLexer::Token::Times);
			oper = '*';
		}
		else if (tokens[0] == SimpleLexer::Token::Div) {
			match(SimpleLexer::Token::Div);
			oper = '/';
		}

		if (oper != 0) {
			factor(queue);
			queue.push_back(expr::ArithmeticExpr::createArithmeticExpr(expr::NullExpr::createNullExpr(),
				expr::NullExpr::createNullExpr(), oper));
			morefactors(queue);
		}

	}

	void SimpleParser::factor(std::deque<std::shared_ptr<expr::Expr>>& queue) {
		auto token = lexer.getNextToken();
		if (token == SimpleLexer::Token::Number) {
			auto number = wstr2floats(lexer.currentLexeme);
			queue.push_back(expr::NumberExpr::createNumberExpr(number[0]));
			return;
		}
		else if (token == SimpleLexer::Token::LParen) {
			auto Expr = expr();
			match(SimpleLexer::Token::RParen);
			queue.push_back(Expr);
			return;
		}
		else if (token == SimpleLexer::Token::Variable) {
			queue.push_back(expr::VariableExpr::createVariableExpr(lexer.currentLexeme));
			return;
		}
		throwError({ SimpleLexer::Token::Number,SimpleLexer::Token::LParen,
			SimpleLexer::Token::Variable }, token, __LINE__);
	}


	std::shared_ptr<expr::Expr> SimpleParser::str() {
		auto Str = substr();
		auto MoreSubStrs = moresubstrs();
		if (MoreSubStrs->getType() == expr::Expr::ExprType::TYPE_BINARYOPERATION) {
			std::dynamic_pointer_cast<expr::BinaryOperatorExpr>(MoreSubStrs)->setLeft(Str);
			return MoreSubStrs;
		}
		return Str;
	}

	std::shared_ptr<expr::Expr> SimpleParser::substr() {
		auto token = lexer.getNextToken();
		if (token == SimpleLexer::Token::Quote) {
			match(SimpleLexer::Token::String);
			auto str = lexer.currentLexeme;
			match(SimpleLexer::Token::Quote);
			return expr::StringExpr::createStringExpr(str);
		}
		return expr::NullExpr::createNullExpr();
	}

	std::shared_ptr<expr::Expr> SimpleParser::moresubstrs() {
		auto tokens = lexer.lookAheadK(1);
		if (tokens[0] == SimpleLexer::Token::Add) {
			match(SimpleLexer::Token::Add);
			auto SubStr = substr();
			auto MoreSubStrs = moresubstrs();
			if (MoreSubStrs->getType() == expr::Expr::ExprType::TYPE_BINARYOPERATION) {
				std::dynamic_pointer_cast<expr::BinaryOperatorExpr>(MoreSubStrs)->setLeft(SubStr);
				return MoreSubStrs;
			}
			return expr::AddExpr::createAddExpr(expr::NullExpr::createNullExpr(), SubStr);
		}
		return expr::NullExpr::createNullExpr();
	}

	void SimpleParser::init() {
		lexer.init();
	}
}
