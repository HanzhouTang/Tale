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
#include"FunctionExpr.h"
#include"ReverseExpr.h"
#include"CallExpr.h"
#include"EqualExpr.h"
#include"LessThanExpr.h"
#include"BooleanExpr.h"
#include"GreaterThanExpr.h"
#include"NotExpr.h"
#include"AndExpr.h"
#include"OrExpr.h"
#include"ConditionExpr.h"
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
		//wcout << "in expr term " << queue.front()->toString() << endl;
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
		using namespace std;
		auto status = lexer.get();
		auto nodes = lexer.lookAheadK(1);
		//wcout << "In moreTerms nodes[0] " << SimpleLexer::getTokenName(nodes[0])<<" lexeme "<<lexer.currentLexeme<< endl;

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

	std::shared_ptr<expr::Expr> SimpleParser::factorHelper() {
		lexer.save();
		auto call = callable();
		if (call->getType() != expr::Expr::TYPE_NULL) {
			lexer.pop();
			return call;
		}
		lexer.restore();
		auto token = lexer.lookAheadK(1)[0];
		if (token == SimpleLexer::Token::Number) {
			match(SimpleLexer::Token::Number);
			auto number = wstr2floats(lexer.currentLexeme);
			return expr::NumberExpr::createNumberExpr(number[0]);
		}
		else if (token == SimpleLexer::Token::Quote) {
			return substr();
		}
		else if (token == SimpleLexer::Token::LParen) {
			match(SimpleLexer::Token::LParen);
			auto Expr = expr();
			match(SimpleLexer::Token::RParen);
			return Expr;
		}
		else if (token == SimpleLexer::Token::Variable) {
			match(SimpleLexer::Token::Variable);
			return expr::VariableExpr::createVariableExpr(lexer.currentLexeme);
		}

		//quitWithError(__LINE__, __FILE__, L"undefined factor");
		return expr::NullExpr::createNullExpr();
	}

	void SimpleParser::factor(std::deque<std::shared_ptr<expr::Expr>>& queue) {
		auto token = lexer.lookAheadK(1)[0];
		if (token == SimpleLexer::Token::Minus) {
			match(SimpleLexer::Token::Minus);
			auto Factor = factorHelper();
			if (Factor->getType() != expr::Expr::TYPE_NULL) {
				queue.push_back(expr::ReverseExpr::createReverseExpr(Factor));
			}
		}
		auto Factor = factorHelper();
		if (Factor->getType() != expr::Expr::TYPE_NULL) {
			queue.push_back(Factor);
		}
	}


	//std::shared_ptr<expr::Expr> SimpleParser::str() {
	//	auto status = lexer.get();
	//	if (strMap.find(status) != strMap.end()) {
	//		auto result = strMap[status];
	//		lexer.set(result.newStatus);
	//		return result.result;
	//	}
	//	auto Str = substr();
	//	if (Str->getType() == expr::Expr::TYPE_NULL) {
	//		auto ret = expr::NullExpr::createNullExpr();
	//		MemoResult result(ret, lexer.get());
	//		strMap[status] = result;
	//		return ret;
	//	}
	//	auto MoreSubStrs = moresubstrs();
	//	if (MoreSubStrs->getType() == expr::Expr::ExprType::TYPE_BINARYOPERATION) {
	//		std::dynamic_pointer_cast<expr::BinaryOperatorExpr>(MoreSubStrs)->setLeft(Str);
	//		MemoResult result(MoreSubStrs, lexer.get());
	//		strMap[status] = result;
	//		return MoreSubStrs;
	//	}
	//	MemoResult result(Str, lexer.get());
	//	strMap[status] = result;
	//	return Str;
	//}

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

	/*std::shared_ptr<expr::Expr> SimpleParser::moresubstrs() {
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
				auto ret = expr::AddExpr::createAddExpr(expr::NullExpr::createNullExpr(), MoreSubStrs);
				MemoResult result(ret, lexer.get());
				moresubstrMap[status] = result;
				return ret;
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
	}*/

	std::shared_ptr<expr::Expr> SimpleParser::element()
	{
		using namespace std;
		auto fs = { &SimpleParser::expr ,&SimpleParser::boolean,
			&SimpleParser::map,&SimpleParser::IF, &SimpleParser::closure,
			&SimpleParser::callable,&SimpleParser::func };
		for (auto f : fs) {
			lexer.save();
			auto x = (this->*f)();
			if (x->getType() != expr::Expr::TYPE_NULL) {
				//wcout << "return element() " << x->toString() << endl;
				lexer.pop();
				return x;
			}
			lexer.restore();
		}
		return expr::NullExpr::createNullExpr();
	}

	std::deque<std::shared_ptr<expr::Expr>> SimpleParser::elementlists()
	{
		auto Element = element();
		if (Element->getType() == expr::Expr::TYPE_NULL) {
			return std::deque<std::shared_ptr<expr::Expr>>();
		}
		auto MoreElements = moreelements();
		MoreElements.emplace_front(Element);
		return MoreElements;
	}

	std::deque<std::shared_ptr<expr::Expr>> SimpleParser::moreelements()
	{
		auto tokens = lexer.lookAheadK(1)[0];
		if (tokens == SimpleLexer::Token::Comma) {
			match(SimpleLexer::Token::Comma);
			auto Element = element();
			auto MoreElements = moreelements();
			MoreElements.emplace_front(Element);
			return MoreElements;
		}
		return std::deque<std::shared_ptr<expr::Expr>>();
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
		auto Str = expr();
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
		// TODO: add type check.
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
		using namespace std;
		auto status = lexer.get();
		if (stateMap.find(status) != stateMap.end()) {
			auto result = stateMap[status];
			lexer.set(result.newStatus);
			return result.result;
		}
		auto fs = { &SimpleParser::assign,&SimpleParser::element };
		for (auto f : fs) {
			lexer.save();
			auto x = (this->*f)();
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
					//wcout << "ret __state__ " << x->toString() << endl;
					lexer.pop();
					MemoResult result(x, lexer.get());
					stateMap.emplace(status, result);
					return x;
				}
			}
			lexer.restore();
		}
		auto x = expr::NullExpr::createNullExpr();
		MemoResult result(x, lexer.get());
		stateMap.emplace(status, result);
		return x;
	}

	std::deque<std::shared_ptr<expr::Expr>> SimpleParser::states() {
		using namespace std;
		auto State = state();
		if (State->getType() == expr::Expr::TYPE_NULL) {
			return std::deque<std::shared_ptr<expr::Expr>>();
		}
		auto ret = states();
		ret.emplace_front(State);
		return ret;

	}

	std::shared_ptr<expr::Expr> SimpleParser::closure() {
		using namespace std;
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
			//wcout << "in closure" << endl;
			//wcout << "States.size "<<States.size() << endl;
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
		lexer.restore();
		auto nullexpr = expr::NullExpr::createNullExpr();
		MemoResult result(nullexpr, lexer.get());
		stateMap.emplace(status, result);
		return nullexpr;
	}

	std::shared_ptr<expr::Expr> SimpleParser::func() {
		using namespace std;
		auto status = lexer.get();
		if (funcMap.find(status) != funcMap.end()) {
			auto result = funcMap[status];
			lexer.set(result.newStatus);
			return result.result;
		}
		auto token = lexer.lookAheadK(1)[0];
		if (token == SimpleLexer::Def) {
			match(SimpleLexer::Def);
			auto FunctionName = funcName();
			match(SimpleLexer::LParen);
			auto parameters = funcParameters();
			match(SimpleLexer::RParen);
			token = lexer.lookAheadK(1)[0];
			while (token == SimpleLexer::Newline) {
				match(SimpleLexer::Newline);
				token = lexer.lookAheadK(1)[0];
			}
			auto Closure = closure();
			auto function = expr::FunctionExpr::createFunctionExpr();
			function->setClosure(std::dynamic_pointer_cast<expr::ClosureExpr>(Closure));
			function->setSignature(std::vector<std::wstring>(parameters.begin(), parameters.end()));
			if (FunctionName != L"") {
				auto variable = expr::VariableExpr::createVariableExpr(FunctionName);
				auto assign = expr::AssignExpr::createAssignExpr(variable, function);
				return assign;
			}
			MemoResult result(function, lexer.get());
			funcMap.emplace(status, result);
			return function;
		}
		else {
			MemoResult result(expr::NullExpr::createNullExpr(), lexer.get());
			funcMap.emplace(status, result);
			return expr::NullExpr::createNullExpr();
		}
	}

	std::wstring SimpleParser::funcName() {
		auto token = lexer.lookAheadK(1)[0];
		if (token == SimpleLexer::Token::Variable) {
			match(SimpleLexer::Token::Variable);
			return lexer.currentLexeme;
		}
		return L"";
	}

	std::deque<std::wstring> SimpleParser::funcParameters() {
		auto token = lexer.lookAheadK(1)[0];
		std::wstring first = L"";
		if (token == SimpleLexer::Token::Variable) {
			match(SimpleLexer::Token::Variable);
			first = lexer.currentLexeme;
		}
		auto parameters = moreFuncParameters();
		if (first != L"") {
			parameters.emplace_front(first);
		}
		return parameters;
	}

	std::deque<std::wstring> SimpleParser::moreFuncParameters() {
		auto token = lexer.lookAheadK(1)[0];
		if (token == SimpleLexer::Token::Comma) {
			match(SimpleLexer::Token::Comma);
			match(SimpleLexer::Token::Variable);
			auto param = lexer.currentLexeme;
			auto MoreParams = moreFuncParameters();
			MoreParams.emplace_front(param);
			return MoreParams;
		}
		return 	std::deque<std::wstring>();
	}

	std::tuple<bool, std::deque<std::shared_ptr<expr::Expr>>> SimpleParser::paramlist()
	{
		auto Token = lexer.lookAheadK(1)[0];
		if (Token == SimpleLexer::Token::LParen) {
			match(SimpleLexer::Token::LParen);
			auto ElementLists = elementlists();
			match(SimpleLexer::Token::RParen);
			return std::make_tuple(true, ElementLists);
		}
		return std::make_tuple(false, std::deque<std::shared_ptr<expr::Expr>>());
	}

	std::shared_ptr<expr::Expr> SimpleParser::compare()
	{
		// need to create template class handle different scenario
		using namespace std;
		auto status = lexer.get();
		if (compareMap.find(status) != compareMap.end()) {
			auto result = compareMap[status];
			lexer.set(result.newStatus);
			return result.result;
		}

		auto funcs = { &SimpleParser::expr };
		std::shared_ptr<expr::Expr> left;
		for (auto f : funcs) {
			lexer.save();
			left = (this->*f)();
			if (left->getType() != expr::Expr::TYPE_NULL) {
				break;
			}
			lexer.restore();
		}
		//wcout << "In compare left " << left->toString() << endl;
		SimpleLexer::Token token;
		if (!left || left->getType() == expr::Expr::TYPE_NULL) {
			left = expr::NullExpr::createNullExpr();
			MemoResult result(left, lexer.get());
			compareMap.emplace(status, result);
			return left;
		}
		token = lexer.lookAheadK(1)[0];
		//wcout << "In compare token " << SimpleLexer::getTokenName(token) << endl;
		if (token == SimpleLexer::EqlEql) {
			match(SimpleLexer::EqlEql);
		}
		else if (token == SimpleLexer::Less) {
			match(SimpleLexer::Less);
		}
		else if (token == SimpleLexer::Greater) {
			match(SimpleLexer::Greater);
		}
		else {
			lexer.restore();
			//throwNotMatchError({ SimpleLexer::EqlEql,SimpleLexer::Less,SimpleLexer::Greater }, token, __LINE__);
			MemoResult result(expr::NullExpr::createNullExpr(), lexer.get());
			compareMap.emplace(status, result);
			return expr::NullExpr::createNullExpr();
		}
		std::shared_ptr<expr::Expr> right;
		for (auto f : funcs) {
			lexer.save();
			right = (this->*f)();
			if (left->getType() != expr::Expr::TYPE_NULL) {
				lexer.pop();
				break;
			}
			lexer.restore();
		}
		//wcout << "In compare right " << right->toString() << endl;
		if (!right || right->getType() == expr::Expr::TYPE_NULL) {
			lexer.restore();// restore from begining. 
			right = expr::NullExpr::createNullExpr();
			MemoResult result(left, lexer.get());
			compareMap.emplace(status, result);
			return right;
		}
		std::shared_ptr<expr::Expr> ret;
		if (token == SimpleLexer::EqlEql) {
			ret = expr::EqualExpr::createEqualExpr(left, right);
		}
		else if (token == SimpleLexer::Less) {
			ret = expr::LessThanExpr::createLessThanExpr(left, right);
		}
		else {
			ret = expr::GreaterThanExpr::createGreaterThanExpr(left, right);
		}
		lexer.pop(); // pop the begin store point;
		MemoResult result(ret, lexer.get());
		compareMap.emplace(status, result);
		return ret;
	}

	std::shared_ptr<expr::Expr> SimpleParser::booleanterm()
	{
		using namespace std;
		auto token = lexer.lookAheadK(1)[0];
		//wcout << L"in booleanterm token: " << SimpleLexer::getTokenName(token)<<endl;
		if (token == SimpleLexer::True) {
			match(SimpleLexer::True);
			return expr::BooleanExpr::createBooleanExpr(true);
		}
		else if (token == SimpleLexer::False) {
			match(SimpleLexer::False);
			return  expr::BooleanExpr::createBooleanExpr(false);
		}
		else if (token == SimpleLexer::Token::Not) {
			match(SimpleLexer::Token::Not);
			return expr::NotExpr::createNotExpr(boolean());
		}
		else if (token == SimpleLexer::Token::LParen) {
			//wcout << "here" << endl;
			match(SimpleLexer::Token::LParen);
			auto ret = boolean();
			match(SimpleLexer::Token::RParen);
			return ret;
		}
		//cout << "before compare" << endl;
		auto cmp = compare();
		//wcout <<"cmp "<< cmp->toString() << endl;
		return cmp;
	}

	std::shared_ptr<expr::Expr> SimpleParser::boolean()
	{
		using namespace std;
		auto Term = booleanterm();
		if (Term->getType() == expr::Expr::TYPE_NULL) {
			return Term;
		}
		auto MoreTerms = morebooleanterms();
		//wcout<<L"More Terms "<< MoreTerms->toString() << endl;
		if (MoreTerms->getType() == expr::Expr::TYPE_BINARYOPERATION) {
			std::dynamic_pointer_cast<expr::BinaryOperatorExpr>(MoreTerms)->setLeft(Term);
			return MoreTerms;
		}
		return Term;
	}

	std::shared_ptr<expr::Expr> SimpleParser::morebooleanterms()
	{
		using namespace std;
		auto token = lexer.lookAheadK(1)[0];
		std::shared_ptr<expr::Expr> first = expr::NullExpr::createNullExpr(),
			second = expr::NullExpr::createNullExpr();
		//wcout << L"in morebooleanterms token: " + SimpleLexer::getTokenName(token) << endl;
		if (token == SimpleLexer::Token::And) {
			match(SimpleLexer::Token::And);
			first = booleanterm();
		}
		else if (token == SimpleLexer::Token::Or) {
			match(SimpleLexer::Token::Or);
			first = booleanterm();
		}
		else {
			return expr::NullExpr::createNullExpr();
		}
		if (first->getType() == expr::Expr::TYPE_NULL) {
			quitWithError(__LINE__, __FILE__, L"need bool type after " + SimpleLexer::getTokenName(token));
		}
		second = morebooleanterms();
		if (second->getType() == expr::Expr::TYPE_BINARYOPERATION) {
			std::dynamic_pointer_cast<expr::BinaryOperatorExpr>(second)->setLeft(first);
			if (token == SimpleLexer::Token::And) {
				return expr::AndExpr::createAndExpr(expr::NullExpr::createNullExpr(), second);
			}
			else if (token == SimpleLexer::Token::Or) {
				return expr::OrExpr::createOrExpr(expr::NullExpr::createNullExpr(), second);
			}
		}
		if (token == SimpleLexer::Token::And) {
			return expr::AndExpr::createAndExpr(expr::NullExpr::createNullExpr(), first);
		}
		else if (token == SimpleLexer::Token::Or) {
			return expr::OrExpr::createOrExpr(expr::NullExpr::createNullExpr(), first);
		}
		return expr::NullExpr::createNullExpr();
	}



	/* function to support f()()()...*/
	std::deque<std::deque<std::shared_ptr<expr::Expr>>> SimpleParser::paramlists() {
		auto Paramlist = paramlist();
		auto existParamlist = std::get<0>(Paramlist);
		if (existParamlist) {
			auto ParamL = std::get<1>(Paramlist);
			auto MoreParamlists = moreparamlists();
			MoreParamlists.emplace_front(ParamL);
			return MoreParamlists;
		}
		//quitWithError(__LINE__, __FILE__, L"function definition must contain (");
		return std::deque<std::deque<std::shared_ptr<expr::Expr>>>();
	}

	std::deque<std::deque<std::shared_ptr<expr::Expr>>> SimpleParser::moreparamlists() {
		using namespace std;
		auto Paramlist = paramlist();
		auto existParamlist = std::get<0>(Paramlist);
		if (existParamlist) {
			auto ParamL = std::get<1>(Paramlist);
			auto MoreParamlists = moreparamlists();
			MoreParamlists.emplace_front(ParamL);
			return MoreParamlists;
		}
		return std::deque<std::deque<std::shared_ptr<expr::Expr>>>();
	}

	std::shared_ptr<expr::Expr> SimpleParser::callable()
	{
		using namespace std;
		auto status = lexer.get();
		if (callableMap.find(status) != callableMap.end()) {
			auto result = callableMap[status];
			lexer.set(result.newStatus);
			return result.result;
		}
		lexer.save();
		auto Callobject = callobject();
		if (Callobject->getType() != expr::Expr::TYPE_NULL) {
			auto ParamLists = paramlists();
		/*	wcout << "size of paramlists: " << ParamLists.size() << endl;*/
			std::shared_ptr<expr::Expr> Call = Callobject;
			if (ParamLists.size() > 0) {
				for (auto ParamList : ParamLists) {
					auto Params = std::vector<std::shared_ptr<expr::Expr>>(ParamList.begin(), ParamList.end());
					Call = expr::CallExpr::createCallExpr(Call, Params);
					/*wcout << "______________________Call________________________" << endl;
					wcout << Call->toString() << endl;*/
				}
				
				lexer.pop();
				MemoResult result(Call, lexer.get());
				callableMap.emplace(status, result);
				return Call;
			}
			else {
				lexer.restore();
				MemoResult result(expr::NullExpr::createNullExpr(), lexer.get());
				callableMap.emplace(status, result);
				return expr::NullExpr::createNullExpr();
			}
		}
		lexer.restore();
		MemoResult result(expr::NullExpr::createNullExpr(), lexer.get());
		callableMap.emplace(status, result);
		return expr::NullExpr::createNullExpr();
	}

	std::shared_ptr<expr::Expr> SimpleParser::callobject()
	{
		using namespace std;
		auto tokens = lexer.lookAheadK(1);
		lexer.save();
		if (tokens[0] == SimpleLexer::Token::Variable) {
			//wcout << "in callobject get varaible as callobject " << endl;
			match(SimpleLexer::Token::Variable);
			auto VariableName = lexer.currentLexeme;
			lexer.pop();
			//wcout << "in callobject variable name " << VariableName << endl;
			return expr::VariableExpr::createVariableExpr(VariableName);
		}

		auto Func = func();
		if (Func->getType() == expr::Expr::TYPE_FUNCTION) {
			lexer.pop();
			return Func;
		}
		lexer.restore();
		return expr::NullExpr::createNullExpr();
	}

}

