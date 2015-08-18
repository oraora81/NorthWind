
#pragma once

#include "Renderer.h"

#include "RenderObj.h"

namespace NT
{
namespace _dx11
{

enum VERTEX_ELEMENT_TYPE
{
	ELEMENT_POSITION = 0x0001,
	ELEMENT_COLOR = 0x0002,
	ELEMENT_NORMAL = 0x0004,
	ELEMENT_UV = 0x0008,
};

class DXMiscContainer
{
public:
	D3D11_INPUT_ELEMENT_DESC* InitMiscDataList(int sumProperty, ...);
	int	GetDXGiByteSize(DXGI_FORMAT formatType);
};


struct cbPerObject
{
	XMMATRIX WVP;
	XMMATRIX World;
};

struct sLight
{
	XMFLOAT3	dir;
	float		pad;
	XMFLOAT3	pos;
	float		range;
	XMFLOAT3	att;
	float		pad2;
	XMFLOAT4	ambient;
	XMFLOAT4	diffuse;
};

struct cbPerFrame
{
	sLight	light;
};


struct sVtxBufferItem
{
	ID3D11Buffer* vtxBuffer;
	unsigned int stride;
	unsigned int vtxCount;
};


class DXRenderer : public IRenderer
{
public:
	DXRenderer();
	~DXRenderer();

	bool Initialize(HWND hwnd);
	bool Release();

	void Render();
	void RenderText(const std::wstring text);
	void Process(const ntFloat& delta);

	// Manage Vertex Data
	bool SetVertexBuffer(unsigned int byteWidth, 
						unsigned int structSize, 
						const void* vtxList,
						int sumProperty);

	bool SetIndexBuffer(unsigned int byteWidth,
						unsigned int structSize,
						const void* idxList);

	bool InitShader();
	bool InitDirectWrite(IDXGIAdapter1* adapter);

	//void RenderText(const std::wstring text);

	void UpdateScene();

	void LookUp();
	void LookDown();

private:
	void InitD2DScreenTexture();

private:
	IDXGISwapChain*			m_d3dSwapChain;
	ID3D11Device*			m_d3dDevice;
	ID3D11DeviceContext*	m_d3dDeviceContext;

	// Back side buffer
	ID3D11RenderTargetView*	m_renderView;

	// shader
	ID3D11VertexShader*		m_VShader;
	ID3D11PixelShader*		m_PShader;
	ID3DBlob*				m_VSBuffer;
	ID3DBlob*				m_PSBuffer;

	// 
	ID3D11Buffer*			m_cbPerFrameBuffer;
	ID3D11PixelShader*		m_d2dPShader;
	ID3D10Blob*				m_d2dPShaderBuffer;

	// Resource (after, It will be Resource Manager)
	std::vector<sVtxBufferItem>	m_vtxBufferList;
	ID3D11Buffer**			m_vtxBuffers;
	ID3D11Buffer*			m_idxBuffer;
	ID3D11InputLayout*		m_layout;

	// depth
	ID3D11DepthStencilView* m_depthStencilView;
	ID3D11Texture2D*		m_depthStencilBuffer;


	// WVP
	ID3D11Buffer*			m_cbPerObjectBuffer;
	XMMATRIX				m_WVP;
	XMMATRIX				m_view;
	XMMATRIX				m_projection;
	cbPerObject				m_cbPerObj;

	cbPerFrame				m_cbPerFrame;

	// relative cam vector's
	XMVECTOR				m_camPosition;
	XMVECTOR				m_camTarget;
	XMVECTOR				m_camup;

	// for font render
	ID3D10Device1*			m_d3dDevice10;

	ID3D11Texture2D*		m_backBuffer;
	ID3D11Texture2D*		m_sharedTexture;
	IDXGIKeyedMutex*		m_keyedMutex10;
	IDXGIKeyedMutex*		m_keyedMutex11;
	ID2D1RenderTarget*		m_d2dRenderTarget;
	ID2D1SolidColorBrush*	m_currentBrush;

	IDWriteFactory*			m_dwriteFactory;
	IDWriteTextFormat*		m_dwriteTextFormat;

	ID3D11RasterizerState*	m_CCWCullMode;
	ID3D11RasterizerState*	m_CWCullMode;
	ID3D11BlendState*		m_transparency;

	ID3D11RasterizerState*	m_noCull;

	ID3D11ShaderResourceView*	m_d2dTexture;
	ID3D11Buffer*				m_d2dVtxBuffer;
	ID3D11Buffer*				m_d2dIdxBuffer;

	// Custom DataType
	DXMiscContainer			m_misc;
	XMMATRIX				m_renderObj[2];
	unsigned int			m_vertexCount;
	unsigned int			m_indexCount;

	// @@@ plz remove
	XMMATRIX				m_rotation;
	XMMATRIX				m_scale;
	XMMATRIX				m_translation;
	float					m_rot;

	// Rasterizer State
	ID3D11RasterizerState*	m_wireFrame;

	// texture
	ID3D11ShaderResourceView* m_texture;
	ID3D11SamplerState*		m_texSamplerState;

	// light
	sLight					m_light;

public:
	ntInt					m_fps;
};

// Normal Function
HRESULT CompileShader(LPCWSTR fileName, char* entryPoint, char* shaderVersion, ID3DBlob** blobPtr);

}	// namespace _dx11
}	// namespace NT