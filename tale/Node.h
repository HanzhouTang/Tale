#pragma once
#include"Common.h"
struct Node : std::enable_shared_from_this<Node> {
	wstring name;
	wstring expressions;
	shared_ptr<Node> parent;
	vector<shared_ptr<Node>> children;
	map<wstring, wstring> attributes;
	void setParent(const shared_ptr<Node>& p) { parent = p; }
	void setName(const wstring& n) { name = n; }
	wstring getName() const { return name; }
	int getChildrenCount() { return children.size(); }
	wstring getValue() const { return expressions; }
	wstring getAttribute(const wstring& key) const { if (attributes.find(key) == attributes.cend()) return L""; else return attributes.find(key)->second; }
	vector<shared_ptr<Node>>& getChildren() { return children; }
	shared_ptr<Node> getChild(int ith) { return children[ith]; }
	void addChild(shared_ptr<Node> c) { children.push_back(c); }
	void appendValue(const wstring& v) { expressions += (v + L" "); }
	void setAttribute(const wstring& key, const wstring& expressions) {
		attributes[key] = expressions;
	}
	std::vector<std::shared_ptr<Node>> getNodesByName(const std::wstring& neededName) {
		std::vector<std::shared_ptr<Node>> ret;
		for (auto& x : children) {
			auto childRet = x->getNodesByName(neededName);
			if (!childRet.empty()) {
				ret.insert(ret.end(), childRet.begin(), childRet.end());
			}
		}
		if (neededName == name) {
			ret.emplace_back(shared_from_this());
		}
		return ret;
	}
	shared_ptr<Node> getParent() const { return parent; }
	wstring info(wstring parentStr = L"")const {
		wstring ret;
		if (expressions.empty()) {
			ret += (name + L"\n");
		}
		else {
			ret += (name + L" : " + expressions + L"\n");
		}
		for (const auto& attr : attributes) {
			ret += (name + L"[" + attr.first + L"]= " + attr.second + L"\n");
		}
		for (const auto& x : children) {
			if (name != L"")
				ret += (name + L"." + x->info());
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