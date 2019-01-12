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
			LBrace = 0, RBrace, Comma, LCurlyBrace, RCurlyBrace, Times, Minus,
			Add, Div, LParen, RParen, Eql, Less, Greater, And, Or, Semicolon
			, If, Else, Quote, Invalid, Variable, Newline, EndofContent, Number,
			String, Colon, Def, EqlEql, True, False, Not
		};
		static const std::vector<std::wstring> TokenNames;
		virtual std::wstring getTokenName(int t);
		std::vector<int> lookAheadK(int k);
		struct LexerNode {
			std::wstring lexeme;
			int token;
			LexerNode(const std::wstring& lex, int t);
		};
		enum State { inString, noString, endString };
		static const std::set<std::wstring> delimiters;
		static const std::map<std::wstring, int> lexeme2token;
		std::wstring content;
		//==============inner content======================
		std::wstring::iterator index0, index1;
		std::wstring currentLexeme;
		int token;
		State state;
		//=================================================
		struct Status {
			std::wstring::iterator index0, index1;
			std::wstring currentLexeme;
			bool endOfContent;
			int token;
			State state;
			Status(std::wstring::iterator i0, std::wstring::iterator i1,
				std::wstring l, int t, State s,bool isEnded = false);
			bool operator ==(const Status& other) const;
			Status() = default;

		};
		struct StatusHasher {
			std::size_t operator()(const Status& s) const;
		};
		std::stack<Status> statusStack;
		void set(const Status& status);
		Status get();
		void save();
		void pop();
		void restore();
		wchar_t peek();
		virtual int getNextToken();
		virtual bool isDelimiter(const wchar_t str);
		virtual bool isWhiteSpace(const wchar_t ch);
		virtual bool isNumber(const std::wstring& str);
		virtual bool isValidVariable(const std::wstring& str);
		SimpleLexer(const std::wstring& str);
		SimpleLexer(std::wstring&& str);
		void init();
		SimpleLexer() = delete;
		~SimpleLexer();
	};

};