#pragma once

#include "NtModel.h"

class BaseShape : public nt::renderer::NtModel
{
public:
    BaseShape();
    virtual ~BaseShape();

    virtual void Update(float deltaTime) override;

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

protected:
    float m_theta;
    float m_phi;
    float m_radius;
};
