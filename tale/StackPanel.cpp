#include "StackPanel.h"

void StackPanel::setPadding(float p)
{
	padding = p;
	rearrangement();

}

void StackPanel::setOrientation(Orientation o)
{
	orientation = o;
	rearrangement();
}

void StackPanel::addChild(const shared_ptr<Element>& t)
{
	
	children.push_back(t);
	rearrangement();
	
}

void StackPanel::rearrangement() {
	int count = children.size();
	float length = Element::MaximumRealtiveRatio / count;
	if (orientation == horizontal) {
		for (int i = 0; i < count; i++) {
			auto position = D2D1::RectF(length*i + padding, 0,length*(i + 1) - padding, Element::MaximumRealtiveRatio);
			children[i]->setPosition(position);
		}

	}
	else if (orientation == vertical) {
		for (int i = 0; i < count; i++) {
			auto position = D2D1::RectF(0, length*i + padding, Element::MaximumRealtiveRatio, length*(i + 1) - padding);
			children[i]->setPosition(position);
		}
	}
}

shared_ptr<StackPanel> StackPanel::createStackPanel(D2D1_RECT_F position, Orientation o, float padding)
{
	auto ret = make_shared<StackPanel>();
	ret->setPosition(position);
	ret->setPadding(padding);
	ret->setOrientation(o);
	return ret;
}

shared_ptr<StackPanel> StackPanel::createStackPanelByXml(const shared_ptr<Node>& node) {
	auto ret = make_shared<StackPanel>();
	auto padding = Utility::wstr2floats(node->getAttribute(L"padding"));
	if (padding.size() == 1) {
		ret->setPadding(padding[0]);
	}
	auto url = node->getAttribute(L"brush");
	if (!url.empty()) {
		auto bitmapBrush = Utility::CreateBitmapBrushFromFile(Element::d2dContext.Get(), Element::imageFactory.Get(), url.c_str());
		Brush brush(BrushType::bitmap, bitmapBrush);
		ret->setBrush(brush);
	}
	auto orientation = node->getAttribute(L"orientation");
	if (orientation == L"vertical") {
		ret->setOrientation(vertical);
	}
	else if (orientation == L"horizontal") {
		ret->setOrientation(horizontal);
	}
	return ret;
}