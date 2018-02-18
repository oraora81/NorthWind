
#include "NtCoreLib.h"
#pragma  hdrstop

#include "NtShader.h"
#include "NtLightShader.h"
#include "NtTexture.h"

namespace nt { namespace renderer {

NtLightShader::NtLightShader()
	: NtShader()
	, m_light1(nullptr), m_light2(nullptr), m_light3(nullptr)
    , m_light0Tex(nullptr), m_light1Tex(nullptr), m_light2Tex(nullptr), m_light3Tex(nullptr)
	, m_fxWorld(nullptr)
	, m_fxWorldInvTranspose(nullptr)
    , m_fxTexTransform(nullptr)
	, m_fxDirLight(nullptr)
	, m_fxMaterial(nullptr)
	, m_fxEyePosW(nullptr)
    , m_fxDiffuseMap(nullptr)
    //, m_fxBlendMap(nullptr)
	, m_sampleState(nullptr)
	, m_matrixBuffer(nullptr)
	, m_lightBuffer(nullptr)
{

}

NtLightShader::~NtLightShader()
{
	Release();
}

bool NtLightShader::InitializeFx(const ntWchar* fx)
{
	NtAsserte(fx != nullptr);

	if (NtShader::InitializeFx(fx) == false)
	{
		return false;
	}

	m_light1 = m_fx->GetTechniqueByName("Light1");
	m_light2 = m_fx->GetTechniqueByName("Light2");
	m_light3 = m_fx->GetTechniqueByName("Light3");

    m_light1Tex = m_fx->GetTechniqueByName("Light1Tex");
    m_light0Tex = m_fx->GetTechniqueByName("Light0Tex");
    m_light2Tex = m_fx->GetTechniqueByName("Light2Tex");
    m_light3Tex = m_fx->GetTechniqueByName("Light3Tex");
    
    m_light0TexAlphaClipTech = m_fx->GetTechniqueByName("Light0TexAlphaClip");
    m_light1TexAlphaClipTech = m_fx->GetTechniqueByName("Light1TexAlphaClip");
    m_light2TexAlphaClipTech = m_fx->GetTechniqueByName("Light2TexAlphaClip");
    m_light3TexAlphaClipTech = m_fx->GetTechniqueByName("Light3TexAlphaClip");

    m_light1FogTech = m_fx->GetTechniqueByName("Light1Fog");
    m_light2FogTech = m_fx->GetTechniqueByName("Light2Fog");
    m_light3FogTech = m_fx->GetTechniqueByName("Light3Fog");

    m_light0TexFogTech = m_fx->GetTechniqueByName("Light0TexFog");
    m_light1TexFogTech = m_fx->GetTechniqueByName("Light1TexFog");
    m_light2TexFogTech = m_fx->GetTechniqueByName("Light2TexFog");
    m_light3TexFogTech = m_fx->GetTechniqueByName("Light3TexFog");

    m_light0TexAlphaClipFogTech = m_fx->GetTechniqueByName("Light0TexAlphaClipFog");
    m_light1TexAlphaClipFogTech = m_fx->GetTechniqueByName("Light1TexAlphaClipFog");
    m_light2TexAlphaClipFogTech = m_fx->GetTechniqueByName("Light2TexAlphaClipFog");
    m_light3TexAlphaClipFogTech = m_fx->GetTechniqueByName("Light3TexAlphaClipFog");

	m_fxWorldViewProj = m_fx->GetVariableByName("gWorldViewProj")->AsMatrix();
	m_fxWorld = m_fx->GetVariableByName("gWorld")->AsMatrix();
	m_fxWorldInvTranspose = m_fx->GetVariableByName("gWorldInvTranspose")->AsMatrix();
    m_fxTexTransform = m_fx->GetVariableByName("gTexTransform")->AsMatrix();
	m_fxEyePosW = m_fx->GetVariableByName("gEyePosW")->AsVector();
	m_fxDirLight = m_fx->GetVariableByName("gDirLights");
	m_fxMaterial = m_fx->GetVariableByName("gMaterial");
    m_fxDiffuseMap = m_fx->GetVariableByName("gDiffuseMap")->AsShaderResource();
    //m_fxBlendMap = m_fx->GetVariableByName("gBlendMap")->AsShaderResource();

    m_fogColor = m_fx->GetVariableByName("gFogColor")->AsVector();
    m_fogStart = m_fx->GetVariableByName("gFogStart")->AsScalar();
    m_fogRange = m_fx->GetVariableByName("gFogRange")->AsScalar();

    NtAsserte(m_light1 != nullptr);
    NtAsserte(m_light2 != nullptr);
    NtAsserte(m_light3 != nullptr);
    NtAsserte(m_light0Tex != nullptr);
    NtAsserte(m_light1Tex != nullptr);
    NtAsserte(m_light2Tex != nullptr);
    NtAsserte(m_light3Tex != nullptr);
    NtAsserte(m_fxWorldViewProj != nullptr);
    NtAsserte(m_fxWorld != nullptr);
    NtAsserte(m_fxWorldInvTranspose != nullptr);
    NtAsserte(m_fxTexTransform != nullptr);
    NtAsserte(m_fxEyePosW != nullptr);
    NtAsserte(m_fxDirLight != nullptr);
    NtAsserte(m_fxMaterial != nullptr);
    NtAsserte(m_fxDiffuseMap != nullptr);
    NtAsserte(m_fogColor != nullptr);
    NtAsserte(m_fogStart != nullptr);
    NtAsserte(m_fogRange != nullptr);
    //NtAsserte(m_fxBlendMap != nullptr);

    NtAsserte(m_light0TexAlphaClipTech != nullptr);
    NtAsserte(m_light1TexAlphaClipTech != nullptr);
    NtAsserte(m_light2TexAlphaClipTech != nullptr);
    NtAsserte(m_light3TexAlphaClipTech != nullptr);

    NtAsserte(m_light1FogTech != nullptr);
    NtAsserte(m_light2FogTech != nullptr);
    NtAsserte(m_light3FogTech != nullptr);

    NtAsserte(m_light0TexFogTech != nullptr);
    NtAsserte(m_light1TexFogTech != nullptr);
    NtAsserte(m_light2TexFogTech != nullptr);
    NtAsserte(m_light3TexFogTech != nullptr);

    NtAsserte(m_light0TexAlphaClipFogTech != nullptr);
    NtAsserte(m_light1TexAlphaClipFogTech != nullptr);
    NtAsserte(m_light2TexAlphaClipFogTech != nullptr);
    NtAsserte(m_light3TexAlphaClipFogTech != nullptr);

	return true;
}

void NtLightShader::Release()
{
	SAFE_RELEASE(m_light1);
	SAFE_RELEASE(m_light2);
	SAFE_RELEASE(m_light3);

    SAFE_RELEASE(m_light0TexAlphaClipTech);
    SAFE_RELEASE(m_light1TexAlphaClipTech);
    SAFE_RELEASE(m_light2TexAlphaClipTech);
    SAFE_RELEASE(m_light3TexAlphaClipTech);

    SAFE_RELEASE(m_light1FogTech);
    SAFE_RELEASE(m_light2FogTech);
    SAFE_RELEASE(m_light3FogTech);

    SAFE_RELEASE(m_light0TexFogTech);
    SAFE_RELEASE(m_light1TexFogTech);
    SAFE_RELEASE(m_light2TexFogTech);
    SAFE_RELEASE(m_light3TexFogTech);

    SAFE_RELEASE(m_light0TexAlphaClipFogTech);
    SAFE_RELEASE(m_light1TexAlphaClipFogTech);
    SAFE_RELEASE(m_light2TexAlphaClipFogTech);
    SAFE_RELEASE(m_light3TexAlphaClipFogTech);

    SAFE_RELEASE(m_fxWorldViewProj);
    SAFE_RELEASE(m_fxWorld);
	SAFE_RELEASE(m_fxWorldInvTranspose);
    SAFE_RELEASE(m_fxTexTransform);
    SAFE_RELEASE(m_fxEyePosW);
    SAFE_RELEASE(m_fxDirLight);
    SAFE_RELEASE(m_fxMaterial);
    SAFE_RELEASE(m_fxDiffuseMap);
    SAFE_RELEASE(m_fogColor);
    SAFE_RELEASE(m_fogStart);
    SAFE_RELEASE(m_fogRange);

	NtShader::Release();
}

}	// namespace renderer
}	// namespace nt
