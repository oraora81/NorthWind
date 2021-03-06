
#pragma once

namespace nt { namespace renderer {

class NtDx11Renderer : public NtD3DRenderer
{
public:
	NtDx11Renderer();
	virtual ~NtDx11Renderer() override;

	virtual bool Initialize(int width, int height, HWND hwnd, bool vsync, bool fullscreen, bool enable4xMsaa, float screenDepth, float screenNear) override;

	virtual void Release() override;

	virtual void BeginScene(const float* color) override;

	virtual void EndScene() override;

	virtual bool Resize(ntInt width, ntInt height) override;

	ID3D11Device* Device() const;

	ID3D11DeviceContext* DeviceContext() const;

	bool CreateShaderResourceView(ID3D11Texture1D* tex, D3D11_SHADER_RESOURCE_VIEW_DESC* SRVDesc, ID3D11ShaderResourceView** textureView);

	bool CreateShaderResourceView(ID3D11Texture2D* tex, D3D11_SHADER_RESOURCE_VIEW_DESC* SRVDesc, ID3D11ShaderResourceView** textureView);

	bool CreateShaderResourceView(ID3D11Texture3D* tex, D3D11_SHADER_RESOURCE_VIEW_DESC* SRVDesc, ID3D11ShaderResourceView** textureView);

    bool CreateShaderResourceView(ntWchar* fileName, ID3D11ShaderResourceView** SRVMap);

	void* operator new(size_t s)
	{
		return _aligned_malloc(s, 16);
	}

	void operator delete(void* obj)
	{
		_aligned_free(obj);
	}

private:
	IDXGISwapChain*				m_swapchain;

	ID3D11Device*				m_device;

	ID3D11DeviceContext*		m_deviceContext;

	ID3D11RenderTargetView*		m_renderTargetView;

	ID3D11Texture2D*			m_depthStencilBuffer;

	ID3D11DepthStencilState*	m_depthStencilState;

	ID3D11DepthStencilView*		m_depthStencilView;

    ntUint                      m_4xMassQuality;
};

}	// namespace renderer
}	// namespace nt
