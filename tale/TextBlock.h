#pragma once
#include"Element.h"
#include<set>
struct TextBlock : Element {
	std::wstring text;
	ComPtr<IDWriteTextFormat> textFormat=nullptr;
	Brush textBrush;
	static ComPtr<ID2D1SolidColorBrush> defaultTextColor;
	void setText(const std::wstring& str);
	std::wstring getText();
	void setTextFormat(ComPtr<IDWriteTextFormat> format);
	void setTextBrush(const Element::Brush& b);
	virtual void postDraw(D2D1_RECT_F, float) override;
	TextBlock();
	virtual ~TextBlock();
	static std::set<std::wstring> systemFontCollection;
	static bool init;
	static void setTextBlockRenderingAttribute(const std::shared_ptr<TextBlock>& button);
	static void setFontCollection(const std::set<std::wstring>& collection);
	static ComPtr<IDWriteTextFormat> createTextFormatFromAttribute(const shared_ptr<Attribute>&);
	static std::set<std::wstring> getSystemFontCollection();
	virtual void setAttribute(const std::wstring& key, const std::wstring& value) override;
	static ComPtr<IDWriteFactory> pDWriteFactory;
	static shared_ptr<TextBlock> createTextblockByXml(const shared_ptr<xml::Node>& node);
	std::shared_ptr<TextBlock> createTextBlock(std::wstring, Element::Brush b, D2D1_RECT_F position, Element::Brush bText, ComPtr<IDWriteTextFormat> foramt);
};