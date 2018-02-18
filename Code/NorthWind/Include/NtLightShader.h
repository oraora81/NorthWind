
#pragma once

namespace nt { namespace renderer {

class NtLightShader : public NtShader
{
	struct NtMatrixBuffer
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX proj;
	};

public:
	NtLightShader();

	virtual ~NtLightShader() override;

	bool InitializeFx(const ntWchar* fx) override;

	void Release() override;

public:
	inline void SetWorld(CXMMATRIX m);

	inline void SetWorldInvTranspose(CXMMATRIX m);

    inline void SetTexTransform(CXMMATRIX m);

	inline void SetEyePosW(XMFLOAT3 eyePosW);

	inline void SetDirLights(const DirectionalLight* light);

	inline void SetMaterial(const Material& mat);

    inline void SetDiffuseMap(ID3D11ShaderResourceView* tex);

    inline void SetBlendMap(ID3D11ShaderResourceView* tex);

    inline void SetFogColor(const FXMVECTOR v);

    inline void SetFogStart(float f);

    inline void SetFogRange(float f);

    inline const ID3DX11EffectMatrixVariable* FxWorld() const;

    inline const ID3DX11EffectMatrixVariable* FxWorldInvTranspose() const;

	inline ID3DX11EffectTechnique* Light1Tech() const;

	inline ID3DX11EffectTechnique* Light2Tech() const;

	inline ID3DX11EffectTechnique* Light3Tech() const;

    inline ID3DX11EffectTechnique* Light0TexTech() const;

    inline ID3DX11EffectTechnique* Light1TexTech() const;

    inline ID3DX11EffectTechnique* Light2TexTech() const;

    inline ID3DX11EffectTechnique* Light3TexTech() const;

    inline ID3DX11EffectTechnique* Light0TexAlphaClipTech() const;

    inline ID3DX11EffectTechnique* Light1TexAlphaClipTech() const;

    inline ID3DX11EffectTechnique* Light2TexAlphaClipTech() const;

    inline ID3DX11EffectTechnique* Light3TexAlphaClipTech() const;

    inline ID3DX11EffectTechnique* Light1FogTech() const;

    inline ID3DX11EffectTechnique* Light2FogTech() const;

    inline ID3DX11EffectTechnique* Light3FogTech() const;

    inline ID3DX11EffectTechnique* Light0TexFogTech() const;

    inline ID3DX11EffectTechnique* Light1TexFogTech() const;

    inline ID3DX11EffectTechnique* Light2TexFogTech() const;

    inline ID3DX11EffectTechnique* Light3TexFogTech() const;

    inline ID3DX11EffectTechnique* Light0TexAlphaClipFogTech() const;

    inline ID3DX11EffectTechnique* Light1TexAlphaClipFogTech() const;

    inline ID3DX11EffectTechnique* Light2TexAlphaClipFogTech() const;

    inline ID3DX11EffectTechnique* Light3TexAlphaClipFogTech() const;

private:
	ID3DX11EffectTechnique* m_light1;
	ID3DX11EffectTechnique* m_light2;
	ID3DX11EffectTechnique* m_light3;

    ID3DX11EffectTechnique* m_light0Tex;
    ID3DX11EffectTechnique* m_light1Tex;
    ID3DX11EffectTechnique* m_light2Tex;
    ID3DX11EffectTechnique* m_light3Tex;

    ID3DX11EffectTechnique* m_light0TexAlphaClipTech;
    ID3DX11EffectTechnique* m_light1TexAlphaClipTech;
    ID3DX11EffectTechnique* m_light2TexAlphaClipTech;
    ID3DX11EffectTechnique* m_light3TexAlphaClipTech;

    ID3DX11EffectTechnique* m_light1FogTech;
    ID3DX11EffectTechnique* m_light2FogTech;
    ID3DX11EffectTechnique* m_light3FogTech;

    ID3DX11EffectTechnique* m_light0TexFogTech;
    ID3DX11EffectTechnique* m_light1TexFogTech;
    ID3DX11EffectTechnique* m_light2TexFogTech;
    ID3DX11EffectTechnique* m_light3TexFogTech;

    ID3DX11EffectTechnique* m_light0TexAlphaClipFogTech;
    ID3DX11EffectTechnique* m_light1TexAlphaClipFogTech;
    ID3DX11EffectTechnique* m_light2TexAlphaClipFogTech;
    ID3DX11EffectTechnique* m_light3TexAlphaClipFogTech;

	ID3DX11EffectMatrixVariable* m_fxWorld;
	ID3DX11EffectMatrixVariable* m_fxWorldInvTranspose;
    ID3DX11EffectMatrixVariable* m_fxTexTransform;

	ID3DX11EffectVariable* m_fxDirLight;
	ID3DX11EffectVariable* m_fxMaterial;
	ID3DX11EffectVectorVariable* m_fxEyePosW;

    ID3DX11EffectShaderResourceVariable* m_fxDiffuseMap;
    //ID3DX11EffectShaderResourceVariable* m_fxBlendMap;

    ID3DX11EffectVectorVariable* m_fogColor;
    ID3DX11EffectScalarVariable* m_fogStart;
    ID3DX11EffectScalarVariable* m_fogRange;

	ID3D11SamplerState* m_sampleState;
	ID3D11Buffer* m_matrixBuffer;
	ID3D11Buffer* m_lightBuffer;
};

#include "NtLightShader.inl"

} }
