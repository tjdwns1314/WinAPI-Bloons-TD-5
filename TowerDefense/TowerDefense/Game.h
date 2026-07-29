#pragma once

#include "Singleton.h"

class Game : public Singleton<Game>
{
	friend Singleton<Game>;

public:
	void Init(HWND hwnd);
	void Cleanup();
	void Update();
	void Render();

	HWND GetHwnd() const { return _hwnd; }
	
private :
	Game() = default;
	~Game() = default;

private :
	HWND _hwnd;
	RECT _rect;

	HDC _hdc;
	HDC _hdcBack;
	HBITMAP _bmpBack;

	Graphic _graphic;
	//IWICImagingFactory* p_wic_factory = nullptr;
};

