#pragma once

#include "BaseShape.h"

class Points : public nt::renderer::NtModel
{
public:
	Points();
	virtual ~Points();

	virtual void Render(XMMATRIX& worldViewProj) override;

	void MakeGeometry();
};
