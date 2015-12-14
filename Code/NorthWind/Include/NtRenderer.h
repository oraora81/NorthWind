
#pragma once

namespace nt { namespace renderer {

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

	bool Initialize(HWND hwnd, ntInt width, ntInt height);
	void Release();
	bool Process();

	// ���� ���̷�Ʈx 9


	// ���� ���̷�ƮX 11
	// ���� D3D�� ��ӹ��� NtRenderer���� ȣ���ϰԲ� �����ʿ�.
	bool CreateTexture1D(D3D11_TEXTURE1D_DESC* desc, D3D11_SUBRESOURCE_DATA* initData, ID3D11Texture1D** tex);
	bool CreateTexture2D(D3D11_TEXTURE2D_DESC* desc, D3D11_SUBRESOURCE_DATA* initData, ID3D11Texture2D** tex);
	bool CreateTexture3D(D3D11_TEXTURE3D_DESC* desc, D3D11_SUBRESOURCE_DATA* initData, ID3D11Texture3D** tex);
	
	int	GetFeatureLevel();
	std::shared_ptr<NtDirectX11Renderer>& GetD3DRenderer();
	bool CreateBuffer(NtRenderBufferParam& param);
	//bool CreateBuffer(const D3D11_BUFFER_DESC& desc, D3D11_SUBRESOURCE_DATA* resData, ID3D11Buffer** buffer);
	void SetPrimitiveTopology(ntInt topology);
	void SetVertexBuffers(ntUint startSlot, ntUint numBuffers, NtVertexBuffer** buffer, ntUint* stride, ntUint* offset);
	void SetIndexBuffers(NtIndexBuffer* buffer, NTCOLORFMT format, ntUint offset);

private:
	bool Render(ntFloat rot);

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

}	// namespace renderer

}	// namespace nt
