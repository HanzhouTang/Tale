#include"Element.h"

Element::Element(){}

void Element::setPosition(D2D1_RECT_F p) {
	assert(p.left >= 0 && p.top >= 0);
	assert(p.top <= MaximumRealtiveRatio && p.bottom <= MaximumRealtiveRatio);
	position = p;
}

const float Element::MaximumRealtiveRatio = 100.0f;


D2D1_RECT_F Element::getRealPosition(D2D1_RECT_F parentPosition) {
	auto realPosition = parentPosition;
	auto relativeWidth = (position.right - position.left) / MaximumRealtiveRatio;
	auto relativeHeight = (position.bottom - position.top) / MaximumRealtiveRatio;
	auto height = realPosition.bottom - realPosition.top;
	auto width = realPosition.right - realPosition.left;
	realPosition.top = (position.top / MaximumRealtiveRatio)*height + realPosition.top;
	realPosition.left = (position.left / MaximumRealtiveRatio)*width + realPosition.left;
	realPosition.bottom = height * relativeHeight + realPosition.top;
	realPosition.right = width * relativeWidth + realPosition.left;
	return realPosition;
}

void Element::onDraw(D2D1_RECT_F parentPosition) {
	preDraw();
	auto realPosition = getRealPosition(parentPosition);
	//std::cout << "top: " << realPosition.top << " bottom: " << realPosition.bottom << " left: " << realPosition.left << " right: " << realPosition.right << std::endl;
	switch (brush.brushType) {
	case solid:
		ID2D1SolidColorBrush * solidbrush = (ID2D1SolidColorBrush*)brush.m_brush.Get();
		renderTarget->FillRectangle(&realPosition, solidbrush);
		break;
	}
	for (auto& x : children) {
		x->onDraw(realPosition);
	}
	postDraw();
}

void Element::update(Element::MouseMessage message, D2D1_RECT_F parentPosition){
	auto realPosition = getRealPosition(parentPosition);
	for (auto& x : children) {
		x->update(message, realPosition);
	}
}



void Element::setRenderTarget(ComPtr<ID2D1HwndRenderTarget> target) {
	renderTarget = target;
}

void Element::setBrush(Brush b) {
	brush = b;
}

ComPtr<ID2D1HwndRenderTarget> Element::renderTarget = nullptr;

void Element::preDraw(){}
void Element::postDraw() {}
void Element::onMouseMoveOn(){}
void Element::onPressLeftButton(){}
void Element::addChild(Element* t) {
	children.push_back(t);
}

Element::~Element() {
	for (auto& x : children) {
		delete x;
	}
	brush.m_brush.Reset();
}
Element* Element::createElement(Brush b, D2D1_RECT_F position) {
	auto ret = new Element();
	ret->setBrush(b);
	ret->setPosition(position);
	return ret;
}