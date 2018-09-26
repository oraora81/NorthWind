#pragma once

namespace nt { namespace renderer {

class NtTreeSpriteShader : public NtShader
{
public:
    NtTreeSpriteShader();

    virtual ~NtTreeSpriteShader() override;

    bool InitializeFx(const ntWchar* fx) override;

    void Release() override;
};

} }
