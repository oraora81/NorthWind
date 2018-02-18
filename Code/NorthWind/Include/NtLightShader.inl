
void NtLightShader::SetWorld(CXMMATRIX m)
{
    m_fxWorld->SetMatrix(reinterpret_cast<const float*>(&m));
}

void NtLightShader::SetWorldInvTranspose(CXMMATRIX m)
{
    m_fxWorldInvTranspose->SetMatrix(reinterpret_cast<const float*>(&m));
}

void NtLightShader::SetTexTransform(CXMMATRIX m)
{
    m_fxTexTransform->SetMatrix(reinterpret_cast<const float*>(&m));
}

void NtLightShader::SetEyePosW(XMFLOAT3 eyePosW)
{
    m_fxEyePosW->SetRawValue(&eyePosW, 0, sizeof(XMFLOAT3));
}

void NtLightShader::SetDirLights(const DirectionalLight* light)
{
    m_fxDirLight->SetRawValue(light, 0, sizeof(DirectionalLight) * 3);
}

void NtLightShader::SetMaterial(const Material& mat)
{
    m_fxMaterial->SetRawValue(&mat, 0, sizeof(Material));
}

void NtLightShader::SetDiffuseMap(ID3D11ShaderResourceView* tex)
{
    NtAsserte(tex != nullptr);

    m_fxDiffuseMap->SetResource(tex);
}

void NtLightShader::SetBlendMap(ID3D11ShaderResourceView* tex)
{
    NtAsserte(tex != nullptr);

    //m_fxBlendMap->SetResource(tex);
}

void NtLightShader::SetFogColor(const FXMVECTOR v)
{
    m_fogColor->SetFloatVector(reinterpret_cast<const float*>(&v));
}

void NtLightShader::SetFogStart(float f)
{
    m_fogStart->SetFloat(f);
}

void NtLightShader::SetFogRange(float f)
{
    m_fogRange->SetFloat(f);
}

const ID3DX11EffectMatrixVariable* NtLightShader::FxWorld() const
{
    return m_fxWorld;
}

const ID3DX11EffectMatrixVariable* NtLightShader::FxWorldInvTranspose() const
{
    return m_fxWorldInvTranspose;
}

ID3DX11EffectTechnique* NtLightShader::Light1Tech() const
{
    return m_light1;
}

ID3DX11EffectTechnique* NtLightShader::Light2Tech() const
{
    return m_light2;
}

ID3DX11EffectTechnique* NtLightShader::Light3Tech() const
{
    return m_light3;
}

ID3DX11EffectTechnique* NtLightShader::Light0TexTech() const
{
    return m_light0Tex;
}

ID3DX11EffectTechnique* NtLightShader::Light1TexTech() const
{
    return m_light1Tex;
}

ID3DX11EffectTechnique* NtLightShader::Light2TexTech() const
{
    return m_light2Tex;
}

ID3DX11EffectTechnique* NtLightShader::Light3TexTech() const
{
    return m_light3Tex;
}

ID3DX11EffectTechnique* NtLightShader::Light0TexAlphaClipTech() const
{
    return m_light0TexAlphaClipTech;
}

ID3DX11EffectTechnique* NtLightShader::Light1TexAlphaClipTech() const
{
    return m_light1TexAlphaClipTech;
}

ID3DX11EffectTechnique* NtLightShader::Light2TexAlphaClipTech() const
{
    return m_light2TexAlphaClipTech;
}

ID3DX11EffectTechnique* NtLightShader::Light3TexAlphaClipTech() const
{
    return m_light3TexAlphaClipTech;
}

ID3DX11EffectTechnique* NtLightShader::Light1FogTech() const
{
    return m_light1FogTech;
}

ID3DX11EffectTechnique* NtLightShader::Light2FogTech() const
{
    return m_light2FogTech;
}

ID3DX11EffectTechnique* NtLightShader::Light3FogTech() const
{
    return m_light3FogTech;
}

ID3DX11EffectTechnique* NtLightShader::Light0TexFogTech() const
{
    return m_light0TexFogTech;
}

ID3DX11EffectTechnique* NtLightShader::Light1TexFogTech() const
{
    return m_light1TexFogTech;
}

ID3DX11EffectTechnique* NtLightShader::Light2TexFogTech() const
{
    return m_light2TexFogTech;
}

ID3DX11EffectTechnique* NtLightShader::Light3TexFogTech() const
{
    return m_light3TexFogTech;
}


ID3DX11EffectTechnique* NtLightShader::Light0TexAlphaClipFogTech() const
{
    return m_light0TexAlphaClipFogTech;
}

ID3DX11EffectTechnique* NtLightShader::Light1TexAlphaClipFogTech() const
{
    return m_light1TexAlphaClipFogTech;
}

ID3DX11EffectTechnique* NtLightShader::Light2TexAlphaClipFogTech() const
{
    return m_light2TexAlphaClipFogTech;
}

ID3DX11EffectTechnique* NtLightShader::Light3TexAlphaClipFogTech() const
{
    return m_light3TexAlphaClipFogTech;
}
