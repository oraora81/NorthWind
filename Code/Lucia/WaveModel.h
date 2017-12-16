#pragma once

#include "NtModel.h"
#include "Waves.h"

class WaveModel : public nt::renderer::NtModel
{
public:
	WaveModel();
	virtual ~WaveModel();

	virtual void Update(float deltaTime) override;

	virtual void Render(XMMATRIX& worldViewProj) override;

	void MakeGeometry();

	void MakeWave();

private:
	XMFLOAT4X4 m_gridWorld;
	XMFLOAT4X4 m_wavesWorld;
	ID3D11Buffer* m_waveVB;
	ID3D11Buffer* m_waveIB;

	UINT m_gridIndexCount;
	Waves m_waves;
    nt::DirectionalLight m_dirLight[3];
    nt::Material m_landMaterial;
    nt::Material m_wavMaterial;

    ID3DX11EffectVariable* m_fxPointLight;
    ID3DX11EffectVariable* m_fxSpotLight;    
};
