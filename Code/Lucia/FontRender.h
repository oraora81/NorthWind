
#pragma once

namespace NT
{

namespace _d2d
{

const float STANDARD_DPI = 96.0f;

class CFontRenderer
{
public:
	CFontRenderer();
	~CFontRenderer();

	bool Initialize(HWND hwnd);
	bool Release();

	void Render();

private:
	HWND m_hwnd;

	float m_dpiScaleWidth;
	float m_dpiScaleHeight;

	// Direct2D
	ID2D1Factory*	m_d2dFactory;
	ID2D1HwndRenderTarget*	m_d2dRenderTarget;
	ID2D1SolidColorBrush*	m_brushColor;

	// DirectWrite
	IDWriteFactory*	m_dwFactory;
	IDWriteTextFormat*	m_dwTextFormat;
};

}	// namespace _d2d

}	// namespace NT
