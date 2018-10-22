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
#include"AssignExpr.h"
#include"MapExpr.h"
#include"ClosureExpr.h"
#include<stack>
#include<functional>
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
		lexer.save();
		auto x = expr();
		if (x->getType() != expr::Expr::TYPE_NULL) {
			lexer.pop();
			return x;
		}
		x = str();
		if (x->getType() != expr::Expr::TYPE_NULL) {
			lexer.pop();
			return x;
		}
		x = map();
		if (x->getType() != expr::Expr::TYPE_NULL) {
			lexer.pop();
			return x;
		}
		x = closure();
		if (x->getType() != expr::Expr::TYPE_NULL) {
			lexer.pop();
			return x;
		}
		lexer.restore();
		return expr::NullExpr::createNullExpr();
	}

	void SimpleParser::init() {
		lexer.init();
	}

	std::shared_ptr<expr::Expr> SimpleParser::assign() {
		auto status = lexer.get();
		if (assignMap.find(status) != assignMap.end()) {
			auto result = assignMap[status];
			lexer.set(result.newStatus);
			return result.result;
		}
		auto token = lexer.lookAheadK(1)[0];
		if (token != SimpleLexer::Token::Variable) {
			MemoResult result(expr::NullExpr::createNullExpr(), lexer.get());
			assignMap[status] = result;
			return expr::NullExpr::createNullExpr();
		}
		lexer.save();
		match(SimpleLexer::Token::Variable);
		auto variableName = lexer.currentLexeme;
		token = lexer.lookAheadK(1)[0];
		if (token != SimpleLexer::Token::Eql) {
			lexer.restore();
			MemoResult result(expr::NullExpr::createNullExpr(), lexer.get());
			assignMap[status] = result;
			return expr::NullExpr::createNullExpr();
		}
		match(SimpleLexer::Token::Eql);
		auto e = element();
		if (e->getType() == expr::Expr::TYPE_NULL) {
			lexer.restore();
			MemoResult result(expr::NullExpr::createNullExpr(), lexer.get());
			assignMap[status] = result;
			return expr::NullExpr::createNullExpr();
		}
		lexer.pop();
		auto variable = expr::VariableExpr::createVariableExpr(variableName);
		auto ret = expr::AssignExpr::createAssignExpr(variable, e);
		MemoResult result(ret, lexer.get());
		assignMap[status] = result;
		return ret;
	}

	std::shared_ptr<expr::Expr> SimpleParser::map()
	{
		auto status = lexer.get();
		if (mapMap.find(status) != mapMap.end()) {
			auto result = mapMap[status];
			lexer.set(result.newStatus);
			return result.result;
		}
		lexer.save();
		auto token = lexer.lookAheadK(1)[0];
		if (token == SimpleLexer::Token::LBrace) {
			match(SimpleLexer::Token::LBrace);
			auto x = pairs();
			token = lexer.lookAheadK(1)[0];
			if (token == SimpleLexer::Token::RBrace) {
				match(SimpleLexer::Token::RBrace);
				std::unordered_map<expr::MapExpr::KeyType, std::shared_ptr<expr::Expr>> map;
				for (auto pair : x) {
					auto key = std::get<0>(pair);
					auto value = std::get<1>(pair);
					map.emplace(key, value);
				}
				lexer.pop();
				auto ret = expr::MapExpr::createMapExpr(map);
				MemoResult result(ret, lexer.get());
				mapMap[status] = result;
				return ret;
			}
			else {
				lexer.restore();
				auto ret = expr::NullExpr::createNullExpr();
				MemoResult result(ret, lexer.get());
				mapMap[status] = result;
				return ret;
			}

		}
		else {
			lexer.restore();
			auto ret = expr::NullExpr::createNullExpr();
			MemoResult result(ret, lexer.get());
			mapMap[status] = result;
			return ret;
		}
	}

	SimpleParser::Pair SimpleParser::pair()
	{
		lexer.save();
		auto Str = str();
		if (Str->getType() == expr::Expr::TYPE_NULL) {
			lexer.restore();
			return Pair(L"", expr::NullExpr::createNullExpr());
		}
		auto token = lexer.lookAheadK(1)[0];
		if (token != SimpleLexer::Token::Colon) {
			lexer.restore();
			return Pair(L"", expr::NullExpr::createNullExpr());
		}
		match(SimpleLexer::Token::Colon);
		auto value = element();
		if (value->getType() == expr::Expr::TYPE_NULL) {
			lexer.restore();
			return Pair(L"", expr::NullExpr::createNullExpr());
		}
		auto key = std::dynamic_pointer_cast<expr::StringExpr>(Str->getValue())->getString();
		lexer.pop();
		return Pair(key, value);
	}

	std::deque<SimpleParser::Pair> SimpleParser::pairs()
	{
		lexer.save();
		auto Pair = pair();
		auto key = std::get<0>(Pair);
		auto value = std::get<1>(Pair);
		if (key == L"" && value->getType() == expr::Expr::TYPE_NULL) {
			lexer.restore();
			return 	std::deque<SimpleParser::Pair>();
		}
		auto Morepairs = morepairs();
		Morepairs.emplace_back(key, value);
		lexer.pop();
		return Morepairs;
	}

	std::deque<SimpleParser::Pair> SimpleParser::morepairs()
	{
		auto token = lexer.lookAheadK(1)[0];
		if (token == SimpleLexer::Token::Comma) {
			match(SimpleLexer::Token::Comma);
			auto Pair = pair();
			auto key = std::get<0>(Pair);
			auto value = std::get<1>(Pair);
			if (key == L"" && value->getType() == expr::Expr::TYPE_NULL) {
				return std::deque<SimpleParser::Pair>();
			}
			auto Morepairs = morepairs();
			Morepairs.emplace_back(Pair);
			return Morepairs;
		}
		return std::deque<SimpleParser::Pair>();
	}

	std::shared_ptr<expr::Expr> SimpleParser::state() {
		auto status = lexer.get();
		if (stateMap.find(status) != stateMap.end()) {
			auto result = stateMap[status];
			lexer.set(result.newStatus);
			return result.result;
		}
		auto x = assign();
		if (x->getType() != expr::Expr::TYPE_NULL) {
			auto token = lexer.lookAheadK(1)[0];
			bool matching = false;
			if (token == SimpleLexer::Token::Semicolon) {
				match(SimpleLexer::Token::Semicolon);
				matching = true;
			}
			else if (token == SimpleLexer::Token::Newline) {
				match(SimpleLexer::Token::Newline);
				matching = true;
			}
			else if (token == SimpleLexer::Token::EndofContent) {
				matching = true;
			}
			if (matching) {
				MemoResult result(x, lexer.get());
				stateMap.emplace(status, result);
				return x;
			}
		}
		x = element();
		if (x->getType() != expr::Expr::TYPE_NULL) {
			auto token = lexer.lookAheadK(1)[0];
			bool matching = false;
			if (token == SimpleLexer::Token::Semicolon) {
				match(SimpleLexer::Token::Semicolon);
				matching = true;
			}
			else if (token == SimpleLexer::Token::Newline) {
				match(SimpleLexer::Token::Newline);
				matching = true;
			}
			else if (token == SimpleLexer::Token::EndofContent) {
				matching = true;
			}
			if (matching) {
				MemoResult result(x, lexer.get());
				stateMap.emplace(status, result);
				return x;
			}
		}
		x = expr::NullExpr::createNullExpr();
		MemoResult result(x, lexer.get());
		stateMap.emplace(status, result);
		return x;

	}

	std::deque<std::shared_ptr<expr::Expr>> SimpleParser::states() {

		auto State = state();
		if (State->getType() == expr::Expr::TYPE_NULL) {
			return std::deque<std::shared_ptr<expr::Expr>>();
		}
		auto ret = states();
		ret.emplace_front(State);
		return ret;

	}

	std::shared_ptr<expr::Expr> SimpleParser::closure() {
		auto status = lexer.get();
		if (closureMap.find(status) != closureMap.end()) {
			auto result = closureMap[status];
			lexer.set(result.newStatus);
			return result.result;
		}
		lexer.save();
		auto token = lexer.lookAheadK(1)[0];
		if (token == SimpleLexer::Token::LCurlyBrace) {
			match(SimpleLexer::Token::LCurlyBrace);
			auto States = states();
			token = lexer.lookAheadK(1)[0];
			if (token == SimpleLexer::Token::RCurlyBrace) {
				match(SimpleLexer::Token::RCurlyBrace);
				auto closure = expr::ClosureExpr::createClosureExpr();
				for (auto& x : States) {
					closure->addExpression(x);
				}
				lexer.pop();
				MemoResult result(closure, lexer.get());
				stateMap.emplace(status, result);
				return closure;
			}
			else {
				auto nullexpr = expr::NullExpr::createNullExpr();
				lexer.restore();
				MemoResult result(nullexpr, lexer.get());
				stateMap.emplace(status, result);
				return nullexpr;
			}
		}
		lexer.pop();
		auto nullexpr = expr::NullExpr::createNullExpr();
		MemoResult result(nullexpr, lexer.get());
		stateMap.emplace(status, result);
		return nullexpr;
	}
}

