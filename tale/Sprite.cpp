#include"Sprite.h"
using namespace Utility;
void Sprite::preDraw(D2D1_RECT_F, float t) {
	dt += t;
	if (dt > interval && brushes.size()!=0 ) {
		index = (index + 1) % brushes.size();
		setBrush(brushes[index]);
		dt = 0;
	}
}

Sprite::~Sprite() {
	for (auto& x : brushes) {
		x.m_brush.Reset();
	}
}

shared_ptr<Sprite> Sprite::createSprite(D2D1_RECT_F position,float t, const Element::Brush& brush) {
	auto ret = make_shared<Sprite>();
	ret->setInterval(t);
	ret->addBrush(brush);
	ret->setBrush(brush);
	ret->setPosition(position);
	ret->setIndex(0);
	return ret;
}

shared_ptr<Sprite> Sprite::createSpriteByXml(const shared_ptr<xml::Node>& node) {
	auto ret = make_shared<Sprite>();
	auto interval = Utility::wstr2floats(node->getAttribute(INTERVAL_EN));
	if (interval.empty())
		interval = Utility::wstr2floats(node->getAttribute(INTERVAL_CH));
	if (interval.size() == 1) {
		ret->setInterval(interval[0]);
	}
	for (const auto& x : node->getChildren()) {
		auto name = x->getName();
		if (name != ITEM_EN && name!=ITEM_CH) {
			warning(name+ L" is not a valid name inside Sprite");
		}
		else {
			auto brush = getBrushFromXml(x);
			ret->addBrush(brush);
		}
	}
	ret->setBrush(ret->getFirstBrush());
	return ret;
}