#pragma once
#include"d2dutil.h"
#include<vector>
using namespace std;
using namespace Microsoft::WRL;
struct Element {
public:
	enum BrushType { solid, linear, radial, image };
	enum Event {LButtonDown,LButtonUp,RButtonDown,RButtonUp,MouseMove};
	struct Brush {
		BrushType brushType;
		ComPtr<ID2D1Brush> m_brush;
		Brush(const Brush& b) {
			brushType = b.brushType;
			m_brush = b.m_brush;
		}
		Brush(BrushType type, ComPtr<ID2D1Brush> b) {
			brushType = type;
			m_brush = b;
		}
		Brush(){}
	};
	struct MouseMessage {
		Event event;
		COORD position;
		MouseMessage(Event e, COORD p) {
			event = e;
			position = p;
		}
	};
	virtual void update(MouseMessage,D2D1_RECT_F);
	static const float MaximumRealtiveRatio;
	vector<Element*> children;
	D2D1_RECT_F position;
	static ComPtr<ID2D1HwndRenderTarget> renderTarget;
	Brush brush;
	D2D1_RECT_F getRealPosition(D2D1_RECT_F);
	virtual ~Element();
	void onDraw(D2D1_RECT_F);
	virtual void preDraw();
	virtual void postDraw();
	virtual void onPressLeftButton();
	virtual void onMouseMoveOn();
	void setPosition(D2D1_RECT_F);
	void setBrush(Brush b);
	static void setRenderTarget(ComPtr<ID2D1HwndRenderTarget>);
	void addChild(Element*);
	static Element* createElement(Brush, D2D1_RECT_F);
protected:
	Element();
};