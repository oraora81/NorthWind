#pragma once

#include "NtModel.h"

class Box : public nt::renderer::NtModel
{
public:
	Box();
	virtual ~Box();

	virtual void Update(float deltaTime) override;

    virtual void RenderColor(XMMATRIX& worldViewProj) override;

	void MakeGeometry();
    
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
    void MakeNormal();

    void MakeGeometryTwoVertexBuf();

    void RenderNormal(XMMATRIX& worldViewProj);

    void RenderTwoVertexBuf(XMMATRIX& worldViewProj);

private:
	float m_theta;
	float m_phi;
	float m_radius;
    ID3D11Buffer* m_CB;
    ID3D11Buffer* m_buffers[2];
    int m_colorCount;
    XMFLOAT4X4 m_world;
};
