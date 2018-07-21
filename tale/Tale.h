#pragma once
#include"AppBase.h"
#include"Element.h"
class Tale:public AppBase {
public:
	Tale(int height, int width, HINSTANCE hinstance,
		bool fullScreen = false, std::wstring caption = L"Tale");
	virtual ~Tale();
	bool InitD2d();
	virtual void StartMessageLoop() override;

protected:
	std::wstring fps;
	Element* root;
	D2D1_RECT_F screenSize;
	ID2D1Factory * m_pDirect2dFactory;
	IDWriteFactory* m_pDWriteFactory;
	ID2D1HwndRenderTarget* m_pRenderTarget;
	IWICImagingFactory *m_pImageFactory;
	IDWriteTextFormat* m_pTextFormat;
	ID2D1SolidColorBrush* m_pTextBrush;
	bool initRootScene();
	virtual void updateScene(float dt);
	bool CreateDependentRescource();
	virtual void OnDraw() override;

};