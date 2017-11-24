
#pragma once

namespace nt { namespace renderer {

class NtD3DRenderer
{
public:
	NtD3DRenderer();

	virtual ~NtD3DRenderer();

	virtual bool Initialize(int width, int height, HWND hwnd, bool vsync, bool fullscreen, bool enableMsaa, float screenDepth, float screenNear) = 0;

	virtual void Release() = 0;

	virtual void BeginScene(const float* color) = 0;

	virtual void EndScene() = 0;

	virtual bool Resize(ntInt width, ntInt height) = 0;

	virtual void SetRenderStateSolid() const = 0;

	virtual void SetRenderStateWireframe() const = 0;
	
	void SetWorldMatrix(XMMATRIX& world);

	void SetViewMatrix(XMMATRIX& view);

	void SetProjectionMatrix(XMMATRIX& proj);

	void GetWorldMatrix(XMMATRIX& world);

	void GetViewMatrix(XMMATRIX& view);

	void GetProjectionMatrix(XMMATRIX& proj);

	void Transform(XMMATRIX& tm);

	void GetOrthoMatrix(XMMATRIX& ortho);

	void GetDeviceCardInfo(NtString& cardName, ntInt& memory);

	const ntChar* GetVShaderModel() const;

	const ntChar* GetPShaderModel() const;

	const ntChar* GetFxShaderModel() const;

	ntInt VideoMemory() const;

	// error Report
	void OutputShaderErrorMessage(ID3D10Blob* errMsg, const ntWchar* fileName);

private:
	// prevent obj copy
	NtD3DRenderer(const NtD3DRenderer&);

	NtD3DRenderer& operator = (NtD3DRenderer&);

protected:
	ntInt m_videoCardMemory;
	NtString m_videoCardDescription;
	ntChar*	m_shaderModel[3];

	XMFLOAT4X4 m_world;
	XMFLOAT4X4 m_view;
	XMFLOAT4X4 m_proj;
	XMMATRIX m_ortho;

	ntUint m_width;
	ntUint m_height;
	ntUint m_refreshRate;
	ntUint m_multiSamples;

	bool m_vsync;
	bool m_windowMode;

	static XMMATRIX ms_identity;
};

}	// namespace renderer

}	// namespace nt
