#include"Node.h"
namespace xml {
	std::shared_ptr<Node> Node::createNode(const std::wstring& name,
		const std::shared_ptr<Node>& parent) {
		auto ret = std::make_shared<Node>();
		ret->setParent(parent);
		ret->setName(name);
		return ret;
	}
}
