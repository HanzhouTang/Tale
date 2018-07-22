#include"AppBase.h"

LRESULT AppBase::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static AppBase* me = nullptr;
	if (message == WM_CREATE) {
		LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
		me = (AppBase *)pcs->lpCreateParams;
	}
	if (me)
		return me->RealWndProc(hwnd, message, wParam, lParam);
	else
		return DefWindowProc(hwnd, message, wParam, lParam);
}

AppBase::AppBase(int height, int width, HINSTANCE hinstance, bool fullScreen, std::wstring caption)
{
	windowClassName = L"AppBaseClass";
	this->caption = caption;
	this->fullScreen = fullScreen;
	this->windowHeight = height;
	this->windowWidth = width;
	this->hInstance = hinstance;
}


bool AppBase::InitApp()
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = windowClassName.c_str();
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL,
			_T("Call to RegisterClassEx failed!"),
			_T("Win32 Guided Tour"),
			NULL);
		return false;
	}

	mainWindow = CreateWindow(
		windowClassName.c_str(),
		caption.c_str(),
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT,
		windowWidth, windowHeight,
		NULL,
		NULL,
		hInstance,
		this
	);
	if (!mainWindow)
	{
		MessageBox(NULL,
			_T("Call to CreateWindow failed!"),
			_T("Win32 Guided Tour"),
			NULL);
		return false;
	}
	return true;
}


bool AppBase::ShowWindow(int nCmdShow)
{
	if (!mainWindow)
	{
		MessageBox(NULL,
			_T("Call to CreateWindow failed!"),
			_T("Win32 Guided Tour"),
			NULL);
		return false;
	}
	::ShowWindow(mainWindow, nCmdShow);
	UpdateWindow(mainWindow);
	return true;

}

void AppBase::StartMessageLoop()
{
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

}

void AppBase::OnResize(int width, int height)
{
	windowHeight = height;
	windowWidth = width;
}

LRESULT AppBase::RealWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	using std::wcout;
	using std::cout;
	using std::endl;
	switch (message)
	{
	case WM_SIZE: 
	{
		UINT width = LOWORD(lParam);
		UINT height = HIWORD(lParam);
		OnResize(width, height);
		OnDraw();
		//InvalidateRect(hwnd, NULL, FALSE);
		return 0; 
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_MOUSEMOVE:
		OnMouseMove(wParam, lParam);
		return 0;
	case WM_LBUTTONDOWN:
		OnLButtonDown(wParam, lParam);
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}


