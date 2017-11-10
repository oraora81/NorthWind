
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

	virtual ~NtColorShader() override;

	bool Initialize(const ntWchar* vs, const ntWchar* ps) override;

    bool InitializeFx(const ntWchar* fx) override;

    bool InitializeFxTwoInputElem(const ntWchar* fx);

    bool InitializeFx32bitColor(const ntWchar* fx);

    void Release() override;

	bool Render(ntInt indexCount, const XMMATRIX& worldMatrix, const XMMATRIX& viewMatrix, const XMMATRIX& projMatrix);

	bool RenderFx(ntInt indexCount, const XMMATRIX& worldViewProj);

private:
	bool InitializeShader(const ntWchar* vs, const ntWchar* ps);

	bool SetShaderParameters(const XMMATRIX& worldMatrix, const XMMATRIX& viewMatrix, const XMMATRIX& projMatrix);

	void RenderShader(int indexCount) const;

private:
	ID3D11Buffer*		m_matrixBuffer;
    //ID3DX11EffectScalarVariable* m_fxTime;
};

} }
