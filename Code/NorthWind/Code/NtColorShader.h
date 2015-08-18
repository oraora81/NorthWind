
#pragma once

namespace NT
{
namespace RENDERER
{

class NtD3DRenderer;

class NtColorShader : public NtShader
{
	struct NtMatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX proj;
	};

public:
	NtColorShader();
	~NtColorShader();

	bool Initialize(const ntWchar* vs, const ntWchar* ps);
	void Release();
	bool Render(ntInt indexCount, const XMMATRIX& worldMatrix, const XMMATRIX& viewMatrix, const XMMATRIX& projMatrix);
	bool RenderLine(const XMMATRIX& worldMatrix, const XMMATRIX& viewMatrix, const XMMATRIX& projMatrix);

private:
	bool InitializeShader(const ntWchar* vs, const ntWchar* ps);
	void ReleaseShader();
	bool SetShaderParameters(const XMMATRIX& worldMatrix, const XMMATRIX& viewMatrix, const XMMATRIX& projMatrix);
	void RenderShader(int indexCount);


private:
	ID3D11VertexShader*	m_vertexShader;
	ID3D11PixelShader*	m_pixelShader;
	ID3D11InputLayout*	m_layout;
	ID3D11Buffer*		m_matrixBuffer;
};

}	// namespace RENDERER
}	// namespace NT
