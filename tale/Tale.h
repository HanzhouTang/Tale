#pragma once
#include"AppBase.h"
#include"Element.h"
#include"Button.h"
class Tale:public AppBase {
public:
	Tale(int height, int width, HINSTANCE hinstance,
		bool fullScreen = false, std::wstring caption = L"Tale");
	virtual ~Tale();
	bool InitDirectX();
	virtual void StartMessageLoop() override;
	bool initRootScene();
protected:
	std::wstring fps;
	shared_ptr<Element> root;
	D2D1_RECT_F screenSize;
	ComPtr<ID3D11Device> m_pD3dDevice;
	ComPtr<ID3D11DeviceContext> m_pD3dContext;
	ComPtr<ID2D1Device> m_pD2dDevice;
	ComPtr<ID2D1DeviceContext> m_pD2dContext;
	ComPtr<IDXGIAdapter> m_pDxgiAdapter;
	ComPtr<IDXGIDevice> m_pDxgiDevice; 
	ComPtr<IDXGISurface> m_pBackBuffer;
	ComPtr<ID2D1Bitmap1> m_pD2dBackBuffer;
	ComPtr<ID2D1Factory1> m_pDirect2dFactory;
	ComPtr<IDWriteFactory> m_pDWriteFactory;
	ComPtr<IDXGIFactory2> m_pDxgiFactory;
	ComPtr<IDXGISwapChain1> m_pSwapChain;
	ComPtr<IWICImagingFactory> m_pImageFactory;
	ComPtr<IDWriteTextFormat> m_pTextFormat;
	ComPtr<ID2D1SolidColorBrush> m_pTextBrush;
	ComPtr<ID2D1Bitmap>     m_pBitmpmapTest;
	DXGI_PRESENT_PARAMETERS parameters={0};
	virtual void updateScene(float dt);
	bool CreateDependentRescource();
	virtual void OnDraw() override;
	virtual void OnMouseMove(WPARAM, LPARAM) override;
	virtual void OnLButtonDown(WPARAM, LPARAM) override;
	HRESULT LoadBitmapFromFile(
		ID2D1DeviceContext *pD2dContext,
		IWICImagingFactory *pIWICFactory,
		PCWSTR uri,
		ID2D1Bitmap **ppBitmap
	);
	ID2D1BitmapBrush* CreateBitmapBrushFromFile(PCWSTR url);

};