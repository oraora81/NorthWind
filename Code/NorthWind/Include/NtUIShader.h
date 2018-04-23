#pragma once

namespace nt { namespace renderer {

class NtUIShader : public NtShader
{
public:
    NtUIShader();

    virtual ~NtUIShader() override;

    bool InitializeFx(const ntWchar* fx) override;

    void Release() override;
};

}
}
