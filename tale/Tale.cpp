#include"Tale.h"
#include<windowsx.h>

Tale::Tale(int height, int width, HINSTANCE hinstance, bool fullScreen, std::wstring caption)
	:AppBase(height, width, hinstance, fullScreen, caption)
{
}

Tale::~Tale() {
	root.reset();
	m_pImageFactory.Reset();
	m_pDWriteFactory.Reset();
	m_pDirect2dFactory.Reset();
	m_pRenderTarget.Reset();
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
			float dt = (currTimeStamp - prevTimeStamp)*secsPerCnt;
			updateScene(dt);
			OnDraw();
			prevTimeStamp = currTimeStamp;
		}
	}
}


bool Tale::InitD2d()
{
	CoInitialize(NULL);
	if (FAILED(CoCreateInstance(
		CLSID_WICImagingFactory,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&m_pImageFactory))))
		return false;
	if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, m_pDirect2dFactory.GetAddressOf())))
		return false;
	if (FAILED(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory),
		reinterpret_cast<IUnknown**>(m_pDWriteFactory.GetAddressOf()))))
		return false;
	if (!CreateDependentRescource())
		return false;
	auto size = m_pRenderTarget->GetSize();
	screenSize.left = 0;
	screenSize.top = 0;
	screenSize.right = size.width;
	screenSize.bottom = size.height;
	return true;
}
//When resize, autoscaling the whole scene? Direct2d natively support?

bool Tale::CreateDependentRescource()
{

	if (!m_pRenderTarget) {
		RECT rc;
		::GetClientRect(GetMainWindow(), &rc);
		D2D1_SIZE_U size = D2D1::SizeU(
			rc.right - rc.left,
			rc.bottom - rc.top
		);
		HRESULT hr;
		if (FAILED(hr = m_pDirect2dFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(GetMainWindow(), size),
			&m_pRenderTarget)))
		{
			return false;
		}
		Element::setRenderTarget(m_pRenderTarget.Get());
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
		if (FAILED(m_pRenderTarget->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF::White),
			&m_pTextBrush)))
			return false;
		if (!initRootScene()) {
			return false;
		}
	}
	return true;
}

bool Tale::initRootScene() {

	auto position = D2D1::RectF(0, 0, Element::MaximumRealtiveRatio, Element::MaximumRealtiveRatio);
	ComPtr<ID2D1SolidColorBrush> temp;
	if (FAILED(m_pRenderTarget->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Gold), &temp)))
		return false;
	Element::Brush b(Element::BrushType::solid, temp);
	root = Element::createElement(b, position);
	
	position.left = 40;
	position.right = 60;
	position.top = 30;
	position.bottom = 70;
	
	if (FAILED(m_pRenderTarget->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::MediumSpringGreen), &temp)))
		return false;
	Element::Brush b1(Element::BrushType::solid, temp);
	if (FAILED(m_pRenderTarget->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Blue), &temp)))
		return false;
	Element::Brush b2(Element::BrushType::solid, temp);
	if (FAILED(m_pRenderTarget->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Black), &temp)))
		return false;
	Element::Brush b3(Element::BrushType::solid, temp);
	auto button = Button::createButton(b1,b3,position,m_pTextFormat);
	button->setCaption(L"a simple test");
	//button->setCallbackFunction([&](Button*) {MessageBox(GetMainWindow(), L"A test of button", L"Test", 0); });
	button->setmouseHoverBrush(b2);
	root->addChild(button);
	return true;
}

void Tale::OnDraw() {
	m_pRenderTarget->BeginDraw();
	m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));
	if (root) {
		root->onDraw(screenSize);
	}
	D2D1_RECT_F rect = D2D1::RectF(
		0, 0, 100, 100
	);
	m_pRenderTarget->DrawText(fps.c_str(), fps.length(), m_pTextFormat.Get(), rect, m_pTextBrush.Get());
	m_pRenderTarget->EndDraw();
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

void Tale::OnResize(int width, int height) {
	AppBase::OnResize(width, height);
	D2D1_SIZE_U size;
	size.height = height;
	size.width = width;
	m_pRenderTarget->Resize(size);
}