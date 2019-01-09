#pragma once
#include"Element.h"
struct TextBox : Element {
	std::wstring text;
	ComPtr<IDWriteTextFormat> textFormat;
	Brush textBrush;
	static ComPtr<ID2D1SolidColorBrush> defaultTextColor;

	void setText(const std::wstring& str);
	std::wstring getText();
	void setTextFormat(ComPtr<IDWriteTextFormat> format);
	void setTextBrush(const Element::Brush& b);
	virtual void update(MouseMessage, D2D1_RECT_F) override;
};