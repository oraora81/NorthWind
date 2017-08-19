#pragma once

#include "BaseShape.h"

class Pyramid : public BaseShape
{
public:
    Pyramid();
    virtual ~Pyramid();

    void MakeGeometry();
};
