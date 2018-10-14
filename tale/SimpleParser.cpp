#include"SimpleParser.h"
#include"Utility.h"
using namespace Utility;
namespace parser {
	bool SimpleParser::match(parser::SimpleLexer::Token t) {
		auto token = lexer.getNextToken();
		if (token == t) return true;
		throwError(t, token);
		return false;
	}

	void SimpleParser::throwError(parser::SimpleLexer::Token expectedToken, parser::SimpleLexer::Token realToken) {
		std::wostringstream buffer;
		buffer << L"Expected Token type <" << SimpleLexer::getTokenName(expectedToken)
			<< L"> however, we got the token <" <<
			SimpleLexer::getTokenName(realToken) << L">" << std::endl;
		quitWithError(__LINE__, __FILE__, buffer.str());
	}

	std::shared_ptr<expr::Expr> SimpleParser::parentheses() {
		match(SimpleLexer::Token::LParen);
		auto ret = expr();
		match(SimpleLexer::Token::RParen);
		return ret;
	}

	std::shared_ptr<expr::NumberExpr> SimpleParser::number() {
		auto token = lexer.getNextToken();
		if (token == SimpleLexer::Token::Number) {
			auto number = lexer.currentLexeme;
			auto x = wstr2floats(number)[0];
			auto numberExpr = expr::NumberExpr::createNumberExpr(x);
			return numberExpr;
		}
		else
			throwError(SimpleLexer::Token::Number, token);
	}

	std::shared_ptr<expr::AddExpr> SimpleParser::add()
	{
		auto left = expr();
		match(SimpleLexer::Token::Add);
		auto right = expr();
		return expr::AddExpr::createAddExpr(left, right);
	}

	std::shared_ptr<expr::ArithmeticExpr> SimpleParser::div()
	{
		auto left = expr();
		match(SimpleLexer::Token::Div);
		auto right = expr();
		return expr::ArithmeticExpr::createArithmeticExpr(left, right, L'/');
	}

	std::shared_ptr<expr::ArithmeticExpr> SimpleParser::minus() {
		auto left = expr();
		match(SimpleLexer::Token::Minus);
		auto right = expr();
		return expr::ArithmeticExpr::createArithmeticExpr(left, right, L'-');
	}

	std::shared_ptr<expr::ArithmeticExpr> SimpleParser::times()
	{
		auto left = expr();
		match(SimpleLexer::Token::Times);
		auto right = expr();
		return expr::ArithmeticExpr::createArithmeticExpr(left, right, L'*');
	}

	std::shared_ptr<expr::Expr> SimpleParser::expr() {
		auto tokens = lexer.lookAheadK(2);
		std::shared_ptr<expr::Expr> ret;
		if (tokens[0] == SimpleLexer::LParen) {
			ret = parentheses();
		}
		else if (tokens[0] == SimpleLexer::Number) {
			ret = number();
		}
		else if (tokens[1] == SimpleLexer::Variable) {
			ret = id();
		}
		return ret;
	}

	std::shared_ptr<expr::VariableExpr> SimpleParser::id() {
		auto token = lexer.getNextToken();
		if (token == SimpleLexer::Token::Variable) {
			auto name = lexer.currentLexeme;
			auto variable = expr::VariableExpr::createVariableExpr(name);
			return variable;
		}
		else
			throwError(SimpleLexer::Token::Variable, token);
	}
	
	void SimpleParser::init() {
		lexer.init();
	}
}
