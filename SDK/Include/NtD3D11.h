
#pragma once

namespace nt
{

namespace renderer
{

class NtDirectX11Renderer : public NtD3DRenderer
{
public:
	NtDirectX11Renderer();
	~NtDirectX11Renderer();

	virtual bool Initialize(int width, int height, HWND hwnd, bool vsync, bool fullscreen, float screenDepth, float screenNear);
	virtual void Release();

	virtual void BeginScene(float r, float g, float b, float a);
	virtual void EndScene();

	ID3D11Device* Device() const;
	ID3D11DeviceContext* DeviceContext() const;

	bool CreateShaderResourceView(ID3D11Texture1D* tex, D3D11_SHADER_RESOURCE_VIEW_DESC* SRVDesc, ID3D11ShaderResourceView** textureView);
	bool CreateShaderResourceView(ID3D11Texture2D* tex, D3D11_SHADER_RESOURCE_VIEW_DESC* SRVDesc, ID3D11ShaderResourceView** textureView);
	bool CreateShaderResourceView(ID3D11Texture3D* tex, D3D11_SHADER_RESOURCE_VIEW_DESC* SRVDesc, ID3D11ShaderResourceView** textureView);

private:
	IDXGISwapChain*				m_swapchain;
	ID3D11Device*				m_device;
	ID3D11DeviceContext*		m_deviceContext;
	ID3D11RenderTargetView*		m_renderTargetView;
	ID3D11Texture2D*			m_depthStencilBuffer;
	ID3D11DepthStencilState*	m_depthStencilState;
	ID3D11DepthStencilView*		m_depthStencilView;
	ID3D11RasterizerState*		m_rasterState;
};

}	// namespace renderer
}	// namespace nt
