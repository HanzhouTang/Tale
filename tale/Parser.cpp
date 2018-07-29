#include"XMLParser.h"
#include<stack>
#include<cassert>
void XMLParser::parse(wstring str) {
	content = str;
	int i = 0;
	stack<wstring> trace;
	bool insideTag = false;
	wstring name;
	shared_ptr<Node> current;
	while (i < content.size()) {
		if (content[i] == COPEN) {
			if (insideTag) {
				cout << "A xml tag cannot inside another tag" << endl;
				assert(1 == 2);
			}
			insideTag = true;
#pragma region COMMNET
			if (content[i + 1] == CEXCLAMATION && content[i + 2] == CMINUS && content[i + 3] == CMINUS) {
				i += 4;
				while (i < content.size() && (content[i] != CMINUS && content[i + 1] != CMINUS && content[i + 2] != CCLOSE)) i++;
				i += 3;
				insideTag = false;
			}
#pragma endregion

		}
	}
}