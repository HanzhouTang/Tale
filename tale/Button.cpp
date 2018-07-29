#include"Button.h"



Button::Button() {}

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