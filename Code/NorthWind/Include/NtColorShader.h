
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

    bool InitializeFx(const ntWchar* fx) override;

    void Release() override;

	bool RenderFx(ntInt indexCount, const XMMATRIX& worldViewProj);

private:

public:
    ID3DX11EffectTechnique* ColorTech;

private:
	ID3D11Buffer*		m_matrixBuffer;
    //ID3DX11EffectScalarVariable* m_fxTime;
};

} }
