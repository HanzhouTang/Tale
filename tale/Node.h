#pragma once
#include"Common.h"
namespace xml {
	struct Node : std::enable_shared_from_this<Node> {
		std::wstring name;
		std::wstring expressions;
		std::shared_ptr<Node> parent;
		std::vector<std::shared_ptr<Node>> children;
		std::map<std::wstring, std::wstring> attributes;
		void setParent(const std::shared_ptr<Node>& p) { parent = p; }
		void setName(const std::wstring& n) { name = n; }
		std::wstring getName() const { return name; }
		int getChildrenCount() { return children.size(); }
		std::wstring getValue() const { return expressions; }
		std::wstring getAttribute(const std::wstring& key) const { if (attributes.find(key) == attributes.cend()) return L""; else return attributes.find(key)->second; }
		std::vector<std::shared_ptr<Node>>& getChildren() { return children; }
		std::shared_ptr<Node> getChild(int ith) { return children[ith]; }
		void addChild(std::shared_ptr<Node> c) { children.push_back(c); }
		void appendValue(const std::wstring& v) { expressions += (v + L" "); }
		void setAttribute(const std::wstring& key, const std::wstring& expressions) {
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
		std::shared_ptr<Node> getParent() const { return parent; }
		std::wstring info(std::wstring parentStr = L"")const {
			std::wstring ret;
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
		std::shared_ptr<Node> getHighLevel() {
			auto ret = getParent();
			if (ret == nullptr) {
				return std::shared_ptr<Node>(this);
			}
			return ret;
		}
		static std::shared_ptr<Node> createNode(const std::wstring& name, const std::shared_ptr<Node>&);
	};
}
