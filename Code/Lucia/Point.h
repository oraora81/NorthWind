#pragma once

#include "BaseShape.h"

class Points : public BaseShape
{
public:
	Points();
	virtual ~Points();

	virtual void Render(XMMATRIX& worldViewProj) override;

	void MakeGeometry();
};
