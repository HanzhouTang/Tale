#include"Node.h"
shared_ptr<Node> Node::createNode(const wstring& name,
	const shared_ptr<Node>& parent){
	auto ret = make_shared<Node>();
	ret->setParent(parent);
	ret->setName(name);
	return ret;
}