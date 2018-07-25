#include"Sprite.h"
void Sprite::preDraw(D2D1_RECT_F, float t) {
	dt += t;
	if (dt > interval) {
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