#include"Button.h"
#include"Expr.h"
using namespace Utility;

Button::Button() {
	setType(TYPE_BUTTON);
	if (defaultButtonColor.Get() == nullptr) {
		if (FAILED(Element::d2dContext->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF::Gray), &defaultButtonColor)))
		{
			quitWithError(__LINE__, __FILE__, L"create default button color brush failed");
		}
		if (FAILED(Element::d2dContext->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF::Black), &defaultTextColor))) {
			quitWithError(__LINE__, __FILE__, L"create default text color brush failed");
		}
		if (FAILED(Element::d2dContext->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF::LightBlue), &defaultMouseHoverColor))) {
			quitWithError(__LINE__, __FILE__, L"create default mouse hover color brush failed");
		}
	}
	Element::Brush btext(Element::BrushType::solid, defaultTextColor);
	setTextBrush(btext);
	Element::Brush bcolor(Element::BrushType::solid, defaultButtonColor);
	setBrush(bcolor);
	setDefaultBrush(bcolor);
	setmouseHoverBrush(Brush(Element::BrushType::solid, defaultMouseHoverColor));
	setTextFormat(Element::defaultTextFormat);
}

void Button::onPressDown(const Button::CallbackFunction& f) {
	if (f != nullptr)
		f->getValue();
}

shared_ptr<Button> Button::createButton(Element::Brush b, D2D1_RECT_F position, Element::Brush bText, ComPtr<IDWriteTextFormat> foramt, CallbackFunction f, wstring c) {
	auto ret = make_shared<Button>();
	ret->setBrush(b);
	ret->setTextBrush(bText);
	ret->setPosition(position);
	ret->setTextFormat(foramt);
	ret->setOnClickFunction(f);
	ret->setCaption(c);
	ret->setDefaultBrush(b);
	ret->setmouseHoverBrush(b);
	return ret;
}


void Button::update(MouseMessage message, D2D1_RECT_F parentPosition)
{
	auto realPosition = getRealPosition(parentPosition);
	if (inside(message.position, realPosition)) {
		if (message.event == Element::Event::LButtonDown) {
			onPressDown(_clickAction);
		}

		setBrush(mouseHoverBrush);
	}
	else {
		setBrush(defaultBrush);
	}
	for (auto& x : children) {
		x->update(message, realPosition);
	}
}

Button::~Button() {
	defaultButtonColor.Reset();
	defaultBrush.m_brush.Reset();
	mouseHoverBrush.m_brush.Reset();
	textBrush.m_brush.Reset();
	textFormat.Reset();
}

void Button::postDraw(D2D1_RECT_F realPosition, float dt) {
	if (textFormat != nullptr && textBrush.m_brush != nullptr) {
		d2dContext->DrawText(caption.c_str(), caption.length(), textFormat.Get(), realPosition, textBrush.m_brush.Get());
	}
}

void Button::setAttribute(const std::wstring & key, const std::wstring & value)
{
	Element::setAttribute(key, value);
	setButtonRenderingAttribute(std::dynamic_pointer_cast<Button>(shared_from_this()));
}

void Button::setButtonRenderingAttribute(const std::shared_ptr<Button>& ret)
{
	auto brush = createBrushFromAttribute(ret);
	ret->setBrush(brush);
	ret->setDefaultBrush(brush);
	auto hoverBrushUrl = ret->getAttribute(MOUSEHOVERBRUSH_EN);
	if (hoverBrushUrl.empty())
		hoverBrushUrl = ret->getAttribute(MOUSEHOVERBRUSH_CH);
	if (!hoverBrushUrl.empty()) {
		auto bitmapBrush = Utility::CreateBitmapBrushFromFile(Element::d2dContext.Get(), Element::imageFactory.Get(), hoverBrushUrl.c_str());
		Brush mouseHoverBrush(BrushType::bitmap, bitmapBrush);
		ret->setmouseHoverBrush(mouseHoverBrush);
	}
}


shared_ptr<Button> Button::createButtonByXml(const shared_ptr<xml::Node>& node) {
	auto ret = make_shared<Button>();
	ret->setCaption(node->getValue());
	auto brush = createBrushFromAttribute(node);
	ret->setBrush(brush);
	ret->setDefaultBrush(brush);
	auto hoverBrushUrl = node->getAttribute(MOUSEHOVERBRUSH_EN);
	if (hoverBrushUrl.empty())
		hoverBrushUrl = node->getAttribute(MOUSEHOVERBRUSH_CH);
	if (!hoverBrushUrl.empty()) {
		auto bitmapBrush = Utility::CreateBitmapBrushFromFile(Element::d2dContext.Get(), Element::imageFactory.Get(), hoverBrushUrl.c_str());
		Brush mouseHoverBrush(BrushType::bitmap, bitmapBrush);
		ret->setmouseHoverBrush(mouseHoverBrush);
	}
	return ret;
}

ComPtr<ID2D1SolidColorBrush> Button::defaultButtonColor = nullptr;
ComPtr<ID2D1SolidColorBrush> Button::defaultTextColor = nullptr;
ComPtr<ID2D1SolidColorBrush> Button::defaultMouseHoverColor = nullptr;


/*
ret->setBrush(b);
ret->setTextBrush(bText);
ret->setPosition(position);
ret->setTextFormat(foramt);
ret->setCallbackFunction(f);
ret->setCaption(c);
ret->setDefaultBrush(b);
ret->setmouseHoverBrush(b);

*/