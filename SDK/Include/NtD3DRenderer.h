
#pragma once

namespace NT
{

namespace RENDERER
{

class NtD3DRenderer
{
public:
	NtD3DRenderer();
	virtual ~NtD3DRenderer();

	virtual bool Initialize(int width, int height, HWND hwnd, bool vsync, bool fullscreen, float screenDepth, float screenNear) = 0;
	virtual void Release() = 0;

	virtual void BeginScene(float r, float g, float b, float a) = 0;
	virtual void EndScene() = 0;
	
	void GetWorldMatrix(XMMATRIX& world);
	void GetProjectionMatrix(XMMATRIX& proj);
	void GetOrthoMatrix(XMMATRIX& ortho);


	//
	void GetDeviceCardInfo(NtString& cardName, ntInt& memory);

	//
	const ntChar* GetVShaderModel() const;
	const ntChar* GetPShaderModel() const;

	// 
	void SetMultiSamples(ntUint sampleCount);

	// error Report
	void OutputShaderErrorMessage(ID3D10Blob* errMsg, const ntWchar* fileName);

private:
	// prevent obj copy
	NtD3DRenderer(const NtD3DRenderer&);
	NtD3DRenderer& operator = (NtD3DRenderer&);

protected:
	ntInt		m_videoCardMemory;
	NtString	m_videoCardDescription;
	ntChar*		m_shaderModel[2];

	XMMATRIX	m_world;
	XMMATRIX	m_proj;
	XMMATRIX	m_ortho;

	ntUint		m_width;
	ntUint		m_height;
	ntUint		m_refreshRate;
	ntUint		m_multiSamples;

	bool		m_vsync;
	bool		m_windowMode;
};

}	// namespace RENDERER

}	// namespace NT
