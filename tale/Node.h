#pragma once
#include"Common.h"
#include"Attribute.h"
struct Node {
	wstring name;
	shared_ptr<Node> parent;
	vector<shared_ptr<Node>> children;
	vector<shared_ptr<Attribute>> attributes;
	static shared_ptr<Node> createNode(shared_ptr<Node> parent);
	void setParent(shared_ptr<Node> p) { parent = p; }
};