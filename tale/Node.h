#pragma once
#include"Common.h"
struct Node {
	wstring name;
	wstring value;
	shared_ptr<Node> parent;
	vector<shared_ptr<Node>> children;
	map<wstring,wstring> attributes;
	static shared_ptr<Node> createNode(shared_ptr<Node> parent);
	void setParent(shared_ptr<Node> p) { parent = p; }
	shared_ptr<Node> getParent() { return parent; }
};