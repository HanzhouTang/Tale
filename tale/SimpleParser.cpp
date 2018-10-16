#include"SimpleParser.h"
#include"NullExpr.h"
#include"AddExpr.h"
#include"VariableExpr.h"
#include"ArithmeticExpr.h"
#include"NumberExpr.h"
#include"Utility.h"
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
		auto Term = term();
		auto MoreTerms = moreterms();
		if (MoreTerms->getType() != expr::Expr::TYPE_NULL) {
			std::dynamic_pointer_cast<expr::ArithmeticExpr>(MoreTerms)->setLeft(Term);
			return MoreTerms;
		}
		return Term;
	}

	std::shared_ptr<expr::Expr> SimpleParser::moreterms() {
		auto nodes = lexer.lookAheadK(1);
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
			auto Term = term();
			auto MoreTerms = moreterms();
			if (MoreTerms->getType() != expr::Expr::TYPE_NULL) {
				auto ret = std::dynamic_pointer_cast<expr::ArithmeticExpr>(MoreTerms)->setLeft(Term);
				return expr::ArithmeticExpr::createArithmeticExpr(expr::NullExpr::createNullExpr(), ret, oper);
			}
			auto ret = expr::ArithmeticExpr::createArithmeticExpr(expr::NullExpr::createNullExpr(), Term, oper);
			return ret;
		}
		return expr::NullExpr::createNullExpr();
	}

	std::shared_ptr<expr::Expr> SimpleParser::term() {
		auto Factor = factor();
		auto MoreFactor = morefactors();
		if (MoreFactor->getType() != expr::Expr::TYPE_NULL) {
			std::dynamic_pointer_cast<expr::ArithmeticExpr>(MoreFactor)->setLeft(Factor);
			return MoreFactor;
		}
		return Factor;
	}

	std::shared_ptr<expr::Expr> SimpleParser::morefactors() {
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
			auto Factor = factor();
			auto MoreFactors = morefactors();
			if (MoreFactors->getType() != expr::Expr::TYPE_NULL) {
				auto ret = std::dynamic_pointer_cast<expr::ArithmeticExpr>(MoreFactors)->setLeft(Factor);
				return expr::ArithmeticExpr::createArithmeticExpr(expr::NullExpr::createNullExpr(), ret, oper);
			}
			return expr::ArithmeticExpr::createArithmeticExpr(expr::NullExpr::createNullExpr(), Factor, oper);
		}

		return expr::NullExpr::createNullExpr();

	}

	std::shared_ptr<expr::Expr> SimpleParser::factor() {
		auto token = lexer.getNextToken();
		if (token == SimpleLexer::Token::Number) {
			auto number = wstr2floats(lexer.currentLexeme);
			return expr::NumberExpr::createNumberExpr(number[0]);
		}
		else if (token == SimpleLexer::Token::LParen) {
			auto ret = expr();
			match(SimpleLexer::Token::RParen);
			return ret;
		}
		else if (token == SimpleLexer::Token::Variable) {
			return expr::VariableExpr::createVariableExpr(lexer.currentLexeme);
		}
		throwError({ SimpleLexer::Token::Number,SimpleLexer::Token::LParen,
			SimpleLexer::Token::Variable }, token, __LINE__);
	}

	void SimpleParser::init() {
		lexer.init();
	}
}
