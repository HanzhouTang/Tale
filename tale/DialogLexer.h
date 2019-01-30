#pragma once
#include"SimpleLexer.h"
namespace dialogParser {
	struct DialogLexer : parser::SimpleLexer {
		enum DialogToken {
			StarStar,StarSign, Colon,Dash,STRING,CR,LF,INVALID,Quote,EndOfContent
		};
		static const std::vector<std::wstring> DialogTokenNames;
		static const std::set<std::wstring> DialogDelimiters;
		static const std::map<std::wstring, int> DialogLexeme2token;
		DialogLexer(const std::wstring& str);
		wchar_t matchingQuote;
		virtual std::wstring getTokenName(int t) override;
		virtual int getNextToken() override;
		virtual bool isDelimiter(const wchar_t str) override;
	};
}
