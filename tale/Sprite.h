#pragma once
#include"Element.h"
struct  Sprite: Element
{
	/*
	Be cation, sprite works not good on debug mode
	*/
	vector<Brush> brushes;
	float interval;
	float dt;
	size_t index;
	void setInterval(float dt) { interval = dt; } // dt: seconds to wait.
	void setIndex(size_t i) { index = i; }
	void addBrush(const Element::Brush& b) { brushes.push_back(b); }
	Brush getFirstBrush() { return brushes.size() > 0 ? brushes[0] : Brush(BrushType::transparent, nullptr); }
	virtual void preDraw(D2D1_RECT_F, float) override; 
	virtual void setAttribute(const std::wstring& key, const std::wstring& value) override;
	static void setSpriteRenderingAttribute(const std::shared_ptr<Sprite>& sprite);
	static shared_ptr<Sprite> createSprite(D2D1_RECT_F position, float interval, const Element::Brush&);
	static shared_ptr<Sprite> createSpriteByXml(const shared_ptr<xml::Node>& node);
	Sprite() {
		setType(TYPE_SPRITE);
		setInterval(0.5);
	}
	virtual ~Sprite();
};