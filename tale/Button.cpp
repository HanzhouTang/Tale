#include"Button.h"



Button::Button() {}

void Button::onPressDown(Button::CallbackFunction f) {
	if (f != nullptr)
		f(this);
}

Button* Button::createButton(Element::Brush b, D2D1_RECT_F position, CallbackFunction f, wstring c) {
	auto ret = new Button();
	ret->setBrush(b);
	ret->setPosition(position);
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