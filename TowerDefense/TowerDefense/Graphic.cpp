#include "pch.h"
#include "Graphic.h"
void Graphic::Init(HWND hwnd)
{
	CoInitialize(nullptr);

	D2D1CreateFactory(
		D2D1_FACTORY_TYPE_SINGLE_THREADED,
		&_factory);
	CoCreateInstance(
		CLSID_WICImagingFactory,
		nullptr,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&_wicFactory));

}

void Graphic::Cleanup()
{
	if (_wicFactory)
	{
		_wicFactory->Release();
		_wicFactory = nullptr;
	}

	if (_renderTarget)
	{
		_renderTarget->Release();
		_renderTarget = nullptr;
	}

	if (_factory)
	{
		_factory->Release();
		_factory = nullptr;
	}
	CoUninitialize();
}

ID2D1Bitmap* Graphic::LoadBitmap(const wchar_t* fileName)
{
	IWICBitmapDecoder* decoder = nullptr;
	_wicFactory->CreateDecoderFromFilename(
		fileName,
		nullptr,
		GENERIC_READ,
		WICDecodeMetadataCacheOnLoad,
		&decoder);
	IWICBitmapFrameDecode* frame = nullptr;
	decoder->GetFrame(0, &frame);

	IWICFormatConverter* converter = nullptr;
	_wicFactory->CreateFormatConverter(&converter);

	converter->Initialize(
		frame,
		GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone,
		nullptr,
		0.f,
		WICBitmapPaletteTypeMedianCut);

	ID2D1Bitmap* bitmap = nullptr;
	_renderTarget->CreateBitmapFromWicBitmap(
		converter,
		nullptr,
		&bitmap);

	converter->Release();
	frame->Release();
	decoder->Release();

	return bitmap;

}