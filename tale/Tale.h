#pragma once
#include"AppBase.h"
#include"Element.h"
#include"Button.h"
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
	ComPtr<ID2D1Factory> m_pDirect2dFactory;
	ComPtr<IDWriteFactory> m_pDWriteFactory;
	ComPtr<ID2D1HwndRenderTarget> m_pRenderTarget;
	ComPtr<IWICImagingFactory> m_pImageFactory;
	ComPtr<IDWriteTextFormat> m_pTextFormat;
	ComPtr<ID2D1SolidColorBrush> m_pTextBrush;
	bool initRootScene();
	virtual void updateScene(float dt);
	bool CreateDependentRescource();
	virtual void OnDraw() override;
	virtual void OnResize(int width, int height) override;
	virtual void OnMouseMove(WPARAM, LPARAM) override;

};