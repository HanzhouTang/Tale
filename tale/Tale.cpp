#include"Tale.h"
#include<windowsx.h>

Tale::Tale(int height, int width, HINSTANCE hinstance, bool fullScreen, std::wstring caption)
	:AppBase(height, width, hinstance, fullScreen, caption)
{
}



Tale::~Tale() {
	root.reset();
	m_pD3dDevice.Reset();
	m_pD3dContext.Reset();
	m_pD2dDevice.Reset();
	m_pD2dContext.Reset();
	m_pDxgiAdapter.Reset();
	m_pDxgiDevice.Reset();
	m_pDxgiFactory.Reset();
	m_pBackBuffer.Reset();
	m_pD2dBackBuffer.Reset();
	m_pSwapChain.Reset();
	m_pImageFactory.Reset();
	m_pDWriteFactory.Reset();
	m_pDirect2dFactory.Reset();
	m_pTextFormat.Reset();
	m_pTextBrush.Reset();
}


void Tale::updateScene(float dt) {
	static float timeElapse = 0.0f;
	static int frames = 0;
	frames += 1;
	timeElapse += dt;
	if (timeElapse > 1.0f) {
		fps = L"FPS: " + std::to_wstring(frames);
		timeElapse = 0.0f;
		frames = 0;
	}
}

void Tale::StartMessageLoop() {
	MSG  msg;
	msg.message = WM_NULL;

	__int64 cntsPerSec = 0;
	QueryPerformanceFrequency((LARGE_INTEGER*)&cntsPerSec);
	float secsPerCnt = 1.0f / (float)cntsPerSec;

	__int64 prevTimeStamp = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&prevTimeStamp);

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			__int64 currTimeStamp = 0;
			QueryPerformanceCounter((LARGE_INTEGER*)&currTimeStamp);
			dt = (currTimeStamp - prevTimeStamp)*secsPerCnt;
			updateScene(dt);
			OnDraw();
			prevTimeStamp = currTimeStamp;
		}
	}
}

bool Tale::InitDirectX()
{
	CoInitialize(NULL);
	if (FAILED(CoCreateInstance(
		CLSID_WICImagingFactory,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&m_pImageFactory))))
		return false;
	if (FAILED(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory),
		reinterpret_cast<IUnknown**>(m_pDWriteFactory.GetAddressOf()))))
		return false;
	UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
	const D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1
	};
	if (FAILED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, 0,
		creationFlags, featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION,
		&m_pD3dDevice, NULL, &m_pD3dContext))) {
		return false;
	}
	if (FAILED(m_pD3dDevice.As(&m_pDxgiDevice))) {
		return false;
	}
	D2D1_FACTORY_OPTIONS options;
	ZeroMemory(&options, sizeof(D2D1_FACTORY_OPTIONS));
	if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED,
		__uuidof(ID2D1Factory1), &options, &m_pDirect2dFactory))) {
		return false;
	}
	if (FAILED(m_pDirect2dFactory->CreateDevice(m_pDxgiDevice.Get(), &m_pD2dDevice))) {
		return false;
	}
	if (FAILED(m_pD2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &m_pD2dContext))) {
		return false;
	}
	if (FAILED(m_pDxgiDevice->GetAdapter(&m_pDxgiAdapter))) {
		return false;
	}
	if (FAILED(m_pDxgiAdapter->GetParent(IID_PPV_ARGS(&m_pDxgiFactory)))) {
		return false;
	}
	if (FAILED(m_pD2dContext->CreateSpriteBatch(m_pSpriteBatch.GetAddressOf()))) {
		return false;
	}
	// Describe Windows 7-compatible Windowed swap chain
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = { 0 };
	swapChainDesc.Width = 0;
	swapChainDesc.Height = 0;
	swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	swapChainDesc.Stereo = false;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 2;
	swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;
	if (FAILED(m_pDxgiFactory->CreateSwapChainForHwnd(m_pD3dDevice.Get(),AppBase::GetMainWindow(),
		&swapChainDesc, nullptr, nullptr, &m_pSwapChain)))
	{
		return false;
	}
	if (FAILED(m_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&m_pBackBuffer)))) {
		return false;
	}
	FLOAT dpiX, dpiY;
	m_pDirect2dFactory->GetDesktopDpi(&dpiX, &dpiY);
	D2D1_BITMAP_PROPERTIES1 bitmapProperties =
		D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
			D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE), dpiX, dpiY);
	if (FAILED(m_pD2dContext->CreateBitmapFromDxgiSurface(m_pBackBuffer.Get()
		, &bitmapProperties, &m_pD2dBackBuffer))) {
		return false;
	}
	m_pD2dContext->SetTarget(m_pD2dBackBuffer.Get());
	if (!CreateDependentRescource())
		return false;


	RECT rc;
	::GetClientRect(GetMainWindow(), &rc);
	screenSize = D2D1::RectF(
		0, 0,
		rc.right - rc.left,
		rc.bottom - rc.top
	);

	parameters.DirtyRectsCount = 0;
	parameters.pDirtyRects = nullptr;
	parameters.pScrollRect = nullptr;
	parameters.pScrollOffset = nullptr;

	return true;
}
//need refactor this function !!!





