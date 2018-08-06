#include"Utility.h"
namespace Utility {
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
			std::wcout << "ERROR: cannot load image "<< url << std::endl;
			assert(1 == 2);
			return nullptr;
		}

		ID2D1BitmapBrush* bitmap;
		if (FAILED(context->CreateBitmapBrush(temp.Get(), &bitmap))) {
			assert(1 == 2);
			return nullptr;
		}
		return bitmap;
	}
};
