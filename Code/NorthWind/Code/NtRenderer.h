
#pragma once

namespace NT
{

namespace RENDERER
{

class NtD3DRenderer;
class NtCamera;
class NtPuppet;
class NtColorShader;
class NtTextureShader;
class NtLightShader;
class NtLight;


class NtRenderer : public NtObject
{
public:
	NtRenderer();
	~NtRenderer();

	bool Initialize(HWND hwnd, NtInt width, NtInt height);
	void Release();
	bool Process();

	// 이하 다이렉트x 9


	// 이하 다이렉트X 11
	// 차후 D3D를 상속받은 NtRenderer에서 호출하게끔 수정필요.
	bool CreateTexture1D(D3D11_TEXTURE1D_DESC* desc, D3D11_SUBRESOURCE_DATA* initData, ID3D11Texture1D** tex);
	bool CreateTexture2D(D3D11_TEXTURE2D_DESC* desc, D3D11_SUBRESOURCE_DATA* initData, ID3D11Texture2D** tex);
	bool CreateTexture3D(D3D11_TEXTURE3D_DESC* desc, D3D11_SUBRESOURCE_DATA* initData, ID3D11Texture3D** tex);
	
	int	GetFeatureLevel();
	std::shared_ptr<NtDirectX11Renderer>& GetD3DRenderer();
	bool CreateBuffer(NtRenderBufferParam& param);
	//bool CreateBuffer(const D3D11_BUFFER_DESC& desc, D3D11_SUBRESOURCE_DATA* resData, ID3D11Buffer** buffer);
	void SetPrimitiveTopology(NtInt topology);
	void SetVertexBuffers(NtUInt startSlot, NtUInt numBuffers, NtVertexBuffer** buffer, NtUInt* stride, NtUInt* offset);
	void SetIndexBuffers(NtIndexBuffer* buffer, NTCOLORFMT format, NtUInt offset);

private:
	bool Render(NtFloat rot);

private:
#if _NT_DX11
	std::shared_ptr<NtDirectX11Renderer> m_renderEngine;
	//NtDirectX11Renderer m_renderEngine;
#elif _NT_DX9
	NtDirectX9Renderer*	m_renderEngine;
#endif
	NtCamera*			m_camera;
	NtPuppet*			m_puppet;
	NtColorShader*		m_colorShader;
	NtTextureShader*	m_textureShader;
	NtLightShader*		m_lightShader;
	NtLight*			m_light;
};

}	// namespace RENDERER

}	// namespace NT
