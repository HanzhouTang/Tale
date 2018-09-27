#include"SimpleLexer.h"
namespace parser {
	const std::set<std::wstring> SimpleLexer::delimiters =
	{
		L"[",L"]",L",",L"{",L"}",L"*",L"-",L"+",L"/",L"(",L")",L"=",L"<",L">",L";",L"\"",L"\n"
	};
	const std::map<std::wstring, SimpleLexer::Token> SimpleLexer::lexeme2token = {
	{L"[",Token::LBrace},{L"]",Token::RBrace},{L",",Token::Comma},{L"{",Token::LCurlyBrace},
	{L"}",Token::RCurlyBrace},{L"*",Token::Times},{L"-",Token::Minus},{L"+",Token::Add},{L"/",Token::Div},
	{L"(",Token::LParen},{L")",Token::RParen},{L"=",Token::Eql},{L"<",Token::Less},{L">",Token::Greater},
	{L";",Token::Smicolon},{L"\"",Token::Quote},{L"\n",Token::Newline} , {L"and",Token::Add},{L"or",Token::Or},
	{L"if",Token::If},{L"else",Token::Else}
	};

	SimpleLexer::Token SimpleLexer::getNextToken() {
		using namespace std;
		auto last = content.end();
		/*
		if in a string 
		*/
		if (index1 == last) return Token::EndofContent;
		index1 = find_if(index1, last, [&](wchar_t ch) {return !isWhiteSpace(ch); });
		if (index1 == last) return Token::EndofContent;
		index0 = index1;
		index1 = find_if(index0, last, isDelimiter);
		currentLexeme = std::wstring(index0, index1);
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

};

/*
OK
I need a schedule
I need follow the schedule
Otherwise, I can do nothing

*/