#pragma once
#include"Element.h"
#include<memory>
#include<vector>
struct StackPanel
{
	enum Orientation{horizontal, vertical};
	Orientation orientation;
	StackPanel(Orientation o) : orientation(o) {};
	virtual void addChild(const shared_ptr<Element>&) override;
};