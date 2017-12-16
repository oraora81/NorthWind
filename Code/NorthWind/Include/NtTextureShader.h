
#pragma once

namespace nt { namespace renderer {

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
	virtual ~NtTextureShader() override;

    bool InitializeFx(const ntWchar* fx) override;

    void Release() override;

	bool Render(int indexCount, const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& proj, ntUint handle);

private:
	bool InitializeShader(const ntWchar* vs, const ntWchar* ps);

	bool SetShaderParameter(const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& proj, ntUint handle);

	void RenderShader(ntInt indexCount);

private:
	ID3D11Buffer*		m_matrixBuffer;
	ID3D11SamplerState*	m_sampleState;
};

} }

