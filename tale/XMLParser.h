#pragma once
#include"Node.h"
#include"Attribute.h"
using namespace std;
class XMLParser {
	wstring content;
	shared_ptr<Node> root;
	void parse(wstring str);
	const static wchar_t COPEN = L'<';
	const static wchar_t CCLOSE = L'>';
	const static wchar_t CSPACE = L' ';
	const static wchar_t CQUOTE = L'\'';
	const static wchar_t CDQUOTE = L'\"';
	const static wchar_t CTAB = L'\t';
	const static wchar_t CNEWLINE = L'\n';
	const static wchar_t CEXCLAMATION = L'!';
	const static wchar_t CMINUS = L'-';
};