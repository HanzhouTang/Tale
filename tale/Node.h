#pragma once
#include"Common.h"
struct Node {
	wstring name;
	wstring value;
	shared_ptr<Node> parent;
	vector<shared_ptr<Node>> children;
	map<wstring, wstring> attributes;
	void setParent(const shared_ptr<Node>& p) { parent = p; }
	void setName(const wstring& n) { name = n; }
	wstring getName() { return name; }
	wstring getAttribute(const wstring& key) { return attributes[key]; }
	shared_ptr<Node> getChild(int ith) { return children[ith]; }
	void addChild(shared_ptr<Node> c) { children.push_back(c); }
	void appendValue(const wstring& v) { value += (v + L" "); }
	void setAttribute(const wstring& key, const wstring& value) {
		attributes[key] = value;
	}
	shared_ptr<Node> getParent() { return parent; }
	wstring info(wstring parentStr = L"")const {
	
		wstring ret;
		if (value.empty()) {
			ret += (name + L"\n");
		}
		else {
			ret += (name + L" : " + value + L"\n");
		}
		for (const auto& attr : attributes) {
			ret += (name + L"[" + attr.first + L"]= " + attr.second + L"\n");
		}
		for (const auto& x : children) {
			if (name != L"")
				ret += (name+L"."+x->info());
			else {
				ret += x->info();
			}
		}
	
		return ret;
	}
	shared_ptr<Node> getHighLevel() {
		auto ret = getParent();
		if (ret == nullptr) {
			return shared_ptr<Node>(this);
		}
		return ret;
	}
	static shared_ptr<Node> createNode(const wstring& name, const shared_ptr<Node>&);
};
//need a function to output it.
//simpler than my imagine