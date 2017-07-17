
#pragma once

namespace nt { namespace renderer {

class NtD3DRenderer;
class NtCamera;
class NtModel;
class NtColorShader;
class NtTextureShader;
class NtLightShader;
class NtLight;


class NtRenderer : public NtObject
{
public:
	NtRenderer();

	virtual ~NtRenderer() override;

	bool Initialize(HWND hwnd, ntInt width, ntInt height, bool fullScreen);

	void Release();

	bool Process(const ntFloat deltaTime);

	bool Draw();

	bool DrawTest();

	bool Resize(ntInt width, ntInt height);

	// 차후 D3D를 상속받은 NtRenderer에서 호출하게끔 수정필요.
	bool CreateTexture1D(D3D11_TEXTURE1D_DESC* desc, D3D11_SUBRESOURCE_DATA* initData, ID3D11Texture1D** tex);

	bool CreateTexture2D(D3D11_TEXTURE2D_DESC* desc, D3D11_SUBRESOURCE_DATA* initData, ID3D11Texture2D** tex);

	bool CreateTexture3D(D3D11_TEXTURE3D_DESC* desc, D3D11_SUBRESOURCE_DATA* initData, ID3D11Texture3D** tex);
	
	int	GetFeatureLevel() const;

	bool CreateBuffer(NtRenderBufferParam& param);

	//bool CreateBuffer(const D3D11_BUFFER_DESC& desc, D3D11_SUBRESOURCE_DATA* resData, ID3D11Buffer** buffer);

	void SetPrimitiveTopology(ntInt topology);

	void SetRenderState(eRenderState state);

	void SetVertexBuffers(ntUint startSlot, ntUint numBuffers, NtVertexBuffer** buffer, ntUint* stride, ntUint* offset);

	void SetIndexBuffers(NtIndexBuffer* buffer, NTCOLORFMT format, ntUint offset);

	bool AddModel(NtModel* model);

private:
	std::shared_ptr<NtDx11Renderer> m_renderEngine;
	NtCamera*			m_camera;
	NtModel*			m_puppet;
	NtColorShader*		m_colorShader;
	NtTextureShader*	m_textureShader;
	NtLightShader*		m_lightShader;
	NtLight*			m_light;

	std::vector<NtModel*> m_models;
};

} }
