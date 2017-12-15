#pragma once

#include "BaseShape.h"

class Pyramid : public nt::renderer::NtModel
{
public:
    Pyramid();
    virtual ~Pyramid();

    void MakeGeometry();
};
