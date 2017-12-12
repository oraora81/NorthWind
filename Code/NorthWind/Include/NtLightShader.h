
#pragma once

namespace nt { namespace renderer {


class NtTexture;

class NtLightShader : public NtShader
{
	struct NtMatrixBuffer
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX proj;
	};

	struct NtLightBuffer
	{
		XMFLOAT4A diffuseColor;
		XMFLOAT3A lightDirection;
		float padding;
	};

public:
	NtLightShader();

	virtual ~NtLightShader() override;

    bool InitializeFx(const ntWchar* fx) override;

	void Release() override;

    bool RenderFx(ntInt indexCount, const XMMATRIX& worldViewProj);

public:
    inline void SetWorld(CXMMATRIX m);
    inline void SetWorldInvTranspose(CXMMATRIX m);
    inline void SetEyePosW(XMFLOAT3 eyePosW);
    inline void SetDirLights(const DirectionalLight* light);
    inline void SetMatrial(const Material& mat);
    inline ID3DX11EffectTechnique* Light1Tech() const;
    inline const ID3DX11EffectTechnique* Light2Tech() const;
    inline const ID3DX11EffectTechnique* Light3Tech() const;

private:
    void ReleaseShader();

private:
    ID3DX11EffectTechnique* m_light1;
    ID3DX11EffectTechnique* m_light2;
    ID3DX11EffectTechnique* m_light3;

    ID3DX11EffectMatrixVariable* m_fxWorld;
    ID3DX11EffectMatrixVariable* m_fxWorldInvTranspose;

    ID3DX11EffectVariable* m_fxDirLight;
    ID3DX11EffectVariable* m_fxMaterial;
    ID3DX11EffectVectorVariable* m_fxEyePosW;

	ID3D11SamplerState* m_sampleState;
	ID3D11Buffer* m_matrixBuffer;
	ID3D11Buffer* m_lightBuffer;
};

#include "NtLightShader.inl"

} }
