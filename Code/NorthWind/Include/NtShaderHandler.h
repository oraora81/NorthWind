#pragma once

namespace nt { namespace renderer {

class NtSampleShader;

class NtShaderHandler
{
public:
    static bool Initialize();
    static void Release();

public:
    static std::unique_ptr<NtLightShader> LightShader;
    static std::unique_ptr<NtColorShader> ColorShader;
    static std::unique_ptr<NtSampleShader> SampleLightShader;
};

} }
