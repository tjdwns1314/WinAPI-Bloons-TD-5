#include "pch.h"
#include "Graphic.h"
void Graphic::Init(HWND hwnd)
{
	// 1. COM 라이브러리 초기화 (WIC, DIrect2D 기능 사용 준비)
	HRESULT hr = CoInitialize(nullptr);
	if (FAILED(hr))
	{
		// 필요하면 에러 처리 / 로그
		return;
	}

	// 2. Direct2D 공장 객체 생성
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &_factory);
	if (FAILED(hr)) return;

	// 3. WIC(이미지 로더) 공장 객체 생성
	hr = CoCreateInstance(
		CLSID_WICImagingFactory,
		nullptr,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&_wicFactory));
	if (FAILED(hr)) return;

	// 4. 창 크기에 맞춰 Direct2D용 그려질 도화지(_renderTarget) 생성
	RECT rc;
	GetClientRect(hwnd, &rc); // 창의 내부 크기를 구해옴
	_factory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(hwnd, D2D1::SizeU(rc.right, rc.bottom)),
		&_renderTarget);
}

void Graphic::BeginDraw() { _renderTarget->BeginDraw(); }
void Graphic::EndDraw() { _renderTarget->EndDraw(); }
void Graphic::Clear(D2D1::ColorF color) { _renderTarget->Clear(color); }

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

// PNG 파일을 Direct2D가 그릴 수 있는 픽셀 포맷(ID2D1Bitmap)으로 만드는 과정
ID2D1Bitmap* Graphic::LoadBitmap(const wchar_t* fileName)
{
	// 1. 이미지 파일 디코더 생성 (PNG 파일 파싱)
	IWICBitmapDecoder* decoder = nullptr;
	_wicFactory->CreateDecoderFromFilename(
		fileName,
		nullptr,
		GENERIC_READ,
		WICDecodeMetadataCacheOnLoad,
		&decoder);

	// 2. 파일 안의 첫 번째 프레임(이미지 한 장)을 추출
	IWICBitmapFrameDecode* frame = nullptr;
	decoder->GetFrame(0, &frame);

	// 3. 포맷 컨버터 생성 (32비트 PBGRA 투명도 포맷으로 변환)
	IWICFormatConverter* converter = nullptr;
	_wicFactory->CreateFormatConverter(&converter);

	// Direct2D가 가장 좋아하는 32비트 알파 채널(GUID_WICPixelFormat32bppPBGRA) 형태로 변환 초기화
	converter->Initialize(
		frame,
		GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone,
		nullptr,
		0.f,
		WICBitmapPaletteTypeMedianCut);

	// 4. WIC 컨버터를 이용해 Direct2D 전용 비트맵 생성
	ID2D1Bitmap* bitmap = nullptr;
	_renderTarget->CreateBitmapFromWicBitmap(
		converter,
		nullptr,
		&bitmap);
	// 사용한 임시 WIC 메모리 해제 (C++ COM 인터페이스는 사용 후 Release 해줘야 함)
	converter->Release();
	frame->Release();
	decoder->Release();

	return bitmap; // 최종 Direct2D 비트맵 반환

}
