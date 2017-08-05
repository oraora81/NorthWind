
#pragma once

namespace nt { namespace renderer {

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

	~NtColorShader() override;

	bool Initialize(const ntWchar* vs, const ntWchar* ps) override;

	bool InitializeFx(const ntWchar* fx) override;

	void Release();

	bool Render(ntInt indexCount, const XMMATRIX& worldMatrix, const XMMATRIX& viewMatrix, const XMMATRIX& projMatrix);

	bool RenderFx(ntInt indexCount, const XMMATRIX& worldViewProj);

	bool RenderLine(const XMMATRIX& worldMatrix, const XMMATRIX& viewMatrix, const XMMATRIX& projMatrix);

    const ID3DX11EffectTechnique* GetEffectTechnique();

    const ID3DX11EffectMatrixVariable* GetEffectMatrix();

private:
	bool InitializeShader(const ntWchar* vs, const ntWchar* ps);

	void ReleaseShader();

	bool SetShaderParameters(const XMMATRIX& worldMatrix, const XMMATRIX& viewMatrix, const XMMATRIX& projMatrix);

	void RenderShader(int indexCount) const;

private:
	ID3D11VertexShader*	m_vertexShader;
	ID3D11PixelShader*	m_pixelShader;
	ID3D11InputLayout*	m_layout;
	ID3D11Buffer*		m_matrixBuffer;
	ID3DX11Effect*      m_fx;
	ID3DX11EffectTechnique* m_tech;
	ID3DX11EffectMatrixVariable* m_fxWorldViewProj;
};

} }
