#pragma once
#include<iostream>
#include<string>
#include<vector>
#include<sstream>
#include<locale>
#include<codecvt>
#include"d2dutil.h"
namespace Utility {

	static const std::wstring ELEMENT_EN = L"Element";
	static const std::wstring ELEMENT_CH = L"元素";
	static const std::wstring POSITION_EN = L"position";
	static const std::wstring POSITION_CH = L"位置";
	static const std::wstring BRUSH_EN = L"brush";
	static const std::wstring BRUSH_CH = L"画刷";
	static const std::wstring BUTTON_EN = L"Button";
	static const std::wstring BUTTON_CH = L"按钮";
	static const std::wstring STACKPANEL_EN = L"StackPanel";
	static const std::wstring STACKPANEL_CH = L"栈布局";
	static const std::wstring MOUSEHOVERBRUSH_EN = L"mouseHoverBrush";
	static const std::wstring MOUSEHOVERBRUSH_CH = L"鼠标悬浮画刷";
	static const std::wstring PADDING_EN = L"padding";
	static const std::wstring PADDING_CH = L"填充";
	static const std::wstring ORIENTATION_EN = L"orienation";
	static const std::wstring ORIENTATION_CH = L"方向";
	static const std::wstring VERTICAL_EN = L"vertical";
	static const std::wstring VERTICAL_CH = L"垂直";
	static const std::wstring HORIZONTAL_EN = L"horizontal";
	static const std::wstring HORIZONTAL_CH = L"水平";
	static const std::wstring SPRITE_EN = L"Sprite";
	static const std::wstring SPRITE_CH = L"精灵";
	static const std::wstring INTERVAL_EN = L"interval";
	static const std::wstring INTERVAL_CH = L"间隔";
	static const std::wstring ITEM_EN = L"Item";
	static const std::wstring ITEM_CH = L"项";
	static const std::wstring SCRIPT = L"Script";
	static const std::wstring RootName = L"_root_";
	static const std::wstring ID = L"id";
	static const std::wstring TextBlock_EN = L"TextBlock";
	static const std::wstring TextBlock_CH = L"文字块";
	std::wstring rtrim(std::wstring &s);
	std::vector<float> wstr2floats(const std::wstring& str);
	HRESULT LoadBitmapFromFile(
		ID2D1DeviceContext *pD2dContext,
		IWICImagingFactory *pIWICFactory,
		PCWSTR uri,
		ID2D1Bitmap **ppBitmap
	);
	ID2D1BitmapBrush* CreateBitmapBrushFromFile(ID2D1DeviceContext *pD2dContext,
		IWICImagingFactory *pIWICFactory, PCWSTR url);
	void quitWithError(int lineNumber, const char* filename, std::wstring error);
	void warning(std::wstring info);
	std::wstring str2wstr(const std::string& str);
	std::string wstr2str(const std::wstring& wstr);
	struct wcout_redirect {
		wcout_redirect(std::wstreambuf * new_buffer)
			: old(std::wcout.rdbuf(new_buffer))
		{ }
		~wcout_redirect() {
			std::wcout.rdbuf(old);
		}
	private:
		std::wstreambuf * old;
	};
}