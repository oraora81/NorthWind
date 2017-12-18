
#pragma once

namespace nt { namespace renderer {

class NtD3DRenderer;
class NtTexture;

class NtTextureShader : public NtShader
{
public:
	
	NtTextureShader();

	virtual ~NtTextureShader() override;

	bool InitializeFx(const ntWchar* fx) override;

	void Release() override;

private:

	void RenderShader(ntInt indexCount);

private:
	ID3D11SamplerState*	m_sampleState;
};

} }
