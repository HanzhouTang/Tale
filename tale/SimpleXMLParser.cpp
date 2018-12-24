#include"SimpleXMLParser.h"
#include"Utility.h"
#include<stack>
#include<cassert>
namespace xml {
	void SimpleXMLParser::parse(std::wstring str) {
		content = str;
		resetLexer();
		this->root = Node::createNode(L"_root", nullptr);
		auto currentNode = this->root;
		Token token = getNextToken();
		bool insideTag = false;
		std::stack<std::wstring> trace;
		while (token != Token::END) {

			switch (token)
			{
			case TAGBEGIN:
				insideTag = true;
				token = getNextToken();
				if (token != STRING) {
					Utility::quitWithError(__LINE__, __FILE__, L"tag must contain its' name");
				}
				else {
					auto node = Node::createNode(lexer.currentLexeme, currentNode);
					trace.push(lexer.currentLexeme);
					currentNode->addChild(node);
					currentNode = node;
				}
				break;
			case TAGBEGINWITHSLASH:
				insideTag = true;
				token = getNextToken();
				if (token != STRING) {
					Utility::quitWithError(__LINE__, __FILE__, L"tag must contain its' name");
				}
				else {
					if (trace.top() != lexer.currentLexeme) {

						Utility::quitWithError(__LINE__, __FILE__, L" ending tag " + lexer.currentLexeme + L" doesn't match starting tag");
					}
					//wcout << "token " << lexer.currentLexeme << " is pop out" << endl;
					trace.pop();
					currentNode = currentNode->getHighLevel();
					token = getNextToken();
					if (token != TAGEND) {
						Utility::quitWithError(__LINE__, __FILE__, L"closing tags cannot contain any attribute");
					}
					else {
						insideTag = false;
					}
				}
				break;
			case TAGEND:
				insideTag = false;
				break;
			case TAGENDWITHSLASH:
				insideTag = false;
				trace.pop();
				currentNode = currentNode->getHighLevel();
				break;
			case STRING:
				if (!insideTag) {
					currentNode->appendValue(lexer.currentLexeme);
				}
				else {
					// temporarily not support signle attribute. Because it's not necessary now.
					auto key = lexer.currentLexeme;
					token = getNextToken();
					if (token != ASSIGN) {
						Utility::quitWithError(__LINE__, __FILE__, L"now we don't support signle attribute. it musht be a pair");
					}
					else {
						token = getNextToken();
						if (token == Token::QUOTE) {
							token = getNextToken();
							currentNode->setAttribute(key, lexer.currentLexeme);
							token = getNextToken();
							if (token != QUOTE) {
								Utility::quitWithError(__LINE__, __FILE__, L" \' must be ended by \'");
							}
						}
						else if (token == Token::DQUOTE) {
							token = getNextToken();
							currentNode->setAttribute(key, lexer.currentLexeme);
							token = getNextToken();
							if (token != DQUOTE) {
								Utility::quitWithError(__LINE__, __FILE__, L"\" must be ended by \"");
							}
						}
						else {
							Utility::quitWithError(__LINE__, __FILE__, L"the one fllowing = must be covered by \' or \"");
						}
					}

				}
				break;
			case COMMENTBEGIN:
				do {
					token = getNextToken();
				} while (token != COMENTEND && token != END);
				if (token == END) {
					Utility::quitWithError(__LINE__, __FILE__, L" the comment doesn't has a ending");
				}
				break;
			}

			token = getNextToken();
			//wcout << getTokenName(token) << " : " << lexer.currentLexeme << endl;
		}
		//wcout << currentNode->info();
	}

	SimpleXMLParser::Token SimpleXMLParser::getNextToken() {
		lexer.index0 = lexer.index1;
		Token token = STRING;
		if (lexer.index0 == content.end()) return Token::END;
		if (lexer.state == inString) {
			std::wstring temp;
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
		lexer.currentLexeme = std::wstring(lexer.index0, lexer.index1);
		return token;
	}

	std::wstring SimpleXMLParser::getTokenName(Token token) {
		switch (token)
		{
		case SimpleXMLParser::STRING:
			return L"STRING";
		case SimpleXMLParser::QUOTE:
			return L"QUOTE";
		case SimpleXMLParser::DQUOTE:
			return L"DQUOTE";
		case SimpleXMLParser::ASSIGN:
			return L"ASSIGN";
		case SimpleXMLParser::TAGBEGIN:
			return L"TAGBEGIN";
		case SimpleXMLParser::TAGEND:
			return L"TAGEND";
		case SimpleXMLParser::SLASH:
			return L"SLASH";
		case SimpleXMLParser::MIUNS:
			return L"MINUS";
		case SimpleXMLParser::END:
			return L"END";
		case SimpleXMLParser::INVALID:
			return L"INVALID";
		case SimpleXMLParser::COMENTEND:
			return L"COMMENTEND";
		case SimpleXMLParser::COMMENTBEGIN:
			return L"COMMENTBEGIN";
		case SimpleXMLParser::TAGBEGINWITHSLASH:
			return L"TAGBEGINWITHSLASH";
		case SimpleXMLParser::TAGENDWITHSLASH:
			return L"TAGENDWITHSLASH";
		default:
			return L"DEFAULT";
			break;
		}
	}
}

