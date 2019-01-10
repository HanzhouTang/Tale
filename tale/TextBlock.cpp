#include"TextBlock.h"
#include"Utility.h"
#include<dwrite.h>
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
		d2dContext->DrawText(text.c_str(), text.length(), textFormat.Get(), realPosition, textBrush.m_brush.Get());
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
	auto fontCollection = getSystemFontCollection();
	setFontCollection(fontCollection);
	Element::Brush btext(Element::BrushType::solid, defaultTextColor);
	setTextBrush(btext);
	setBrush(Element::Brush(Element::BrushType::transparent, nullptr));
	//setTextFormat(Element::defaultTextFormat);
}

TextBlock::~TextBlock()
{
	defaultTextColor.Reset();
	textBrush.m_brush.Reset();
	textFormat.Reset();
}


void TextBlock::setTextBlockRenderingAttribute(const std::shared_ptr<TextBlock>& ret)
{
	auto brush = createBrushFromAttribute(ret);
	ret->setBrush(brush);
	auto format = createTextFormatFromAttribute(ret);
	ret->setTextFormat(format);
}

void TextBlock::setFontCollection(const std::set<std::wstring>& collection)
{
	systemFontCollection = collection;
}

ComPtr<IDWriteTextFormat> TextBlock::createTextFormatFromAttribute(const shared_ptr<Attribute>& att)
{
	using namespace Utility;
	if (pDWriteFactory == nullptr) {
		if (FAILED(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory),
			reinterpret_cast<IUnknown**>(pDWriteFactory.GetAddressOf())))) {
			Utility::warning(L"create direct write factory failed");
			return nullptr;
		}
	}
	std::wstring font = att->getAttribute(Font_EN);
	if (font.empty()) {
		font = att->getAttribute(Font_CH);
	}
	if (font.empty()) {
		font = L"Gabriola";
	}
	if (!systemFontCollection.count(font)) {
		Utility::warning(L"The font " + font + L" doesn't exist, use the default font");
		font = L"Gabriola";
	}
	auto fontSize = att->getAttribute(FontSize_EN);
	if (fontSize.empty()) {
		fontSize = att->getAttribute(FontSize_CH);
	}
	if (fontSize.empty()) {
		fontSize = L"21";
	}
	auto tmp = Utility::wstr2floats(fontSize);
	if (tmp.size() != 1) {
		Utility::warning(L"invalid font size");
		tmp = { 21 };
	}
	ComPtr<IDWriteTextFormat> pTextFormat;
	if (FAILED(pDWriteFactory->CreateTextFormat(
		font.c_str(),                // Font family name.
		NULL,                       // Font collection (NULL sets it to use the system font collection).
		DWRITE_FONT_WEIGHT_MEDIUM,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_MEDIUM,
		tmp[0],
		L"zh-cn",
		&pTextFormat))) {
		return nullptr;
		// need change later
	}
	pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
	pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
	return pTextFormat;
}

std::set<std::wstring> TextBlock::getSystemFontCollection()
{
	// make sure everything need draw text inhert from this.

	if (init) {
		return systemFontCollection;
	}
	init = true;

	if (pDWriteFactory == nullptr) {
		if (FAILED(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory),
			reinterpret_cast<IUnknown**>(pDWriteFactory.GetAddressOf())))) {
			Utility::warning(L"create direct write factory failed");
			return systemFontCollection;
		}
	}
	ComPtr<IDWriteFontCollection> pFontCollection;
	if (FAILED(pDWriteFactory->GetSystemFontCollection(&pFontCollection))) {
		Utility::warning(L"get font collection failed");
		return systemFontCollection;
	}
	int count = pFontCollection->GetFontFamilyCount();
	std::set<std::wstring> fontCollection;
	for (int i = 0; i < count; i++) {
		ComPtr<IDWriteFontFamily> pFontFamily;
		pFontCollection->GetFontFamily(i, &pFontFamily);
		ComPtr<IDWriteLocalizedStrings> pFamilyName;
		pFontFamily->GetFamilyNames(&pFamilyName);
		UINT32 index = 0;
		BOOL exist = false;
		pFamilyName->FindLocaleName(L"zh-cn", &index, &exist);
		if (!exist) {
			pFamilyName->FindLocaleName(L"en-us", &index, &exist);
		}
		UINT32 length;
		pFamilyName->GetStringLength(index, &length);
		wchar_t* name = new (std::nothrow) wchar_t[length + 1];
		pFamilyName->GetString(index, name, length + 1);
		fontCollection.emplace(name);
		//std::wcout << name << std::endl;
		delete[] name;
	}
	return fontCollection;
}

void TextBlock::setAttribute(const std::wstring & key, const std::wstring & value)
{
	Element::setAttribute(key, value);
	setTextBlockRenderingAttribute(std::dynamic_pointer_cast<TextBlock>(shared_from_this()));
}

void TextBlock::setValue(std::wstring value)
{
	setText(value);
}

shared_ptr<TextBlock> TextBlock::createTextblockByXml(const shared_ptr<xml::Node>& node)
{
	auto ret = make_shared<TextBlock>();
	ret->setText(node->getValue());
	auto brush = createBrushFromAttribute(node);
	ret->setBrush(brush);
	auto format = createTextFormatFromAttribute(node);
	ret->setTextFormat(format);
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
std::set<std::wstring> TextBlock::systemFontCollection;
bool TextBlock::init = false;
ComPtr<IDWriteFactory> TextBlock::pDWriteFactory;