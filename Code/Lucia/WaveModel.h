#pragma once

#include "NtModel.h"
#include "Waves.h"

class WaveModel : public nt::renderer::NtModel
{
public:
	WaveModel();
	virtual ~WaveModel();

	virtual void Update(float deltaTime) override;

	virtual void RenderColor(XMMATRIX& worldViewProj) override;

	void MakeGeometry();

	void MakeWave();

	float& Theta()
	{
		return m_theta;
	}

	float& Phi()
	{
		return m_phi;
	}

	void Phi(float p)
	{
		m_phi = p;
	}

	float Radius()
	{
		return m_radius;
	}

	void Radius(float r)
	{
		m_radius = r;
	}

private:
	float m_theta;
	float m_phi;
	float m_radius;

	XMFLOAT4X4 m_gridWorld;
	XMFLOAT4X4 m_wavesWorld;
	ID3D11Buffer* m_waveVB;
	ID3D11Buffer* m_waveIB;

	UINT m_gridIndexCount;
	Waves m_waves;
};