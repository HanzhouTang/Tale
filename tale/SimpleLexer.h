#include<iostream>
#include<vector>
#include<algorithm>
#include<memory>
#include<string>
#include<set>
#include<regex>
#include<map>
#include<stack>
#include<functional>
namespace parser {
	struct SimpleLexer {
		enum Token {
			LBrace=0, RBrace, Comma,LCurlyBrace, RCurlyBrace, Times, Minus,
			Add, Div, LParen, RParen, Eql, Less, Greater, And,Or, Semicolon
			, If, Else, Quote, Invalid, Variable,Newline, EndofContent,Number,
			String,Colon,Def,EqlEql,True,False
		};
		static const std::vector<std::wstring> TokenNames;
		static std::wstring getTokenName(Token t) {
			return TokenNames[t];
		}
		std::vector<Token> lookAheadK(int k);
		struct LexerNode {
			std::wstring lexeme;
			Token token;
			LexerNode(const std::wstring& lex, Token t): lexeme(lex),token(t){}
		};
		enum State { inString, noString,endString };
		static const std::set<std::wstring> delimiters;
		static const std::map<std::wstring, Token> lexeme2token;
		std::wstring content;
		//====================================
		std::wstring::iterator index0, index1;
		std::wstring currentLexeme;
		Token token;
		State state;
		//====================================
		struct Status {
			std::wstring::iterator index0, index1;
			std::wstring currentLexeme;
			Token token;
			State state;
			Status(std::wstring::iterator i0, std::wstring::iterator i1,
				std::wstring l,Token t,State s):
				index0(i0),index1(i1),currentLexeme(l),token(t),state(s)
			{}
			bool operator ==(const Status& other) const {
				return &*index0 == &*other.index0 && &*index1 == &*other.index1 && state == other.state;
			}
			//may need calculate distance from beginning
			Status(){}

		};

		struct StatusHasher {
			std::size_t operator()(const Status& s) const {
				using namespace std;
				return hash<wchar_t*>{}(&*s.index0) ^ (hash<wchar_t*>{}(&*s.index1) << 1) ^ (hash<int>{}(s.state) << 2);
			}
		};

		std::stack<Status> statusStack;

		inline void set(const Status& status) {
			index0 = status.index0;
			index1 = status.index1;
			currentLexeme = status.currentLexeme;
			token = status.token;
			state = status.state;
		}

		inline Status get() {
			return Status(index0, index1, currentLexeme, token, state);
		}

		inline void save() {
			Status status(index0, index1, currentLexeme, token, state);
			statusStack.push(status);
		}

		inline void pop() {
			statusStack.pop();
		}

		inline void restore() {
			auto x = statusStack.top();
			statusStack.pop();

			index0 = x.index0;
			index1 = x.index1;
			currentLexeme = x.currentLexeme;
			token = x.token;
			state = x.state;
		}

		wchar_t peek();
		Token getNextToken();
		static bool isDelimiter(const wchar_t str) {
			return delimiters.count(std::wstring(1, str));
		}

		static bool isWhiteSpace(const wchar_t ch) {
			using namespace std;
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
		SimpleLexer() = delete;

	};

};