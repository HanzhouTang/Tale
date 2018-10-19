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
		throwNotMatchError({ t }, token, __LINE__);
		return false;
	}

	void SimpleParser::throwNotMatchError(const std::vector<parser::SimpleLexer::Token>& expectedToken, parser::SimpleLexer::Token realToken, int lineNumber) {
		std::wostringstream buffer;
		buffer << L"Expected Token type < ";
		for (auto x : expectedToken) {
			buffer << SimpleLexer::getTokenName(x) << " ";
		}
		buffer << L"> however, we got the token < " <<
			SimpleLexer::getTokenName(realToken) << L" >" << std::endl;
		quitWithError(lineNumber, __FILE__, buffer.str());
	}


	//when is the best time to clear map?
	std::shared_ptr<expr::Expr> SimpleParser::expr() {
		using namespace std;
		auto status = lexer.get();
		if (exprMap.find(status) != exprMap.end()) {
			auto after = exprMap[status];
			lexer.set(after.newStatus);
			return after.result;
		}
		std::deque<std::shared_ptr<expr::Expr>> queue;
		term(queue);
		if (queue.size() == 0) {
			MemoResult result(expr::NullExpr::createNullExpr(), lexer.get());
			exprMap[status] = result;
			return expr::NullExpr::createNullExpr();
		}
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
			MemoResult result(stack.top(), lexer.get());
			exprMap[status] = result;
			return stack.top();
		}
		quitWithError(__LINE__, __FILE__, L"stack has more than one element");
	}

	void  SimpleParser::moreterms(std::deque<std::shared_ptr<expr::Expr>>& queue) {
		auto status = lexer.get();
		auto nodes = lexer.lookAheadK(1);
		using namespace std;
		wchar_t oper = 0;
		if (nodes[0] == SimpleLexer::Token::Add) {
			lexer.save();
			match(SimpleLexer::Token::Add);
			oper = L'+';
		}
		else if (nodes[0] == SimpleLexer::Token::Minus) {
			lexer.save();
			match(SimpleLexer::Token::Minus);
			oper = L'-';
		}
		auto size = queue.size();
		if (oper != 0) {
			term(queue);
			if (size != queue.size()) {
				lexer.pop();
				queue.push_back(expr::ArithmeticExpr::createArithmeticExpr(expr::NullExpr::createNullExpr(),
					expr::NullExpr::createNullExpr(), oper));
				moreterms(queue);
			}
			else { lexer.restore(); }
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
		auto token = lexer.lookAheadK(1)[0];
		if (token == SimpleLexer::Token::Number) {
			match(SimpleLexer::Token::Number);
			auto number = wstr2floats(lexer.currentLexeme);
			queue.push_back(expr::NumberExpr::createNumberExpr(number[0]));
			return;
		}
		else if (token == SimpleLexer::Token::LParen) {
			match(SimpleLexer::Token::LParen);
			auto Expr = expr();
			match(SimpleLexer::Token::RParen);
			queue.push_back(Expr);
			return;
		}
		else if (token == SimpleLexer::Token::Variable) {
			match(SimpleLexer::Token::Variable);
			queue.push_back(expr::VariableExpr::createVariableExpr(lexer.currentLexeme));
			return;
		}
		/*throwNotMatchError({ SimpleLexer::Token::Number,SimpleLexer::Token::LParen,
			SimpleLexer::Token::Variable }, token, __LINE__);
		*/
	}


	std::shared_ptr<expr::Expr> SimpleParser::str() {
		auto status = lexer.get();
		if (strMap.find(status) != strMap.end()) {
			auto result = strMap[status];
			lexer.set(result.newStatus);
			return result.result;
		}
		auto Str = substr();
		if (Str->getType() == expr::Expr::TYPE_NULL) {
			auto ret = expr::NullExpr::createNullExpr();
			MemoResult result(ret, lexer.get());
			strMap[status] = result;
			return ret;
		}
		auto MoreSubStrs = moresubstrs();
		if (MoreSubStrs->getType() == expr::Expr::ExprType::TYPE_BINARYOPERATION) {
			std::dynamic_pointer_cast<expr::BinaryOperatorExpr>(MoreSubStrs)->setLeft(Str);
			MemoResult result(MoreSubStrs, lexer.get());
			strMap[status] = result;
			return MoreSubStrs;
		}
		MemoResult result(Str, lexer.get());
		strMap[status] = result;
		return Str;
	}

	std::shared_ptr<expr::Expr> SimpleParser::substr() {
		auto status = lexer.get();
		if (substrMap.find(status) != substrMap.end()) {
			auto result = substrMap[status];
			lexer.set(result.newStatus);
			return result.result;
		}
		auto token = lexer.lookAheadK(1)[0];
		if (token == SimpleLexer::Token::Quote) {
			match(SimpleLexer::Token::Quote);
			match(SimpleLexer::Token::String);
			auto str = lexer.currentLexeme;
			match(SimpleLexer::Token::Quote);
			auto ret = expr::StringExpr::createStringExpr(str);
			MemoResult result(ret, lexer.get());
			substrMap[status] = result;
			return ret;
		}
		MemoResult result(expr::NullExpr::createNullExpr(), lexer.get());
		substrMap[status] = result;
		return expr::NullExpr::createNullExpr();
	}

	std::shared_ptr<expr::Expr> SimpleParser::moresubstrs() {
		auto status = lexer.get();
		if (moresubstrMap.find(status) != moresubstrMap.end()) {
			auto result = moresubstrMap[status];
			lexer.set(result.newStatus);
			return result.result;
		}
		auto tokens = lexer.lookAheadK(1);
		if (tokens[0] == SimpleLexer::Token::Add) {
			lexer.save();
			match(SimpleLexer::Token::Add);
			auto SubStr = substr();
			auto MoreSubStrs = moresubstrs();
			if (MoreSubStrs->getType() == expr::Expr::ExprType::TYPE_BINARYOPERATION) {
				std::dynamic_pointer_cast<expr::BinaryOperatorExpr>(MoreSubStrs)->setLeft(SubStr);
				MemoResult result(MoreSubStrs, lexer.get());
				moresubstrMap[status] = result;
				return MoreSubStrs;
			}

			else if (SubStr->getType() != expr::Expr::TYPE_NULL) {
				lexer.pop();
				auto ret = expr::AddExpr::createAddExpr(expr::NullExpr::createNullExpr(), SubStr);
				MemoResult result(ret, lexer.get());
				moresubstrMap[status] = result;
				return ret;
			}

			else {
				lexer.restore();
				MemoResult result(expr::NullExpr::createNullExpr(), lexer.get());
				moresubstrMap[status] = result;
				return expr::NullExpr::createNullExpr();
			}
		}
		MemoResult result(expr::NullExpr::createNullExpr(), lexer.get());
		moresubstrMap[status] = result;
		return expr::NullExpr::createNullExpr();
	}

	std::shared_ptr<expr::Expr> SimpleParser::element()
	{
		auto x = expr();
		if (x->getType() != expr::Expr::TYPE_NULL) {
			return x;
		}
		return str();
	}

	void SimpleParser::init() {
		lexer.init();
	}
}
