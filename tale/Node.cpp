#include"Node.h"
namespace xml {
	std::map<std::wstring, std::wstring>& Node::getAttributes()
	{
		return attributes;
	}
	std::shared_ptr<Node> Node::createNode(const std::wstring& name,
		const std::shared_ptr<Node>& parent) {
		auto ret = std::make_shared<Node>();
		ret->setParent(parent);
		ret->setName(name);
		return ret;
	}
}
