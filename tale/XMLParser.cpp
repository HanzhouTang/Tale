#include"XMLParser.h"
#include<stack>
#include<cassert>
void XMLParser::parse(wstring str) {
	content = str;
	resetLexer();
	Token token = getNextToken();
	while (token != Token::END) {
		wcout << getTokenName(token)<<" : "<<lexer.currentLexeme<< endl;
		token = getNextToken();
	}
}

XMLParser::Token XMLParser::getNextToken() {
	lexer.index0 = lexer.index1;
	Token token = STRING;
	if (lexer.index0 == content.end()) return Token::END;
	if (lexer.state == inString) {
		wstring temp;
		for (; lexer.index1 != content.end() && * (lexer.index1) != CQUOTE && *(lexer.index1) != CDQUOTE;lexer.index1++) {
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
	if (lexer.index0 == lexer.index1) {
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
			token = Token::MIUNS;
			break;
		case CSLASH:
			token = Token::SLASH;
			break;
		case COPEN:
			token = Token::TAGBEGIN;
			break;
		case CCLOSE:
			token = Token::TAGEND;
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
		break;
	case XMLParser::QUOTE:
		return L"QUOTE";
		break;
	case XMLParser::DQUOTE:
		return L"DQUOTE";
		break;
	case XMLParser::ASSIGN:
		return L"ASSIGN";
		break;
	case XMLParser::TAGBEGIN:
		return L"TAGBEGIN";
		break;
	case XMLParser::TAGEND:
		return L"TAGEND";
		break;
	case XMLParser::SLASH:
		return L"SLASH";
		break;
	case XMLParser::MIUNS:
		return L"MINUS";
		break;
	case XMLParser::END:
		return L"END";
		break;
	case XMLParser::INVALID:
		return L"INVALID";
	default:
		break;
	}
}