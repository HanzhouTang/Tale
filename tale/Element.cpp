#include"Element.h"
#include"Button.h"
#include"StackPanel.h"
#include"Sprite.h"
using namespace Utility;
Element::Element() {
	setBrush(Brush(BrushType::transparent, nullptr));
	position = D2D1::RectF(0, 0, 100, 100);
	setType(TYPE_ELEMENT);
}
const std::vector<std::wstring> Element::TypeList = { L"TYPE_ELEMENT",L"TYPE_BUTTON",L"TYPE_SPRITE",L"TYPE_STACKPANEL" };
void Element::setPosition(D2D1_RECT_F p) {
	assert(p.left >= 0 && p.top >= 0);
	assert(p.top <= MaximumRealtiveRatio && p.bottom <= MaximumRealtiveRatio);
	position = p;
}

const float Element::MaximumRealtiveRatio = 100.0f;


D2D1_RECT_F Element::getRealPosition(D2D1_RECT_F parentPosition) {
	auto realPosition = parentPosition;
	auto relativeWidth = (position.right - position.left) / MaximumRealtiveRatio;
	auto relativeHeight = (position.bottom - position.top) / MaximumRealtiveRatio;
	auto height = realPosition.bottom - realPosition.top;
	auto width = realPosition.right - realPosition.left;
	realPosition.top = (position.top / MaximumRealtiveRatio)*height + realPosition.top;
	realPosition.left = (position.left / MaximumRealtiveRatio)*width + realPosition.left;
	realPosition.bottom = height * relativeHeight + realPosition.top;
	realPosition.right = width * relativeWidth + realPosition.left;
	return realPosition;
}

void Element::onDraw(D2D1_RECT_F parentPosition, float dt) {

	auto realPosition = getRealPosition(parentPosition);
	preDraw(realPosition, dt);
	//std::cout << "top: " << realPosition.top << " bottom: " << realPosition.bottom << " left: " << realPosition.left << " right: " << realPosition.right << std::endl;
	switch (brush.brushType) {
	case transparent:
		break;

	case bitmap:
	{
		ComPtr<ID2D1BitmapBrush> bitmapBrush(reinterpret_cast<ID2D1BitmapBrush*>(brush.m_brush.Get()));
		ComPtr<ID2D1Bitmap> bitmap;
		bitmapBrush->GetBitmap(bitmap.GetAddressOf());
		auto size = bitmap->GetPixelSize();
		float width = realPosition.right - realPosition.left;
		float height = realPosition.bottom - realPosition.top;
		auto translation = D2D1::Matrix3x2F::Translation(realPosition.left, realPosition.top);
		auto scale = D2D1::Matrix3x2F::Scale(width / size.width, height / size.height);
		brush.m_brush->SetTransform(scale*translation);
		d2dContext->FillRectangle(&realPosition, brush.m_brush.Get());
		break;
	}
	default:
		d2dContext->FillRectangle(&realPosition, brush.m_brush.Get());
		break;
	}
	postDraw(realPosition, dt);
	for (auto& x : children) {
		x->onDraw(realPosition, dt);
	}

}

void Element::update(Element::MouseMessage message, D2D1_RECT_F parentPosition) {
	auto realPosition = getRealPosition(parentPosition);
	for (auto& x : children) {
		x->update(message, realPosition);
	}
}



void Element::setD2dContext(ComPtr<ID2D1DeviceContext> target) {
	d2dContext = target;
}
void Element::setImageFactory(ComPtr<IWICImagingFactory> factory) {
	imageFactory = factory;
}


void Element::setBrush(Brush b) {
	brush = b;
}

ComPtr<ID2D1DeviceContext> Element::d2dContext = nullptr;
ComPtr<IWICImagingFactory> Element::imageFactory = nullptr;
ComPtr<IDWriteTextFormat>  Element::textFormat = nullptr;
void Element::preDraw(D2D1_RECT_F, float) {}
void Element::postDraw(D2D1_RECT_F, float) {}
void Element::onMouseMoveOn() {}
void Element::onPressLeftButton() {}
void Element::addChild(const shared_ptr<Element>& t) {
	children.push_back(t);
}

std::shared_ptr<Element> Element::getIthChild(std::size_t i)
{
	if (i < children.size()) {
		return children[i];
	}
	else {
		quitWithError(__LINE__, __FILE__, L"vist a doesn't exist child");
		return nullptr;
	}
	return std::shared_ptr<Element>();
}

Element::~Element() {
	for (auto& x : children) {
		x.reset();
	}
	brush.m_brush.Reset();
}
shared_ptr<Element> Element::createElement(Brush b, D2D1_RECT_F position) {
	auto ret = make_shared<Element>();
	ret->setBrush(b);
	ret->setPosition(position);
	return ret;
}

shared_ptr<Element> Element::createElementByXml(const shared_ptr<Node>& root) {
	if (root == nullptr) return shared_ptr<Element>(nullptr);
	const wstring& name = root->getName();
	shared_ptr<Element> ret;
	if (name == ELEMENT_EN || name == ELEMENT_CH) {
		ret = make_shared<Element>();
		auto brush = getBrushFromXml(root);
		ret->setBrush(brush);
	}
	else if (name == BUTTON_EN || name == BUTTON_CH) {
		ret = Button::createButtonByXml(root);
	}
	else if (name == STACKPANEL_EN || name == STACKPANEL_CH) {
		ret = StackPanel::createStackPanelByXml(root);
	}
	else if (name == SPRITE_EN || name == SPRITE_CH) {
		ret = Sprite::createSpriteByXml(root);
	}

	if (ret == nullptr) {
		warning(name + L" is not a valid XML Tag");
		return nullptr;
	}

	auto position = Utility::wstr2floats(root->getAttribute(POSITION_EN));
	if (position.empty()) {
		position = Utility::wstr2floats(root->getAttribute(POSITION_CH));
	}

	if (!position.empty()) {
		if (position.size() != 4) {
			quitWithError(__LINE__, __FILE__, L"the size of position must be 4");
		}
		auto positionRect = D2D1::RectF(position[0], position[1], position[2], position[3]);
		//std::cout << "top: " << positionRect.top << " bottom: " << positionRect.bottom << " left: " << positionRect.left << " right: " << positionRect.right << std::endl;
		ret->setPosition(positionRect);
	}

	if (name == SPRITE_CH || name ==  SPRITE_EN) return ret;

	for (const auto& nodeChild : root->getChildren()) {
		auto child = Element::createElementByXml(nodeChild);
		if (child != nullptr) {
			ret->addChild(child);
		}
	}
	return ret;
}

Element::Brush Element::getBrushFromXml(const shared_ptr<Node>& node) {
	auto url = node->getAttribute(BRUSH_EN);
	if (url.empty())
		url = node->getAttribute(BRUSH_CH);
	if (!url.empty()) {
		auto bitmapBrush = Utility::CreateBitmapBrushFromFile(Element::d2dContext.Get(), Element::imageFactory.Get(), url.c_str());
		return Brush(BrushType::bitmap, bitmapBrush);
	}
	return Brush(BrushType::transparent, nullptr);
}