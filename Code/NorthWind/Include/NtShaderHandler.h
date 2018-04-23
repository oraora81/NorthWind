#pragma once

namespace nt { namespace renderer {

class NtLightShader;
class NtColorShader;
class NtUIShader;
class NtSampleShader;

class NtShaderHandler
{
public:
    static bool Initialize();
    static void Release();

public:
    static std::unique_ptr<NtLightShader> LightShader;
    static std::unique_ptr<NtColorShader> ColorShader;
    static std::unique_ptr<NtUIShader> UIShader;
    static std::unique_ptr<NtSampleShader> SampleLightShader;
};

} }
