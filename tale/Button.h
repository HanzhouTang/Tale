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
	static shared_ptr<Button> createButton(Element::Brush, D2D1_RECT_F, Element::Brush=Brush(), ComPtr<IDWriteTextFormat> = ComPtr<IDWriteTextFormat>(nullptr),CallbackFunction f=nullptr, wstring=L"");
	static shared_ptr<Button> createButtonByXml(const shared_ptr<Node>& node);
	void setCallbackFunction(CallbackFunction f) { action = f; }
	inline void setCaption(wstring s) { caption = s; }
	inline void setmouseHoverBrush(Brush b) { mouseHoverBrush = b; }
	inline void setDefaultBrush(Brush b) { defaultBrush = b; }
	inline bool inside(const COORD&, const D2D1_RECT_F&);
	inline void setTextFormat(ComPtr<IDWriteTextFormat> format) { textFormat = format; }
	inline void setTextBrush(const Element::Brush& b) { textBrush = b; }
	virtual void update(MouseMessage, D2D1_RECT_F) override;
	virtual void postDraw(D2D1_RECT_F,float) override;
	static ComPtr<ID2D1SolidColorBrush> defaultButtonColor;
	static ComPtr<ID2D1SolidColorBrush> defaultTextColor;
	static ComPtr<ID2D1SolidColorBrush> defaultMouseHoverColor;
	virtual ~Button();
	Button();

};
//TODO bitmap brush