#pragma once

#include "NtModel.h"

class Box : public nt::renderer::NtModel
{
public:
	Box();
	virtual ~Box();

	virtual void Update(float deltaTime) override;

private:
	float m_theta;
	float m_phi;
	float m_radius;
};
