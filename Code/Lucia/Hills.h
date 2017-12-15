#pragma once

#include "NtModel.h"

class Hills : public nt::renderer::NtModel
{
public:
	Hills();
	virtual ~Hills();

	void MakeGeometry();
};
