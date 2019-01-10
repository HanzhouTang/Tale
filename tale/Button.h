#pragma once
#include"Element.h"
#include"TextBlock.h"

#include<functional>
namespace expr {
	struct Expr;
}

struct Button : public TextBlock {
public:
	//wstring caption;
	//ComPtr<IDWriteTextFormat> textFormat=nullptr;
	Brush defaultBrush;
	Brush mouseHoverBrush;
	Brush textBrush;
	typedef  std::shared_ptr<expr::Expr> CallbackFunction;
	CallbackFunction _clickAction;
	virtual void onPressDown(const CallbackFunction& f);
	static shared_ptr<Button> createButton(Element::Brush, D2D1_RECT_F, Element::Brush = Brush(), ComPtr<IDWriteTextFormat> = ComPtr<IDWriteTextFormat>(nullptr), CallbackFunction f = nullptr, wstring = L"");
	static shared_ptr<Button> createButtonByXml(const shared_ptr<xml::Node>& node);
	void setOnClickFunction(const CallbackFunction& f) { _clickAction = f; }
	//inline void setCaption(wstring s) { caption = s; }
	inline void setmouseHoverBrush(Brush b) { mouseHoverBrush = b; }
	inline void setDefaultBrush(Brush b) { defaultBrush = b; }
	//inline void setTextFormat(ComPtr<IDWriteTextFormat> format) { textFormat = format; }
	//inline void setTextBrush(const Element::Brush& b) { textBrush = b; }
	virtual void update(MouseMessage, D2D1_RECT_F) override;
	virtual void postDraw(D2D1_RECT_F, float) override;
	virtual void setAttribute(const std::wstring& key, const std::wstring& value) override;
	static void setButtonRenderingAttribute(const std::shared_ptr<Button>& button);
	static ComPtr<IDWriteTextFormat> createButtonTextFormatFromAttribute(const shared_ptr<Attribute>&);
	static ComPtr<ID2D1SolidColorBrush> defaultButtonColor;
	static ComPtr<ID2D1SolidColorBrush> defaultTextColor;
	static ComPtr<ID2D1SolidColorBrush> defaultMouseHoverColor;
	virtual ~Button();
	Button();
};
//TODO bitmap brush