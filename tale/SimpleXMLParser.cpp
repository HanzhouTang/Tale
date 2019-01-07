#include"SimpleXMLParser.h"
#include"Utility.h"
#include<stack>
#include<cassert>
#include<set>
namespace xml {
	void SimpleXMLParser::parse(std::wstring str) {
		using namespace std;
		content = str;
		resetLexer();
		this->root = Node::createNode(L"_root", nullptr);
		auto currentNode = this->root;
		Token token = getNextToken();
		std::set<std::wstring> ids;
		std::stack<std::wstring> trace;
		while (token != Token::END) {

			switch (token)
			{
			case TAGBEGIN:
				if (insideTag) {
					Utility::quitWithError(__LINE__, __FILE__, L"cannot contain a tag in another tag");
				}
				else {
					if (trace.size() > 1 && trace.top() == Utility::SCRIPT) {
						currentNode->appendValue(lexer.currentLexeme);
					}
					else {
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
					}
				}
				break;
			case TAGBEGINWITHSLASH:
				if (insideTag) {
					Utility::quitWithError(__LINE__, __FILE__, L"cannot contain a tag in another tag");
				}
				insideTag = true;
				token = getNextToken();
				if (token != STRING) {
					Utility::quitWithError(__LINE__, __FILE__, L"tag must contain its' name");
				}
				else {
					if (trace.top() != lexer.currentLexeme) {

						Utility::quitWithError(__LINE__, __FILE__, L" ending tag " + lexer.currentLexeme + L" doesn't match starting tag");
					}
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
					if (token == COMMENTBEGIN) {
						handleComment();
						token = getNextToken();
					}
					if (token != ASSIGN) {
						Utility::quitWithError(__LINE__, __FILE__, L"now we don't support single attribute. it musht be a pair");
					}
					else {
						token = getNextToken();
						if (token == COMMENTBEGIN) {
							handleComment();
							token = getNextToken();
						}
						if (token == Token::QUOTE) {
							token = getNextToken();
							if (key == Utility::ID) {
								if (ids.count(lexer.currentLexeme)) {
									Utility::warning(L"id " + lexer.currentLexeme + L" already existed");
								}
								ids.insert(lexer.currentLexeme);
							}
							currentNode->setAttribute(key, lexer.currentLexeme);
							token = getNextToken();
							if (token != QUOTE) {
								Utility::quitWithError(__LINE__, __FILE__, L" \' must be ended by \'");
							}
						}
						else if (token == Token::DQUOTE) {
							token = getNextToken();
							if (key == Utility::ID) {
								if (ids.count(lexer.currentLexeme)) {
									Utility::warning(L"id " + lexer.currentLexeme + L" already existed");
								}
								ids.insert(lexer.currentLexeme);
							}
							currentNode->setAttribute(key, lexer.currentLexeme);
							token = getNextToken();
							if (token != DQUOTE) {
								Utility::quitWithError(__LINE__, __FILE__, L"\" must be ended by \"");
							}
						}
						else {
							Utility::quitWithError(__LINE__, __FILE__, L"value fllowed by = must be covered by \' or \"");
						}
					}

				}
				break;
			case COMMENTBEGIN:
				if (insideTag) {
					handleComment();
				}
				else {
					insideTag = true;
					handleComment();
					insideTag = false;
				}
				break;
			}
			token = getNextToken();
		}
	}

	SimpleXMLParser::Token SimpleXMLParser::getNextToken() {
		using namespace std;
		lexer.index0 = lexer.index1;
		if (lexer.index0 == content.end()) return Token::END;
		if (insideTag) {
			Token token = STRING;
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
			if (lexer.index1 == content.end()) return Token::END;
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
		else {
			Token token = STRING;
			auto tempIndex = lexer.index0;
			lexer.index0 = find_if(lexer.index0, content.end(), isNotWhiteSpace);
			lexer.index1 = find_if(lexer.index0, content.end(), [](wchar_t ch) {return ch == COPEN; });
			if (lexer.index1 == content.end()) return Token::END;
			if (lexer.index0 == lexer.index1) {
				lexer.index1++;
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
			}
			lexer.currentLexeme = std::wstring(tempIndex, lexer.index1);
			return token;
		}
		
	}

	SimpleXMLParser::TokenInfo SimpleXMLParser::lookAheadK(int n)
	{
		lexer.save();
		Token token;
		std::wstring lexeme;
		for (int i = 0; i < n; i++) {
			token = getNextToken();
			lexeme = lexer.currentLexeme;
		}
		lexer.restore();
		return TokenInfo(token, lexeme);
	}

	SimpleXMLParser::Token SimpleXMLParser::handleComment()
	{
		Token token;
		using namespace std;
		do {
			 token = getNextToken();
		} while (token != COMENTEND && token != END);
		if (token == END) {
			Utility::quitWithError(__LINE__, __FILE__, L" the comment doesn't has a ending");
		}
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
	void SimpleXMLParser::Lexer::save()
	{
		Status status(index0, index1, currentLexeme, token, state);
		statuses.emplace_back(status);
	}
	void SimpleXMLParser::Lexer::restore()
	{
		auto tmp = statuses.back();
		statuses.pop_back();
		currentLexeme = tmp.currentLexeme;
		index0 = tmp.index0;
		index1 = tmp.index1;
		token = tmp.token;
		state = tmp.state;
	}
	SimpleXMLParser::Lexer::Status::Status(const std::wstring::iterator & i0, const std::wstring::iterator & i1,const std::wstring& c, SimpleXMLParser::Token t, State s)
		:index0(i0),index1(i1),token(t),state(s), currentLexeme(c)
	{
	}
	SimpleXMLParser::TokenInfo::TokenInfo(Token t, const std::wstring & s)
		:token(t),lexeme(s)
	{
	}
}

