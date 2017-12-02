
void NtLightShader::SetWorld(CXMMATRIX m)
{
    m_fxWorld->SetMatrix(reinterpret_cast<const float*>(m));
}

void NtLightShader::SetWorldInvTranspose(CXMMATRIX m)
{
    m_fxWorldInvTranspose->SetMatrix(reinterpret_cast<const float*>(m));
}

void NtLightShader::SetEyePosW(XMFLOAT3 eyePosW)
{
    m_fxEyePosW->SetRawValue(&eyePosW, 0, sizeof(XMFLOAT3));
}

void NtLightShader::SetDirLights(const DirectionalLight* light)
{
    m_fxDirLight->SetRawValue(&m_fxDirLight, 0, sizeof(DirectionalLight) * 3);
}

void NtLightShader::SetMatrial(const Material& mat)
{
    m_fxMaterial->SetRawValue(&mat, 0, sizeof(Material));
}
