#pragma once
#include"d2dutil.h"
#include"Node.h"
#include"Utility.h"
#include"Attribute.h"
#include<vector>
#include<memory>
#include<set>
using namespace std;
using namespace Microsoft::WRL;
struct Element :std::enable_shared_from_this<Element>, Attribute {
public:
	enum BrushType { solid, linear, radial, bitmap, transparent };
	enum Event { LButtonDown, LButtonUp, RButtonDown, RButtonUp, MouseMove };
	enum ElementType { TYPE_ELEMENT = 0, TYPE_BUTTON, TYPE_SPRITE, TYPE_STACKPANEL, TYPE_TEXTBOOCK };
	static const std::vector<std::wstring> TypeList;
	ElementType _type;
	inline ElementType getType() { return _type; }
	inline void setType(ElementType t) { _type = t; }
	inline std::wstring getTypeString() { return TypeList[getType()]; }
	struct Brush {
		BrushType brushType;
		ComPtr<ID2D1Brush> m_brush;
		Brush(const Brush& b) :
			brushType(b.brushType), m_brush(b.m_brush) { }
		Brush(BrushType type, const ComPtr<ID2D1Brush>& b) :
			brushType(type), m_brush(b) { }
		Brush() :
			brushType(transparent), m_brush(nullptr) { }
	};
	struct MouseMessage {
		Event event;
		COORD position;
		MouseMessage(Event e, COORD p) {
			event = e;
			position = p;
		}
	};
	virtual void update(MouseMessage, D2D1_RECT_F);
	static const float MaximumRealtiveRatio;
	vector<shared_ptr<Element>> children;
	inline vector<shared_ptr<Element>>& getChildren() { return children; }
	D2D1_RECT_F position;
	static ComPtr<ID2D1DeviceContext> d2dContext;
	static ComPtr<IWICImagingFactory> imageFactory;
	static ComPtr<IDWriteTextFormat>  defaultTextFormat;
	Brush brush;
	virtual ~Element();
	void onDraw(D2D1_RECT_F, float);
	virtual void preDraw(D2D1_RECT_F, float);
	virtual void postDraw(D2D1_RECT_F, float);
	virtual void onPressLeftButton();
	virtual void onMouseMoveOn();
	void setPosition(D2D1_RECT_F);
	void setBrush(Brush b);
	D2D1_RECT_F getRealPosition(D2D1_RECT_F);
	static void setD2dContext(ComPtr<ID2D1DeviceContext>);
	static void setImageFactory(ComPtr<IWICImagingFactory>);
	static void setTextFormat(ComPtr<IDWriteTextFormat> format) { defaultTextFormat = format; }
	virtual void setAttribute(const std::wstring& key, const std::wstring& value) override;
	virtual void addChild(const shared_ptr<Element>&);
	std::shared_ptr<Element> getIthChild(std::size_t i);
	static bool inside(const COORD&, const D2D1_RECT_F&);
	static shared_ptr<Element> createElementByXml(const shared_ptr<xml::Node>&);
	static void setPositionFromAttribute(const shared_ptr<Element>& ret);
	static shared_ptr<Element> createElement(Brush, D2D1_RECT_F);
	static Brush createBrushFromAttribute(const shared_ptr<Attribute>&);
	static std::shared_ptr<Element> getElementByID(const std::shared_ptr<Element>& root, const std::wstring & str);
	Element();
};