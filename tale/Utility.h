#pragma once
#include<iostream>
#include<string>
#include<vector>
#include<sstream>
#include"d2dutil.h"
namespace Utility {
	
	std::vector<float> wstr2floats(const std::wstring& str);
	HRESULT LoadBitmapFromFile(
		ID2D1DeviceContext *pD2dContext,
		IWICImagingFactory *pIWICFactory,
		PCWSTR uri,
		ID2D1Bitmap **ppBitmap
	);
	ID2D1BitmapBrush* CreateBitmapBrushFromFile(ID2D1DeviceContext *pD2dContext,
		IWICImagingFactory *pIWICFactory, PCWSTR url);
	
}