HRESULT Tale::LoadBitmapFromFile(
	ID2D1DeviceContext *pD2dContext,
	IWICImagingFactory *pIWICFactory,
	PCWSTR uri,
	ID2D1Bitmap **ppBitmap
)
{
	ComPtr<IWICBitmapDecoder> pDecoder = NULL;
	ComPtr<IWICBitmapFrameDecode> pSource = NULL;
	ComPtr<IWICStream> pStream = NULL;
	ComPtr<IWICFormatConverter> pConverter = NULL;
	ComPtr<IWICBitmapScaler> pScaler = NULL;

	HRESULT hr = pIWICFactory->CreateDecoderFromFilename(
		uri,
		NULL,
		GENERIC_READ,
		WICDecodeMetadataCacheOnLoad,
		&pDecoder
	);

	if (SUCCEEDED(hr))
	{
		// Create the initial frame.
		hr = pDecoder->GetFrame(0, &pSource);
	}
	if (SUCCEEDED(hr))
	{

		// Convert the bitmap format to 32bppPBGRA
		// (DXGI_FORMAT_B8G8R8A8_UNORM + D2D1_ALPHA_MODE_PREMULTIPLIED).
		hr = pIWICFactory->CreateFormatConverter(&pConverter);

	}


	if (SUCCEEDED(hr))
	{
		hr = pConverter->Initialize(
			pSource.Get(),
			GUID_WICPixelFormat32bppPBGRA,
			WICBitmapDitherTypeNone,
			NULL,
			0.f,
			WICBitmapPaletteTypeMedianCut
		);
	}   if (SUCCEEDED(hr))
	{

		// Create a Direct2D bitmap from the WIC bitmap.
		hr = pD2dContext->CreateBitmapFromWicBitmap(
			pConverter.Get(),
			NULL,
			ppBitmap
		);
	}

	pDecoder.Reset();
	pSource.Reset();
	pStream.Reset();
	pConverter.Reset();
	pScaler.Reset();
	return hr;
}




bool Tale::CreateDependentRescource()
{


	if (FAILED(m_pDWriteFactory->CreateTextFormat(
		L"Gabriola",                // Font family name.
		NULL,                       // Font collection (NULL sets it to use the system font collection).
		DWRITE_FONT_WEIGHT_MEDIUM,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_MEDIUM,
		21.0f,
		L"en-us",
		&m_pTextFormat)))
		return false;
	m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	m_pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	if (FAILED(m_pD2dContext->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Red),
		&m_pTextBrush)))
		return false;

	return true;
}

