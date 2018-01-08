
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
