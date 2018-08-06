#include"Button.h"



Button::Button() {
	if (defaultButtonColor.Get() == nullptr) {
		if (FAILED(Element::d2dContext->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF::Gray), &defaultButtonColor)))
		{
			cout << "ERROR: create solid brush failed" << endl;
			assert(1 == 2);
		}
		if (FAILED(Element::d2dContext->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF::Black), &defaultTextColor))) {
			cout << "ERROR: create solid brush failed" << endl;
			assert(1 == 2);
		}
		if (FAILED(Element::d2dContext->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF::LightBlue), &defaultMouseHoverColor))) {
			cout << "ERROR: create solid brush failed" << endl;
			assert(1 == 2);
		}
	}
	Element::Brush btext(Element::BrushType::solid, defaultTextColor);
	setTextBrush(btext);
	Element::Brush bcolor(Element::BrushType::solid, defaultButtonColor);
	setBrush(bcolor);
	setDefaultBrush(bcolor);
	setmouseHoverBrush(Brush(Element::BrushType::solid, defaultMouseHoverColor));
	setTextFormat(Element::textFormat);

}

void Button::onPressDown(Button::CallbackFunction f) {
	if (f != nullptr)
		f(this);
}

shared_ptr<Button> Button::createButton(Element::Brush b, D2D1_RECT_F position, Element::Brush bText, ComPtr<IDWriteTextFormat> foramt, CallbackFunction f, wstring c) {
	auto ret = make_shared<Button>();
	ret->setBrush(b);
	ret->setTextBrush(bText);
	ret->setPosition(position);
	ret->setTextFormat(foramt);
	ret->setCallbackFunction(f);
	ret->setCaption(c);
	ret->setDefaultBrush(b);
	ret->setmouseHoverBrush(b);
	return ret;
}

bool Button::inside(const COORD& position, const D2D1_RECT_F& realPosition) {
	if (position.X >= realPosition.left&& position.X <= realPosition.right
		&&position.Y >= realPosition.top&&position.Y <= realPosition.bottom)
		return true;
	return false;
}
void Button::update(MouseMessage message, D2D1_RECT_F parentPosition)
{
	auto realPosition = getRealPosition(parentPosition);
	if (inside(message.position, realPosition)) {
		if (message.event == Element::Event::LButtonDown) {
			onPressDown(action);
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

void Button::postDraw(D2D1_RECT_F realPosition,float dt) {
	
	if (textFormat != nullptr && textBrush.m_brush!=nullptr) {
		
		d2dContext->DrawText(caption.c_str(), caption.length(), textFormat.Get(), realPosition, textBrush.m_brush.Get());
	}
}


shared_ptr<Button> Button::createButtonByXml(const shared_ptr<Node>& node) {
	auto ret = make_shared<Button>();
	ret->setCaption(node->getValue());
	auto hoverBrushUrl = node->getAttribute(L"mouseHoverBrush");

	auto url = node->getAttribute(L"brush");
	if (!url.empty()) {
		auto bitmapBrush = Utility::CreateBitmapBrushFromFile(Element::d2dContext.Get(), Element::imageFactory.Get(), url.c_str());
		Brush brush(BrushType::bitmap, bitmapBrush);
		ret->setBrush(brush);
		ret->setDefaultBrush(brush);
	}

	if (!hoverBrushUrl.empty()) {
		auto bitmapBrush = Utility::CreateBitmapBrushFromFile(Element::d2dContext.Get(), Element::imageFactory.Get(), hoverBrushUrl.c_str());
		Brush mouseHoverBrush(BrushType::bitmap, bitmapBrush);
		ret->setmouseHoverBrush(mouseHoverBrush);
	}
	return ret;
}

ComPtr<ID2D1SolidColorBrush> Button::defaultButtonColor = nullptr;
ComPtr<ID2D1SolidColorBrush> Button::defaultTextColor =nullptr;
ComPtr<ID2D1SolidColorBrush> Button::defaultMouseHoverColor =nullptr;


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