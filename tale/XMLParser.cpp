#include"XMLParser.h"
#include<stack>
#include<cassert>
void XMLParser::parse(wstring str) {
	content = str;
	Token token = getNextToken();
	while (token != Token::END) {
		switch (token) {

		}
	}
}

XMLParser::Token XMLParser::getNextToken() {
	return Token::END;

}