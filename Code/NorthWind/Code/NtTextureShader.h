
#pragma once

namespace NT
{
namespace RENDERER
{
class NtD3DRenderer;
class NtTexture;

class NtTextureShader : public NtShader
{
	struct NtMatrixBuffer
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

public:
	NtTextureShader();
	~NtTextureShader();

	virtual bool Initialize(const NtWChar* vs, const NtWChar* ps);
	void Release();
	bool Render(int indexCount, const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& proj, NtUInt handle);

private:
	bool InitializeShader(const NtWChar* vs, const NtWChar* ps);
	void ReleaseShader();
	bool SetShaderParameter(const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& proj, NtUInt handle);
	void RenderShader(int indexCount);

private:
	ID3D11VertexShader*	m_vertexShader;
	ID3D11PixelShader*	m_pixelShader;
	ID3D11InputLayout*	m_layout;
	ID3D11Buffer*		m_matrixBuffer;
	ID3D11SamplerState*	m_sampleState;
};

}
}
