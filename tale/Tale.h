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
	bool initRootScene();
protected:
	std::wstring fps;
	shared_ptr<Element> root;
	D2D1_RECT_F screenSize;
	ComPtr<ID2D1Factory> m_pDirect2dFactory;
	ComPtr<IDWriteFactory> m_pDWriteFactory;
	ComPtr<ID2D1HwndRenderTarget> m_pRenderTarget;
	ComPtr<IWICImagingFactory> m_pImageFactory;
	ComPtr<IDWriteTextFormat> m_pTextFormat;
	ComPtr<ID2D1SolidColorBrush> m_pTextBrush;
	ComPtr<ID2D1Bitmap>     m_pBitmpmapTest;
	virtual void updateScene(float dt);
	bool CreateDependentRescource();
	virtual void OnDraw() override;
	virtual void OnResize(int width, int height) override;
	virtual void OnMouseMove(WPARAM, LPARAM) override;
	virtual void OnLButtonDown(WPARAM, LPARAM) override;
	HRESULT LoadBitmapFromFile(
		ID2D1RenderTarget *pRenderTarget,
		IWICImagingFactory *pIWICFactory,
		PCWSTR uri,
		ID2D1Bitmap **ppBitmap
	);

};