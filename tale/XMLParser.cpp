#include"XMLParser.h"
#include<stack>
#include<cassert>
void XMLParser::parse(wstring str) {
	content = str;
	resetLexer();
	Token token = getNextToken();
	while (token != Token::END) {
		wcout << getTokenName(token) << " : " << lexer.currentLexeme << endl;
		token = getNextToken();
	}
}

XMLParser::Token XMLParser::getNextToken() {
	lexer.index0 = lexer.index1;
	Token token = STRING;
	if (lexer.index0 == content.end()) return Token::END;
	if (lexer.state == inString) {
		wstring temp;
		for (; lexer.index1 != content.end() && * (lexer.index1) != CQUOTE && *(lexer.index1) != CDQUOTE; lexer.index1++) {
			if (*(lexer.index1) == L'\\') {
				lexer.index1++;
			}
			temp += *(lexer.index1);
		}
		if (lexer.index1 == content.end()) {
			return Token::INVALID;
		}
		else lexer.currentLexeme = temp;
		lexer.state = endString;
		return Token::STRING;
	}
	lexer.index0 = find_if(lexer.index0, content.end(), isNotWhiteSpace);
	lexer.index1 = find_if(lexer.index0, content.end(), isDelimiter);
	//cout << "index0(" <<*(lexer.index0)<<")"<<" index1("<<*(lexer.index1)<<")"<< endl;
	if (lexer.index1 == content.end()) return Token::END;
	if (lexer.index0 == lexer.index1) {
		//cout << "point to the same character" << endl;
		lexer.index1++;
		switch (*(lexer.index0))
		{
		case CASSIGN:
			token = Token::ASSIGN;
			break;
		case CDQUOTE:
			token = Token::DQUOTE;
			if (lexer.state == noString) {
				lexer.state = inString;
			}
			else if (lexer.state == endString) {
				lexer.state = noString;
			}
			break;
		case CQUOTE:
			token = Token::QUOTE;
			if (lexer.state == noString) {
				lexer.state = inString;
			}
			else if (lexer.state == endString) {
				lexer.state = noString;
			}
			break;
		case CMINUS: 
		{
			auto tempIndex = lexer.index1;
			if (*(lexer.index1) == CMINUS && *(++lexer.index1) == CCLOSE) {
				token = Token::COMENTEND;
				lexer.index1++;
			}
			else {
				lexer.index1 = tempIndex;
				token = Token::MIUNS;
			}
			break;
		}
		case CSLASH: {
		
			if (*(lexer.index1) == CCLOSE) {
				token = Token::TAGENDWITHSLASH;
				lexer.index1++;
			}
			else {
				token = Token::SLASH;
			}
		}
			break;
		case COPEN: {
			auto tempIndex = lexer.index1;
			if (*(lexer.index1) == CSLASH) {
				token = Token::TAGBEGINWITHSLASH;
				lexer.index1++;
			}
			else if (*(lexer.index1) == CEXCLAMATION && *(++lexer.index1) == CMINUS && *(++lexer.index1) == CMINUS) {
				token = Token::COMMENTBEGIN;
				lexer.index1++;
			}
			else {
				token = Token::TAGBEGIN;
				lexer.index1 = tempIndex;
			}
			break; 
		}
		case CCLOSE:
			token = Token::TAGEND;
			break;
		default:
			break;
		}
	}
	lexer.currentLexeme = wstring(lexer.index0, lexer.index1);
	return token;
}

wstring XMLParser::getTokenName(Token token) {
	switch (token)
	{
	case XMLParser::STRING:
		return L"STRING";
	case XMLParser::QUOTE:
		return L"QUOTE";
	case XMLParser::DQUOTE:
		return L"DQUOTE";
	case XMLParser::ASSIGN:
		return L"ASSIGN";
	case XMLParser::TAGBEGIN:
		return L"TAGBEGIN";
	case XMLParser::TAGEND:
		return L"TAGEND";
	case XMLParser::SLASH:
		return L"SLASH";
	case XMLParser::MIUNS:
		return L"MINUS";
	case XMLParser::END:
		return L"END";
	case XMLParser::INVALID:
		return L"INVALID";
	case XMLParser::COMENTEND:
		return L"COMMENTEND";
	case XMLParser::COMMENTBEGIN:
		return L"COMMENTBEGIN";
	case XMLParser::TAGBEGINWITHSLASH:
		return L"TAGBEGINWITHSLASH";
	case XMLParser::TAGENDWITHSLASH:
		return L"TAGENDWITHSLASH";
	default:
		break;
	}
}