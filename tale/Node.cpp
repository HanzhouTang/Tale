#include"Node.h"
shared_ptr<Node> Node::createNode(shared_ptr<Node> p) {
	shared_ptr<Node> ret = make_shared<Node>();
	ret->setParent(p);
	return ret;
}