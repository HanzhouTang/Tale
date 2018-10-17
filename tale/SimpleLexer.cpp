#include"SimpleLexer.h"
namespace parser {
	const std::set<std::wstring> SimpleLexer::delimiters =
	{
		L"[",L"]",L",",L"{",L"}",L"*",L"-",L"+",L"/",L"(",L")",L"=",L"<",L">",L";",L"\"",L"\n",L"\"",L" "
	};
	const std::map<std::wstring, SimpleLexer::Token> SimpleLexer::lexeme2token = {
	{L"[",Token::LBrace},{L"]",Token::RBrace},{L",",Token::Comma},{L"{",Token::LCurlyBrace},
	{L"}",Token::RCurlyBrace},{L"*",Token::Times},{L"-",Token::Minus},{L"+",Token::Add},{L"/",Token::Div},
	{L"(",Token::LParen},{L")",Token::RParen},{L"=",Token::Eql},{L"<",Token::Less},{L">",Token::Greater},
	{L";",Token::Smicolon},{L"\"",Token::Quote},{L"\n",Token::Newline} , {L"and",Token::Add},{L"or",Token::Or},
	{L"if",Token::If},{L"else",Token::Else}
	};

	const std::vector<std::wstring> SimpleLexer::TokenNames = {
		L"LBrace",L"RBrace", L"Comma",L"LCurlyBrace",L"RCurlyBrace", L"Times", L"Minus",
		L"Add", L"Div", L"LParen", L"RParen", L"Eql", L"Less", L"Greater", L"And",L"Or", L"Smicolon"
		, L"If", L"Else", L"Quote", L"Invalid", L"Variable",L"Newline", L"EndofContent",L"Number",
		L"String"
	};
	
	SimpleLexer::Token SimpleLexer::getNextToken() {
		using namespace std;
		auto last = content.end();
		if (index1 == last) return Token::EndofContent;
		if (state == State::inString) {
			wstring str;
			for (; index1 != last; index1++) {
				if (*index1 == L'"') break;
				if (*index1 == L'\\') {
					index1++;
				}
				str.push_back(*index1);
			}
			currentLexeme = str;
			//wcout << L"(" << currentLexeme << L")" << endl;
			state = State::endString;
			return Token::String;
		}
		index1 = find_if(index1, last, [&](wchar_t ch) {return !isWhiteSpace(ch); });
		if (index1 == last) return Token::EndofContent;
		index0 = index1;
		index1 = find_if(index0, last, isDelimiter);
		if (index0 == index1) {
			index1++;
		}
		currentLexeme = std::wstring(index0, index1);

		if (currentLexeme == L"\"") {
			if (state == State::endString) {
				state = State::noString;
			}
			else if (state == State::noString) {
				state = State::inString;
			}
		}
		//wcout << L"(" << currentLexeme << L")" << endl;
		if (lexeme2token.count(currentLexeme)) {
			return lexeme2token.find(currentLexeme)->second;
		}
		if (isNumber(currentLexeme)) {
			return Token::Number;
		}
		if (isValidVariable(currentLexeme)) {
			return Token::Variable;
		}
		return Token::Invalid;
	}
	// if no string 

	wchar_t SimpleLexer::peek() {
		return *(index1);
	}

	std::vector<SimpleLexer::Token> SimpleLexer::lookAheadK(int k) {
		auto _state = state;
		auto _index0 = index0;
		auto _index1 = index1;
		auto _currentLexeme = currentLexeme;
		auto _token = token;
		std::vector<Token> tokens;
		for (int i = 0; i < k; i++) {
			auto t = getNextToken();
			tokens.push_back(t);
			if (t == Token::EndofContent) {
				break;
			}
		}
		state = _state;
		index0 = _index0;
		index1 = _index1;
		currentLexeme = _currentLexeme;
		token = _token;
		return tokens;
	}


};

/*
OK
I need a schedule
I need follow the schedule
Otherwise, I can do nothing

*/