bool Tale::initRootScene() {
	Element::setD2dContext(m_pD2dContext.Get());
	ComPtr<ID2D1BitmapBrush> background(CreateBitmapBrushFromFile(L"Resource\\Image\\background\\bg_06a.jpg"));
	auto position = D2D1::RectF(0, 0, Element::MaximumRealtiveRatio, Element::MaximumRealtiveRatio);
	Element::Brush bBackground(Element::BrushType::bitmap, background);
	root = Element::createElement(bBackground, position);
	Element::Brush ares_a(Element::BrushType::bitmap, CreateBitmapBrushFromFile(L"Resource\\icon\\table\\aries_a.png"));
	Element::Brush ares_b(Element::BrushType::bitmap, CreateBitmapBrushFromFile(L"Resource\\icon\\table\\aries_b.png"));
	Element::Brush hearts_a(Element::BrushType::bitmap, CreateBitmapBrushFromFile(L"Resource\\icon\\table\\hearts_a.png"));
	Element::Brush hearts_b(Element::BrushType::bitmap, CreateBitmapBrushFromFile(L"Resource\\icon\\table\\hearts_b.png"));
	Element::Brush scales_a(Element::BrushType::bitmap, CreateBitmapBrushFromFile(L"Resource\\icon\\table\\scales_a.png"));
	Element::Brush scales_b(Element::BrushType::bitmap, CreateBitmapBrushFromFile(L"Resource\\icon\\table\\scales_b.png"));
	Element::Brush ying_yang_a(Element::BrushType::bitmap, CreateBitmapBrushFromFile(L"Resource\\icon\\table\\yin-yang_a.png"));
	Element::Brush ying_yang_b(Element::BrushType::bitmap, CreateBitmapBrushFromFile(L"Resource\\icon\\table\\yin-yang_b.png"));
	Element::Brush triorb_a(Element::BrushType::bitmap, CreateBitmapBrushFromFile(L"Resource\\icon\\table\\triorb_a.png"));
	Element::Brush triorb_b(Element::BrushType::bitmap, CreateBitmapBrushFromFile(L"Resource\\icon\\table\\triorb_b.png"));
	Element::Brush sward_spade_a(Element::BrushType::bitmap, CreateBitmapBrushFromFile(L"Resource\\icon\\table\\sword-spade_a.png"));
	Element::Brush sward_spade_b(Element::BrushType::bitmap, CreateBitmapBrushFromFile(L"Resource\\icon\\table\\sword-spade_b.png"));
	auto Bares = Button::createButton(ares_a, D2D1::RectF(0, 0, 0, 0));
	Bares->setmouseHoverBrush(ares_b);
	auto Bheart = Button::createButton(hearts_a, D2D1::RectF(0, 0, 0, 0));
	Bheart->setmouseHoverBrush(hearts_b);
	auto Bscale= Button::createButton(scales_a, D2D1::RectF(0, 0, 0, 0));
	Bscale->setmouseHoverBrush(scales_b);
	auto Bying_yang = Button::createButton(ying_yang_a, D2D1::RectF(0, 0, 0, 0));
	Bying_yang->setmouseHoverBrush(ying_yang_b);
	auto Btriorb= Button::createButton(triorb_a, D2D1::RectF(0, 0, 0, 0));
	Btriorb->setmouseHoverBrush(triorb_b);
	auto Bsward_spade = Button::createButton(sward_spade_a, D2D1::RectF(0, 0, 0, 0));
	Bsward_spade->setmouseHoverBrush(sward_spade_b);
	/*ComPtr<ID2D1SolidColorBrush> temp;
	if (FAILED(m_pD2dContext->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Blue), &temp)))
		return false;
	Element::Brush b2(Element::BrushType::solid, temp);
	if (FAILED(m_pD2dContext->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Black), &temp)))
		return false;
	Element::Brush b3(Element::BrushType::solid, temp);
	auto button = Button::createButton(b1,D2D1::RectF(40,30,60,70),b3, m_pTextFormat);
	button->setCaption(L"a simple test");
	button->setmouseHoverBrush(b2);
	root->addChild(button);*/
	/*auto sprite = Sprite::createSprite(D2D1::RectF(10, 10, 30, 30), 0.5,bBackground);
	auto sprite1 = Sprite::createSprite(D2D1::RectF(10, 10, 30, 30), 0.5, bBackground);
	sprite->addBrush(b1);
	sprite1->addBrush(b1);
	*/
	auto stackPanel = StackPanel::createStackPanel(D2D1::RectF(0, 70, Element::MaximumRealtiveRatio, Element::MaximumRealtiveRatio),StackPanel::Orientation::horizontal);
	Element::Brush scroll(Element::BrushType::bitmap, CreateBitmapBrushFromFile(L"Resource\\icon\\table\\background.jpg"));
	stackPanel->setBackground(scroll);
	/*
	stackPanel->addChild(sprite);
	stackPanel->addChild(sprite1);
	*/
	stackPanel->addChild(Bares);
	stackPanel->addChild(Bheart);
	stackPanel->addChild(Bscale);
	stackPanel->addChild(Bying_yang);
	stackPanel->addChild(Btriorb);
	stackPanel->addChild(Bsward_spade);
	root->addChild(stackPanel);
	Element::Brush character(Element::BrushType::bitmap, CreateBitmapBrushFromFile(L"Resource\\DevelopQHead\\100011_01.png"));
	auto  sprite = Sprite::createSprite(D2D1::RectF(0, 20, 30, 70), 0.5, character);
	root->addChild(sprite);
	return true;
}

void Tale::OnDraw() {
	HRESULT result;
	m_pD2dContext->BeginDraw();
	m_pD2dContext->Clear(D2D1::ColorF(D2D1::ColorF::Black));
	if (root) {
		root->onDraw(screenSize,dt);
	}

	D2D1_RECT_F rect = D2D1::RectF(
		0, 0, 100, 100
	);
	m_pD2dContext->DrawText(fps.c_str(), fps.length(), m_pTextFormat.Get(), rect, m_pTextBrush.Get());
	m_pSwapChain->Present1(1, 0, &parameters);
	if (FAILED(result = m_pD2dContext->EndDraw())) {
		cout << "drawing fault" << endl;
		assert(1 == 2);
	}
}

void Tale::OnMouseMove(WPARAM wParam, LPARAM lParam) {
	COORD position;
	position.X = GET_X_LPARAM(lParam);
	position.Y = GET_Y_LPARAM(lParam);
	if (root) {
		root->update(Element::MouseMessage(Element::Event::MouseMove, position), screenSize);
	}
}

void Tale::OnLButtonDown(WPARAM wParam, LPARAM lParam) {
	COORD position;
	position.X = GET_X_LPARAM(lParam);
	position.Y = GET_Y_LPARAM(lParam);
	if (root) {
		root->update(Element::MouseMessage(Element::Event::LButtonDown, position), screenSize);
	}
}

ID2D1BitmapBrush* Tale::CreateBitmapBrushFromFile(PCWSTR url) {
	ComPtr<ID2D1Bitmap> temp;
	if (FAILED(LoadBitmapFromFile(m_pD2dContext.Get(), m_pImageFactory.Get(),
		url, temp.GetAddressOf()))) {
		assert(1 == 2);
		return nullptr;
	}
	ID2D1BitmapBrush* bitmap;
	if (FAILED(m_pD2dContext->CreateBitmapBrush(temp.Get(), &bitmap))) {
		assert(1 == 2);
		return nullptr;
	}
	return bitmap;
}


//TODO bitmap brush auto scale 