#pragma once
#include"Element.h"
struct Button : public Element {
public:
	wstring caption;
	Brush defaultBrush;
	Brush mouseHoverBrush;
	typedef void(*CallbackFunction)(Button*);
	CallbackFunction action;
	virtual void onPressDown(CallbackFunction f);
	static Button* createButton(Element::Brush, D2D1_RECT_F, CallbackFunction f=nullptr, wstring=L"");
	void setCallbackFunction(CallbackFunction f) { action = f; }
	inline void setCaption(wstring s) { caption = s; }
	inline void setDefaultBrush(Brush b) { defaultBrush = b; }
	inline void setmouseHoverBrush(Brush b) { mouseHoverBrush = b; }
	inline bool inside(const COORD&, const D2D1_RECT_F&);
	virtual void update(MouseMessage, D2D1_RECT_F) override;
protected:
	Button();
};