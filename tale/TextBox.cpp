#include"TextBox.h"

void TextBox::setText(const std::wstring & str)
{
	text = str;
}

std::wstring TextBox::getText()
{
	return text;
}

void TextBox::setTextFormat(ComPtr<IDWriteTextFormat> format)
{
	textFormat = format;
}

void TextBox::setTextBrush(const Element::Brush & b)
{
	textBrush = b;
}

void TextBox::update(MouseMessage, D2D1_RECT_F)
{
}

ComPtr<ID2D1SolidColorBrush> TextBox::defaultTextColor = nullptr;