#pragma once
#include"d2dutil.h"
#include<vector>
using namespace std;
class Element {
public:
	enum BrushType { solid, linear, radial, image };
	enum Event {LButtonDown,LButtonUp,RButtonDown,RButtonUp,MouseMove};
	struct Message {
		Event event;
		COORD position;
	};
	static const float MaximumRealtiveRatio;
	vector<Element*> children;
	D2D1_RECT_F position;
	static ID2D1HwndRenderTarget* renderTarget;
	BrushType brushType;
	ID2D1Brush* brush;
	
	Element();
	virtual ~Element();
	void onDraw(D2D1_RECT_F);
	virtual void preDraw();
	virtual void postDraw();
	virtual void onPressLeftButton();
	virtual void onMouseMoveOn();
	void setPosition(D2D1_RECT_F);
	void setBrush(BrushType, ID2D1Brush*);
	static void setRenderTarget(ID2D1HwndRenderTarget*);
	void addChild(Element*);
	static Element* createElement(BrushType, ID2D1Brush*, D2D1_RECT_F);
};