std::shared_ptr<expr::Expr> parser::SimpleParser::IF()
{
	using namespace std;
	auto token = lexer.lookAheadK(1)[0];
	if (token != SimpleLexer::Token::If) {
		return expr::NullExpr::createNullExpr();
	}
	std::shared_ptr<expr::Expr> condition = expr::NullExpr::createNullExpr();
	std::shared_ptr<expr::Expr> ifStatement = expr::NullExpr::createNullExpr();
	std::shared_ptr<expr::Expr> elseStatement = expr::NullExpr::createNullExpr();
	match(SimpleLexer::Token::If);
	//wcout << "if statement" << endl;
	condition = boolean();
	//wcout << "condition " << condition->toString() << endl;
	if (condition->getType() == expr::Expr::TYPE_NULL) {
		quitWithError(__LINE__, __FILE__, L"if statement without boolean condition");
		return expr::NullExpr::createNullExpr();
	}
	ifStatement = closure();
	//wcout << "ifStatement " << ifStatement->toString() << endl;
	if (ifStatement->getType() == expr::Expr::TYPE_NULL) {
		quitWithError(__LINE__, __FILE__, L"if statement without closure");
		return expr::NullExpr::createNullExpr();
	}
	token = lexer.lookAheadK(1)[0];
	if (token == SimpleLexer::Token::Else) {
		match(SimpleLexer::Token::Else);
		elseStatement = closure();
		if (elseStatement->getType() == expr::Expr::TYPE_NULL) {
			quitWithError(__LINE__, __FILE__, L"else statement without closure");
			return expr::NullExpr::createNullExpr();
		}
	}
	return expr::ConditionExpr::createConditionExpr(condition, ifStatement, elseStatement);
}