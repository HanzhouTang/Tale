#include"Tale.h"
#include"NumberExpr.h"
#include"StringExpr.h"
#include"ClosureExpr.h"
#include"VariableExpr.h"
#include"AssignExpr.h"
#include"AddExpr.h"
#include"ReturnExpr.h"
#include"FunctionExpr.h"
#include"GreaterThanExpr.h"
#include"ConditionExpr.h"
#include"EqualExpr.h"
#include"NullExpr.h"
#include"CallExpr.h"
#include"BooleanExpr.h"
#include"ReverseExpr.h"
#include"ExprLiteral.h"
#include"MapExpr.h"
#include"ExternalFunctionExpr.h"
#include"ExprLibrary.h"
#include"SimpleParser.h"
#include"TextBlock.h"
#include<windowsx.h>
using namespace Utility;
Tale::Tale(int height, int width, HINSTANCE hinstance, bool fullScreen, std::wstring caption)
	:AppBase(height, width, hinstance, fullScreen, caption)
{
	std::wcout.imbue(std::locale("chs"));
	SetConsoleOutputCP(936);//gb2312
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
	if (FAILED(m_pDxgiFactory->CreateSwapChainForHwnd(m_pD3dDevice.Get(), AppBase::GetMainWindow(),
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
	realScreenSize = logicScreenSize = D2D1::RectF(
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

bool Tale::CreateDependentRescource()
{
	
	if (FAILED(m_pDWriteFactory->CreateTextFormat(
		L"Gabriola",                // Font family name.
		NULL,                       // Font collection (NULL sets it to use the system font collection).
		DWRITE_FONT_WEIGHT_MEDIUM,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_MEDIUM,
		21.0f,
		L"zh-cn",
		&m_pTextFormat)))
		return false;
	if (FAILED(m_pD2dContext->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Red),
		&m_pTextBrush)))
		return false;
	return true;
}

bool Tale::initRootScene() {
	using namespace expr;
	Element::setD2dContext(m_pD2dContext);
	Element::setImageFactory(m_pImageFactory);
	//Element::setTextFormat(m_pTextFormat);
	wstring content = readFile(L"C:\\Users\\tangh\\source\\repos\\tale\\tale\\Resource\\xml\\initScene1.xml");
	//auto content = readFile(L"initScene.xml");
	unique_ptr<xml::SimpleXMLParser> xmlParser = make_unique<xml::SimpleXMLParser>();
	xmlParser->parse(content);
	auto _root = xmlParser->getRoot();
	if (_root->getChildrenCount() != 1) {
		quitWithError(__LINE__, __FILE__, L" 1 xml file only can have 1 root node");
	}
	auto nodeRoot = _root->getChild(0);
	root = Element::createElementByXml(nodeRoot);
	auto scriptNodes = _root->getNodesByName(Utility::SCRIPT);
	if (scriptNodes.size() > 1) {
		warning(L"For now, only can add one script node in a xml file");
	}
	if (scriptNodes.size() > 0) {
		std::wstring scriptContent = scriptNodes[0]->getValue();
		//wcout <<"("<< scriptContent <<")"<< endl;
		parser::SimpleParser parser(scriptContent);
		parser.init();
		auto closure = parser.state();
		if (closure->getType() != expr::Expr::TYPE_CLOSURE) {
			wcout << closure->toString() << endl;
			quitWithError(__LINE__, __FILE__, L"script tag must contain a closure");
		}
		else {
			setRuntimeEnv(closure);
			setRootElement(closure, root);
			closure->getValue();
		}
	}
	return true;
}

void Tale::OnDraw() {
	HRESULT result;
	m_pD2dContext->BeginDraw();
	m_pD2dContext->Clear(D2D1::ColorF(D2D1::ColorF::Black));
	if (root) {
		root->onDraw(logicScreenSize, dt);
	}

	D2D1_RECT_F rect = D2D1::RectF(
		0, 0, 100, 100
	);
	m_pD2dContext->DrawText(fps.c_str(), fps.length(), m_pTextFormat.Get(), rect, m_pTextBrush.Get());
	m_pSwapChain->Present1(1, 0, &parameters);
	if (FAILED(result = m_pD2dContext->EndDraw())) {
		quitWithError(__LINE__, __FILE__, L"draw fault");
	}
}

void Tale::OnMouseMove(WPARAM wParam, LPARAM lParam) {
	COORD position;
	position.X = GET_X_LPARAM(lParam);
	position.Y = GET_Y_LPARAM(lParam);
	if (root) {
		root->update(Element::MouseMessage(Element::Event::MouseMove, position), realScreenSize);
	}
}

void Tale::OnLButtonDown(WPARAM wParam, LPARAM lParam) {
	COORD position;
	position.X = GET_X_LPARAM(lParam);
	position.Y = GET_Y_LPARAM(lParam);
	if (root) {
		root->update(Element::MouseMessage(Element::Event::LButtonDown, position), realScreenSize);
	}
}

void Tale::OnResize(int width, int height)
{
	using namespace std;
	AppBase::OnResize(width, height);
	RECT rc;
	::GetClientRect(GetMainWindow(), &rc);
	realScreenSize = D2D1::RectF(
		0, 0,
		rc.right - rc.left,
		rc.bottom - rc.top
	);
}


wstring Tale::readFile(wstring filename) {
	std::wifstream read(filename, std::ios::binary);
	read.imbue(std::locale(std::locale("chs"),
		new std::codecvt_utf16<wchar_t, 0x10ffff, std::consume_header>));
	if (!read.is_open()) {
		quitWithError(__LINE__, __FILE__, L"cannot opend file " + filename);
	}
	return wstring(istreambuf_iterator<wchar_t>(read), istreambuf_iterator<wchar_t>());
}

//TODO bitmap brush auto scale finished!