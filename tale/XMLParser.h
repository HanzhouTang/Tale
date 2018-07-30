#pragma once
#include"Node.h"
#include"Attribute.h"
using namespace std;
class XMLParser {
	wstring content;
	shared_ptr<Node> root;
public:
	enum Token{STRING,QUOTE,DQUOTE,ASSIGN,TAGBEGIN,TAGEND,TAGSLASHBEIGN,TAGSLASHEND,COMMENTBEGIN,COMMENTEND,END};
	enum State{insideTag,outsideTag};
	struct Lexer {
		wstring::iterator index0, index1;
		wstring currentToken;
		Token token;
		State state;
	};
	Lexer lexer;
	void parse(wstring str);
	Token getNextToken();
	void resetLexer() { 
		lexer.index0 = lexer.index1 = content.begin(); 
		lexer.state = outsideTag;
	}
	const static wchar_t COPEN = L'<';
	const static wchar_t CCLOSE = L'>';
	const static wchar_t CSLASH = L'/';
	const static wchar_t CSPACE = L' ';
	const static wchar_t CQUOTE = L'\'';
	const static wchar_t CDQUOTE = L'\"';
	const static wchar_t CTAB = L'\t';
	const static wchar_t CNEWLINE = L'\n';
	const static wchar_t CEXCLAMATION = L'!';
	const static wchar_t CMINUS = L'-';
};