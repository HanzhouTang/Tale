#include<iostream>
#include<vector>
#include<algorithm>
#include<memory>
#include<string>
#include<set>
#include<regex>
#include<map>
namespace parser {
	struct SimpleLexer {
		enum Token {
			LBrace, RBrace, Comma,LCurlyBrace, RCurlyBrace, Times, Minus,
			Add, Div, LParen, RParen, Eql, Less, Greater, And,Or, Smicolon
			, If, Else, Quote, Invalid, Variable,Newline, EndofContent,Number
		};
		enum State { inString, noString, endString };
		static const std::set<std::wstring> delimiters;
		static const std::map<std::wstring, Token> lexeme2token;
		std::wstring content;
		std::wstring::iterator index0, index1;
		std::wstring currentLexeme;
		Token token;
		State state;
		wchar_t peek();
		Token getNextToken();
		static bool isDelimiter(const wchar_t str) {
			return delimiters.count(str+L"");
		}
		static bool isWhiteSpace(const wchar_t ch) {
			return ch == L' ' || ch == L'\t';
		}
		static bool isNumber(const std::wstring& str) {
			using namespace std;
			return regex_match(str,wregex(L"[+-]?([0-9]*[.])?[0-9]+"));
		}
		static bool isValidVariable(const std::wstring& str) {
			using namespace std;
			return regex_match(str,wregex(L"^[a-z_]\\w*$"));
		}
		SimpleLexer(const std::wstring& str) :content(str){}
		SimpleLexer(std::wstring&& str) :content(std::move(str)){}
		void init() {
			index0 = index1 = content.begin();
			token = Invalid;
			state = noString;
		}
	};

};