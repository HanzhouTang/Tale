#pragma once
#include"Element.h"
struct  Sprite: Element
{
	vector<Brush> brushes;
	float interval;
	float dt;
	size_t index;
	static shared_ptr<Sprite> createSprite(D2D1_RECT_F position, float interval,const Element::Brush&);
	void setInterval(float dt) { interval = dt; } // dt: seconds to wait.
	void setIndex(size_t i) { index = i; }
	void addBrush(const Element::Brush& b) { brushes.push_back(b); }
	virtual void preDraw(D2D1_RECT_F, float) override; 
	virtual ~Sprite();
};