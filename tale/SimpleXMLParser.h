#pragma once
#include"Node.h"
using namespace std;
class SimpleXMLParser {
	wstring content;
	shared_ptr<Node> root;
public:
	enum Token{STRING,QUOTE,DQUOTE,ASSIGN,TAGBEGIN,TAGEND,
		SLASH,MIUNS,END,INVALID,COMENTEND,COMMENTBEGIN,TAGENDWITHSLASH,TAGBEGINWITHSLASH};
	enum State{inString,noString,endString};
	struct Lexer {
		wstring::iterator index0, index1;
		wstring currentLexeme;
		Token token;
		State state;
	};
	Lexer lexer;
	void parse(wstring str);
	Token getNextToken();
	void resetLexer() { 
		lexer.index0 = lexer.index1 = content.begin(); 
		lexer.state = noString;
	}
	static bool isDelimiter(wchar_t ch) {
		return ch == CASSIGN || isWhiteSpace(ch)||ch == CQUOTE||
			ch == CDQUOTE||ch==CSLASH||ch==COPEN||ch==CCLOSE||ch==CMINUS;
	}
	static bool isNotWhiteSpace(wchar_t ch) {
		return !isWhiteSpace(ch);
	}
	static bool isWhiteSpace(wchar_t ch) {
		return  ch == CSPACE || ch == CTAB || ch == CNEWLINE||ch==CRETURN;
	}
	wstring getTokenName(Token token);
	const static wchar_t COPEN = L'<';
	const static wchar_t CCLOSE = L'>';
	const static wchar_t CSLASH = L'/';
	const static wchar_t CSPACE = L' ';
	const static wchar_t CQUOTE = L'\'';
	const static wchar_t CDQUOTE = L'\"';
	const static wchar_t CTAB = L'\t';
	const static wchar_t CASSIGN = L'=';
	const static wchar_t CNEWLINE = 10;
	const static wchar_t CEXCLAMATION = L'!';
	const static wchar_t CMINUS = L'-';
	const static wchar_t CRETURN = 13;
};