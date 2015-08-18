
#include "stdafx.h"
#pragma hdrstop

#include "FontRender.h"

namespace NT
{

namespace _d2d
{

CFontRenderer::CFontRenderer()
: m_dpiScaleWidth(0.0f)
, m_dpiScaleHeight(0.0f)
, m_d2dFactory(nullptr)
, m_d2dRenderTarget(nullptr)
, m_brushColor(nullptr)
, m_dwFactory(nullptr)
, m_dwTextFormat(nullptr)
{

}

CFontRenderer::~CFontRenderer()
{
	Release();
}

bool CFontRenderer::Initialize(HWND hwnd)
{
	m_hwnd = hwnd;

	HDC screenDC = GetDC(0);
	m_dpiScaleWidth = GetDeviceCaps(screenDC, LOGPIXELSX) / STANDARD_DPI;
	m_dpiScaleHeight = GetDeviceCaps(screenDC, LOGPIXELSY) / STANDARD_DPI;
	ReleaseDC(0, screenDC);


	// Direct2d Factory
	HRESULT result = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_d2dFactory);
	if (FAILED(result))
	{
		return false;
	}

	// Direct Write Factory
	result = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory),
		reinterpret_cast<IUnknown**>(&m_dwFactory));

	if (FAILED(result))
	{
		return false;
	}

	// Create TextFormat
	result = m_dwFactory->CreateTextFormat(L"굴림",
		NULL,
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		12.0f,
		L"ko",
		&m_dwTextFormat);

	if (FAILED(result))
	{
		return false;
	}

	m_dwTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	m_dwTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	// Create Render Target
	RECT rt;
	GetClientRect(hwnd, &rt);
	D2D1_SIZE_U screenSize = D2D1::SizeU(rt.right - rt.left, rt.bottom - rt.top);

	result = m_d2dFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(hwnd, screenSize),
		&m_d2dRenderTarget);
	if (FAILED(result))
	{
		return false;
	}


	// Create Brush
	result = m_d2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &m_brushColor);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

bool CFontRenderer::Release()
{
	SAFE_RELEASE(m_d2dFactory);
	SAFE_RELEASE(m_d2dRenderTarget);
	SAFE_RELEASE(m_brushColor);
	SAFE_RELEASE(m_dwFactory);
	SAFE_RELEASE(m_dwTextFormat);

	return true;
}

void CFontRenderer::Render()
{
	m_d2dRenderTarget->BeginDraw();
	m_d2dRenderTarget->SetTransform(D2D1::IdentityMatrix());
	m_d2dRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));


	RECT rt;
	GetClientRect(m_hwnd, &rt);

	D2D1_RECT_F layout = D2D1::RectF(
		static_cast<float>(rt.left) / m_dpiScaleWidth,
		static_cast<float>(rt.top) / m_dpiScaleHeight,
		100.0f, 50.f);

	m_d2dRenderTarget->DrawText(L"테스트", wcslen(L"테스트"),
		m_dwTextFormat, layout, m_brushColor);

	m_d2dRenderTarget->EndDraw();
}

}	// namespace _d2d

}	// namespace NT
