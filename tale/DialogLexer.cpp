#include"DialogLexer.h"
namespace dialogParser {
	const std::vector<std::wstring> DialogLexer::DialogTokenNames = {
		L"StarStar",L"StarSign", L"Colon",L"Dash",L"STRING",L"CR",L"LF",L"INVALID",L"Quote",L"EndOfContent"
	};
	const std::set<std::wstring> DialogLexer::DialogDelimiters = {
		L"*",L":",L"-",L"\n",L"\r",L"：",L"\"",L"“",L"”"
	};
	const std::map<std::wstring, int> DialogLexer::DialogLexeme2token = {
		{L"*",DialogLexer::DialogToken::StarSign},
		{L":",DialogLexer::DialogToken::Colon},
		{ L"：",DialogLexer::DialogToken::Colon },
		{ L"-",DialogLexer::DialogToken::Dash },
		{ L"\n",DialogLexer::DialogToken::LF },
		{ L"\r",DialogLexer::DialogToken::CR },
		{L"\"",DialogLexer::DialogToken::Quote},
		{ L"“",DialogLexer::DialogToken::Quote },
		{ L"”",DialogLexer::DialogToken::Quote }
	};

	DialogLexer::DialogLexer(const std::wstring & str)
		:parser::SimpleLexer(str)
	{
	}

	std::wstring DialogLexer::getTokenName(int t)
	{
		return t<DialogTokenNames.size()?DialogTokenNames[t]:L"INVALID";
	}

	int DialogLexer::getNextToken()
	{
		using namespace std;
		auto last = content.end();
		if (index1 == last) return DialogToken::EndOfContent;
		if (state == State::inString) {
			wstring str;
			for (; index1 != last; index1++) {
				if (*index1 == matchingQuote) break;
				if (*index1 == L'\\') {
					index1++;
				}
				str.push_back(*index1);
			}
			currentLexeme = str;
			state = State::endString;
			return DialogToken::STRING;
		}
		index1 = find_if(index1, last, [&](wchar_t ch) {return !isWhiteSpace(ch); });
		if (index1 == last) return Token::EndofContent;
		index0 = index1;
		index1 = find_if(index0, last, [&](const wchar_t& ch) {return isDelimiter(ch); });
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
				matchingQuote = L'"';
			}
		}
		else if (currentLexeme == L"“") {
			state = State::inString;
			matchingQuote = L'”';
		}
		else if (currentLexeme == L"”") {
			if (state == State::endString) {
				state = State::noString;
			}
		}
		if (currentLexeme == L"*") {
			auto nextChar = peek();
			if (nextChar == L'*') {
				index1++;
				currentLexeme = L"**";
				return DialogToken::StarStar;
			}
		}
		if (DialogLexeme2token.count(currentLexeme)) {
			return DialogLexeme2token.find(currentLexeme)->second;
		}
		if (!currentLexeme.empty()) {
			return DialogToken::STRING;
		}
		return DialogToken::INVALID;
	}

	bool DialogLexer::isDelimiter(const wchar_t str)
	{
		return DialogDelimiters.count(std::wstring(1, str));
	}

}