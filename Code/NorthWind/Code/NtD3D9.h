
#pragma once

#ifdef _NT_DX9

namespace nt
{

namespace renderer
{

class NtDirectX9Renderer : public NtD3DRenderer
{
public:
	NtDirectX9Renderer();
	~NtDirectX9Renderer();

	virtual bool Initialize(int width, int height, HWND hwnd, bool vsync, bool fullscreen, float screenDepth, float screenNear);
	virtual void Release();

	virtual void BeginScene(float r, float g, float b, float a);
	virtual void EndScene();


private:
	bool		EnumerateDisplayMode();
	bool		CreateDevice();
	bool		CreateSystemFont();

private:
	HWND				m_hwnd;
	IDirect3D9*			m_driver;
	IDirect3DDevice9*	m_device;
	NTCOLORFMT			m_colorFormat;
	NTCOLORFMT			m_depthStencilFormat;

	// font
	LPD3DXFONT			m_systemFont;

	bool				m_deviceLost;
};

}	// namespace renderer
}	// namespace nt

#endif
