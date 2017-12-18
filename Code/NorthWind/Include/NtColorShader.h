
#pragma once

namespace nt { namespace renderer {

class NtD3DRenderer;

class NtColorShader : public NtShader
{
public:
	NtColorShader();

	virtual ~NtColorShader() override;

	bool InitializeFx(const ntWchar* fx) override;

	bool RenderFx(ntInt indexCount, const XMMATRIX& worldViewProj);

private:

public:
	ID3DX11EffectTechnique* ColorTech;

private:
	//ID3DX11EffectScalarVariable* m_fxTime;
};

} }
