#pragma once
#include"AppBase.h"
#include"Element.h"
#include"Button.h"
#include"Sprite.h"
#include"StackPanel.h"
#include"SimpleXMLParser.h"
#include<fstream>
#include<cwctype>
#include<codecvt>
class Tale:public AppBase {
public:
	Tale(int height, int width, HINSTANCE hinstance,
		bool fullScreen = false, std::wstring caption = L"Tale");
	virtual ~Tale();
	bool InitDirectX();
	virtual void StartMessageLoop() override;
	bool initRootScene();
	wstring readFile(wstring filename);
	float dt;
protected:
	std::wstring fps;
	shared_ptr<Element> root;
	D2D1_RECT_F logicScreenSize;
	D2D1_RECT_F realScreenSize;
	ComPtr<ID3D11Device> m_pD3dDevice;
	ComPtr<ID3D11DeviceContext> m_pD3dContext;
	ComPtr<ID2D1Device3> m_pD2dDevice;
	ComPtr<ID2D1DeviceContext3> m_pD2dContext;
	ComPtr<IDXGIAdapter> m_pDxgiAdapter;
	ComPtr<IDXGIDevice> m_pDxgiDevice; 
	ComPtr<IDXGISurface> m_pBackBuffer;
	ComPtr<ID2D1Bitmap1> m_pD2dBackBuffer;
	ComPtr<ID2D1Factory4> m_pDirect2dFactory;
	ComPtr<IDWriteFactory> m_pDWriteFactory;
	ComPtr<IDXGIFactory2> m_pDxgiFactory;
	ComPtr<IDXGISwapChain1> m_pSwapChain;
	ComPtr<ID2D1SpriteBatch> m_pSpriteBatch;
	ComPtr<IWICImagingFactory> m_pImageFactory;
	ComPtr<IDWriteTextFormat> m_pTextFormat;
	ComPtr<ID2D1SolidColorBrush> m_pTextBrush;
	DXGI_PRESENT_PARAMETERS parameters={0};
	virtual void updateScene(float dt);
	bool CreateDependentRescource();
	virtual void OnDraw() override;
	virtual void OnMouseMove(WPARAM, LPARAM) override;
	virtual void OnLButtonDown(WPARAM, LPARAM) override;
	virtual void OnResize(int width, int height) override;
	

};