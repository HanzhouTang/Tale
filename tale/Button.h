#pragma once
#include"Element.h"
#include<functional>
struct Button : public Element {
public:
	wstring caption;
	ComPtr<IDWriteTextFormat> textFormat;
	Brush defaultBrush;
	Brush mouseHoverBrush;
	Brush textBrush;
	//typedef void(*CallbackFunction)(Button*);
	typedef function<void(Button*)> CallbackFunction;
	CallbackFunction action;
	virtual void onPressDown(CallbackFunction f);
	static shared_ptr<Button> createButton(Element::Brush,  Element::Brush, D2D1_RECT_F, ComPtr<IDWriteTextFormat>,CallbackFunction f=nullptr, wstring=L"");
	void setCallbackFunction(CallbackFunction f) { action = f; }
	inline void setCaption(wstring s) { caption = s; }
	inline void setmouseHoverBrush(Brush b) { mouseHoverBrush = b; }
	inline bool inside(const COORD&, const D2D1_RECT_F&);
	inline void setTextFormat(ComPtr<IDWriteTextFormat> format) { textFormat = format; }
	inline void setTextBrush(const Element::Brush& b) { textBrush = b; }
	virtual void update(MouseMessage, D2D1_RECT_F) override;
	virtual void postDraw(D2D1_RECT_F) override;
	virtual ~Button();
	Button();
protected:
	inline void setDefaultBrush(Brush b) { defaultBrush = b; }
};
//TODO bitmap brush