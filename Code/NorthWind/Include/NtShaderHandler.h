#pragma once

namespace nt { namespace renderer {

class NtShaderHandler
{
public:
    static bool Initialize();
    static void Release();

public:
    static std::unique_ptr<NtLightShader> LightShader;
};

} }
