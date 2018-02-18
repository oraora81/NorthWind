#pragma once

#include "NtModel.h"
#include "Waves.h"

using namespace nt::renderer;

class WaveModel : public nt::renderer::NtModel
{
public:
	WaveModel();
	virtual ~WaveModel();

	virtual void Update(float deltaTime) override;

	virtual void Render(XMMATRIX& worldViewProj) override;

	void MakeGeometry();

	void MakeWave();

    void MakeCrate();

private:
	XMFLOAT4X4 m_gridWorld;
	XMFLOAT4X4 m_wavesWorld;
    XMFLOAT4X4 m_boxWorld;
    XMFLOAT4X4 m_grassTexTransform;
    XMFLOAT4X4 m_waterTexTransform;
    XMFLOAT4X4 m_boxTexTransform;

    ID3D11Buffer* m_waveVB;
	ID3D11Buffer* m_waveIB;

    ID3D11Buffer* m_boxVB;
    ID3D11Buffer* m_boxIB;

    ID3D11ShaderResourceView* m_grassMapSRV;
    ID3D11ShaderResourceView* m_waveMapSRV;
    ID3D11ShaderResourceView* m_boxMapSRV;

	UINT m_gridIndexCount;
    UINT m_boxIndexCount;
	Waves m_waves;

    DirectionalLight m_dirLight[3];
    Material m_landMaterial;
    Material m_wavMaterial;
    Material m_boxMaterial;

    XMFLOAT2 m_waterTexOffset;
    int m_lightCount;
};
