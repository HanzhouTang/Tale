#include"Utility.h"
#include<Shlwapi.h>
#include<algorithm>
namespace Utility {
	std::wstring rtrim(std::wstring & s)
	{
		s.erase(std::find_if(s.rbegin(), s.rend(), [](wchar_t ch) {
			return (ch != L' '&&ch != L'\t');
		}).base(), s.end());
		return s;
	}
	std::vector<float> wstr2floats(const std::wstring& str) {
		std::wstringstream ss(str);
		float temp;
		std::vector<float> ret;
		while (ss >> temp) {
			ret.push_back(temp);
		}
		return ret;
	}

	HRESULT LoadBitmapFromFile(
		ID2D1DeviceContext *pD2dContext,
		IWICImagingFactory *pIWICFactory,
		PCWSTR uri,
		ID2D1Bitmap **ppBitmap
	)
	{
		Microsoft::WRL::ComPtr<IWICBitmapDecoder> pDecoder = NULL;
		Microsoft::WRL::ComPtr<IWICBitmapFrameDecode> pSource = NULL;
		Microsoft::WRL::ComPtr<IWICStream> pStream = NULL;
		Microsoft::WRL::ComPtr<IWICFormatConverter> pConverter = NULL;
		Microsoft::WRL::ComPtr<IWICBitmapScaler> pScaler = NULL;

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


	ID2D1BitmapBrush* CreateBitmapBrushFromFile(ID2D1DeviceContext* context,
		IWICImagingFactory *imageFactory, PCWSTR url) {
		Microsoft::WRL::ComPtr<ID2D1Bitmap> temp;
		if (FAILED(LoadBitmapFromFile(context, imageFactory,
			url, temp.GetAddressOf()))) {
			quitWithError(__LINE__, __FILE__, std::wstring(L"cannot load image ")+url);
			return nullptr;
		}

		ID2D1BitmapBrush* bitmap;
		if (FAILED(context->CreateBitmapBrush(temp.Get(), &bitmap))) {
			quitWithError(__LINE__, __FILE__, L"cannot create bitmap");
			return nullptr;
		}
		return bitmap;
	}
	
	std::wstring str2wstr(const std::string& str) {
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		return converter.from_bytes(str);
	}

	std::string wstr2str(const std::wstring& wstr) {
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		return converter.to_bytes(wstr);
	}

	void quitWithError(int lineNumber, const char* filename, std::wstring error)
	{
		std::wostringstream stream;
		stream << "Error: " << error << "\nlINE " << lineNumber << " FILE " << PathFindFileNameA(filename);
		MessageBoxW(NULL, stream.str().c_str(),L"Error", MB_OK);
		exit(-1);
	}
	void warning(std::wstring info) {
		std::wcout <<"Warning: " <<info << std::endl;
	}
};
