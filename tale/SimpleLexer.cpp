#include"SimpleLexer.h"
namespace parser {
	const std::set<std::wstring> SimpleLexer::delimiters =
	{
		L"[",L"]",L",",L"{",L"}",L"*",L"-",L"+",L"/",L"(",L")",L"=",L"<",L">",L";",L"\"",L"\n",L"\"",L" ",L":"
	};
	const std::map<std::wstring, SimpleLexer::Token> SimpleLexer::lexeme2token = {
	{L"[",Token::LBrace},{L"]",Token::RBrace},{L",",Token::Comma},{L"{",Token::LCurlyBrace},
	{L"}",Token::RCurlyBrace},{L"*",Token::Times},{L"-",Token::Minus},{L"+",Token::Add},{L"/",Token::Div},
	{L"(",Token::LParen},{L")",Token::RParen},{L"=",Token::Eql},{L"<",Token::Less},{L">",Token::Greater},
	{L";",Token::Semicolon},{L"\"",Token::Quote},{L"\n",Token::Newline} , {L"and",Token::And},{L"or",Token::Or},
	{L"if",Token::If},{L"else",Token::Else},{L":",Token::Colon},{L"def",Token::Def},{ L"true",Token::True }
	,{ L"false",Token::False }	,{ L"not",Token::Not }
	};

	const std::vector<std::wstring> SimpleLexer::TokenNames = {
		L"LBrace",L"RBrace", L"Comma",L"LCurlyBrace",L"RCurlyBrace", L"Times", L"Minus",
		L"Add", L"Div", L"LParen", L"RParen", L"Eql", L"Less", L"Greater", L"And",L"Or", L"Semicolon"
		, L"If", L"Else", L"Quote", L"Invalid", L"Variable",L"Newline", L"EndofContent",L"Number",
		L"String",L"Colon",L"Def",L"EqlEql",L"True",L"False",L"Not"
	};

	SimpleLexer::Token SimpleLexer::getNextToken() {
		using namespace std;
		auto last = content.end();
		if (index1 == last) return Token::EndofContent;
		if (state == State::inString) {
			wstring str;
			for (; index1 != last; index1++) {
				if (*index1 == L'"') break;
				if (*index1 == L'\\') {
					index1++;
				}
				str.push_back(*index1);
			}
			currentLexeme = str;
			state = State::endString;
			return Token::String;
		}
		index1 = find_if(index1, last, [&](wchar_t ch) {return !isWhiteSpace(ch); });
		if (index1 == last) return Token::EndofContent;
		index0 = index1;
		index1 = find_if(index0, last, isDelimiter);
		if (index0 == index1) {
			index1++;
		}
		currentLexeme = std::wstring(index0, index1);

		if (currentLexeme == L"\"") {
			if (state == State::endString) {
				state = State::noString;
			}
			else if (state == State::noString) {
				state = State::inString;
			}
		}
		if (currentLexeme == L"=") {
			auto nextChar = peek();
			if (nextChar == L'=') {
				index1++;
				currentLexeme = L"==";
				return Token::EqlEql;
			}
		}
		//wcout << L"(" << currentLexeme << L")" << endl;
		if (lexeme2token.count(currentLexeme)) {
			return lexeme2token.find(currentLexeme)->second;
		}
		if (isNumber(currentLexeme)) {
			return Token::Number;
		}
		if (isValidVariable(currentLexeme)) {
			return Token::Variable;
		}
		return Token::Invalid;
	}

	bool SimpleLexer::isDelimiter(const wchar_t str)
	{
		return delimiters.count(std::wstring(1, str));
	}

	bool SimpleLexer::isWhiteSpace(const wchar_t ch)
	{
		using namespace std;
		return ch == L' ' || ch == L'\t';
	}
	bool SimpleLexer::isNumber(const std::wstring & str)
	{
		using namespace std;
		return regex_match(str, wregex(L"[+-]?([0-9]*[.])?[0-9]+"));
	}
	bool SimpleLexer::isValidVariable(const std::wstring & str)
	{
		using namespace std;
		return regex_match(str, wregex(L"^[a-z_]\\w*$"));
	}

	SimpleLexer::SimpleLexer(const std::wstring & str)
		:content(str) {
		init();
	}

	SimpleLexer::SimpleLexer(std::wstring && str)
		: content(std::move(str)) {
		init();
	}

	void SimpleLexer::init()
	{
		if (!content.empty()) {
			index0 = index1 = content.begin();
		}
		else {
			std::wcerr << "init simpleLexer without content" << std::endl;
		}
		token = Invalid;
		state = noString;
	}

	SimpleLexer::~SimpleLexer()
	{
		if (!content.empty()) {
			index0 = index1 = content.begin();
		}
	}

	void SimpleLexer::set(const Status & status)
	{
		index0 = status.index0;
		index1 = status.index1;
		currentLexeme = status.currentLexeme;
		token = status.token;
		state = status.state;
	}

	SimpleLexer::Status SimpleLexer::get()
	{
		if (index1 == content.end()) {
			return Status(index0, index1, currentLexeme, token, state, true);
		}
		return Status(index0, index1, currentLexeme, token, state);
	}
	void SimpleLexer::save()
	{
		Status status(index0, index1, currentLexeme, token, state);
		statusStack.push(status);
	}

	void SimpleLexer::pop()
	{
		statusStack.pop();
	}
	void SimpleLexer::restore()
	{
		auto x = statusStack.top();
		statusStack.pop();

		index0 = x.index0;
		index1 = x.index1;
		currentLexeme = x.currentLexeme;
		token = x.token;
		state = x.state;
	}
	wchar_t SimpleLexer::peek() {
		return *(index1);
	}

	std::wstring SimpleLexer::getTokenName(Token t)
	{
		return TokenNames[t];
	}

	std::vector<SimpleLexer::Token> SimpleLexer::lookAheadK(int k) {
		save();
		std::vector<Token> tokens;
		for (int i = 0; i < k; i++) {
			auto t = getNextToken();
			tokens.push_back(t);
			if (t == Token::EndofContent) {
				break;
			}
		}
		restore();
		return tokens;
	}


	SimpleLexer::LexerNode::LexerNode(const std::wstring & lex, Token t) : lexeme(lex), token(t) {}

	SimpleLexer::Status::Status(std::wstring::iterator i0,
		std::wstring::iterator i1, std::wstring l, Token t, State s, bool isEnded) :
		index0(i0), index1(i1), currentLexeme(l), token(t), state(s), endOfContent(isEnded) {}

	bool SimpleLexer::Status::operator==(const Status & other) const
	{
		return &*index0 == &*other.index0 && &*index1 == &*other.index1 && state == other.state;
	}

	std::size_t SimpleLexer::StatusHasher::operator()(const Status & s) const
	{
		using namespace std;
		if (s.endOfContent) {
			return  hash<wchar_t*>{}(nullptr) ^ (hash<wchar_t*>{}(nullptr) << 1) ^ (hash<int>{}(s.state) << 2);
		}
		return hash<wchar_t*>{}(&*s.index0) ^ (hash<wchar_t*>{}(&*s.index1) << 1) ^ (hash<int>{}(s.state) << 2);

	}

};

/*
OK
I need a schedule
I need follow the schedule
Otherwise, I can do nothing

*/