#pragma once
#include"Element.h"
struct Button : public Element {
public:
	wstring caption;
	ComPtr<IDWriteTextFormat> textFormat;
	Brush defaultBrush;
	Brush mouseHoverBrush;
	typedef void(*CallbackFunction)(Button*);
	CallbackFunction action;
	virtual void onPressDown(CallbackFunction f);
	static shared_ptr<Button> createButton(Element::Brush, D2D1_RECT_F, CallbackFunction f=nullptr, wstring=L"");
	void setCallbackFunction(CallbackFunction f) { action = f; }
	inline void setCaption(wstring s) { caption = s; }
	inline void setmouseHoverBrush(Brush b) { mouseHoverBrush = b; }
	inline bool inside(const COORD&, const D2D1_RECT_F&);
	inline void setTextFormat(ComPtr<IDWriteTextFormat> format) { textFormat = format; }
	virtual void update(MouseMessage, D2D1_RECT_F) override;
	virtual ~Button();
	Button();
protected:
	inline void setDefaultBrush(Brush b) { defaultBrush = b; }
};
//Finding a bug, when resize the windows, it doesn't work