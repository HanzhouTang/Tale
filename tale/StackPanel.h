#pragma once
#include"Element.h"
#include<memory>
#include<vector>
struct StackPanel:Element
{
	enum Orientation{horizontal, vertical};
	Orientation orientation = horizontal;
	float padding = 1.0f;
	StackPanel() {
		setType(TYPE_STACKPANEL);
		setBrush(Element::Brush(Element::BrushType::transparent, ComPtr<ID2D1Brush>(nullptr)));
	};
	void setPadding(float p);
	void setOrientation(Orientation o); 
	void rearrangement();
	virtual void addChild(const shared_ptr<Element>&) override;
	static shared_ptr<StackPanel> createStackPanel(D2D1_RECT_F, Orientation = horizontal, float =1.0f);
	static shared_ptr<StackPanel> createStackPanelByXml(const shared_ptr<Node>& node);
};