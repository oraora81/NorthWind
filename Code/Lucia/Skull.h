#pragma once

#include "NtModel.h"

class Skull : public nt::renderer::NtModel
{
public:
    Skull();
    virtual ~Skull();

    virtual void Update(float deltaTime) override;

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
    float m_theta;
    float m_phi;
    float m_radius;
    int m_skullIndexCount;
};
