#pragma once
class Graphic
{
public :
	void Init(HWND hwnd);
	void Cleanup();
	ID2D1Bitmap* LoadBitmap(const wchar_t* fileName);
	//void BeginDraw();
	//void EndDraw();

	//ID2D1RenderTarget* GetRenderTarget();
	//IWICImagingFactory* GetWICFactory();
private:
	ID2D1Factory* _factory;
	ID2D1HwndRenderTarget* _renderTarget;
	IWICImagingFactory* _wicFactory;
};

