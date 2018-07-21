#pragma once
#include"d2dutil.h"
class AppBase {
public:
	static LRESULT CALLBACK WndProc(
		HWND   hwnd,
		UINT   uMsg,
		WPARAM wParam,
		LPARAM lParam
	);
	AppBase(int height, int width, HINSTANCE hinstance, bool fullScreen=false, std::wstring caption=L"App");
	virtual ~AppBase(){}
	bool InitApp();
	bool ShowWindow(int);
	virtual void StartMessageLoop();
	inline HWND GetMainWindow() { return mainWindow; }
	inline HINSTANCE GetInstance() { return hInstance; }
	LRESULT RealWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
protected:
	HINSTANCE hInstance;
	HWND mainWindow;
	std::wstring caption;
	std::wstring windowClassName;
	bool fullScreen;
	int windowHeight;
	int windowWidth;
	virtual void OnDraw() {}
	virtual void OnResize(int width, int height);
};

// my design isn't perfect, but not worst either.