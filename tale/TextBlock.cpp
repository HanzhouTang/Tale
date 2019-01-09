#include"TextBlock.h"
#include"Utility.h"
void TextBlock::setText(const std::wstring & str)
{
	text = str;
}

std::wstring TextBlock::getText()
{
	return text;
}

void TextBlock::setTextFormat(ComPtr<IDWriteTextFormat> format)
{
	textFormat = format;
}

void TextBlock::setTextBrush(const Element::Brush & b)
{
	textBrush = b;
}



void TextBlock::postDraw(D2D1_RECT_F realPosition, float)
{
	if (textFormat != nullptr && textBrush.m_brush != nullptr) {
		auto preAligment = textFormat->GetTextAlignment();
		auto preParagraphAlignment = textFormat->GetParagraphAlignment();
		textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
		textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
		d2dContext->DrawText(text.c_str(), text.length(), textFormat.Get(), realPosition, textBrush.m_brush.Get());
		textFormat->SetTextAlignment(preAligment);
		textFormat->SetParagraphAlignment(preParagraphAlignment);
	}
}

TextBlock::TextBlock()
{
	setType(TYPE_TEXTBOOCK);
	if (defaultTextColor == nullptr) {
		if (FAILED(Element::d2dContext->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF::Black), &defaultTextColor))) {
			Utility::quitWithError(__LINE__, __FILE__, L"create default text color brush failed");
		}
	}
	Element::Brush btext(Element::BrushType::solid, defaultTextColor);
	setTextBrush(btext);
	setBrush(Element::Brush(Element::BrushType::transparent, nullptr));
	setTextFormat(Element::defaultTextFormat);
}

TextBlock::~TextBlock()
{
	defaultTextColor.Reset();
	textBrush.m_brush.Reset();
	textFormat.Reset();
}

void TextBlock::setTextBlockRenderingAttribute(const std::shared_ptr<TextBlock>& ret)
{
	auto brush = getBrushFromAttribute(ret);
	ret->setBrush(brush);
}

void TextBlock::setAttribute(const std::wstring & key, const std::wstring & value)
{
	Element::setAttribute(key, value);
	setTextBlockRenderingAttribute(std::dynamic_pointer_cast<TextBlock>(shared_from_this()));
}

shared_ptr<TextBlock> TextBlock::createTextblockByXml(const shared_ptr<xml::Node>& node)
{
	auto ret = make_shared<TextBlock>();
	ret->setText(node->getValue());
	auto brush = getBrushFromAttribute(node);
	ret->setBrush(brush);
	return ret;
}

std::shared_ptr<TextBlock> TextBlock::createTextBlock(std::wstring, Element::Brush b, D2D1_RECT_F position, Element::Brush bText, ComPtr<IDWriteTextFormat> foramt)
{
	auto ret = make_shared<TextBlock>();
	ret->setBrush(b);
	ret->setTextBrush(bText);
	ret->setPosition(position);
	ret->setTextFormat(foramt);
	ret->setText(text);
	return ret;
}

ComPtr<ID2D1SolidColorBrush> TextBlock::defaultTextColor = nullptr;