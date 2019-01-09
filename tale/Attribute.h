#pragma once
#include<map>
struct Attribute {
	std::map<std::wstring, std::wstring> attributes;
	virtual std::wstring getAttribute(const std::wstring& key) const;
	virtual void setAttribute(const std::wstring& key, const std::wstring& value);
	std::map<std::wstring, std::wstring>& getAttributes();
};