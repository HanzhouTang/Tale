#include"Attribute.h"

std::wstring Attribute::getAttribute(const std::wstring & key) const
{
	if (attributes.find(key) == attributes.cend()) return L""; else return attributes.find(key)->second;
}

void Attribute::setAttribute(const std::wstring & key, const std::wstring & value)
{
	attributes[key] = value;
}

std::map<std::wstring, std::wstring>& Attribute::getAttributes()
{
	return attributes;
}
