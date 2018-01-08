
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

    m_light0Tex = m_fx->GetTechniqueByName("Light0Tex");
    m_light1Tex = m_fx->GetTechniqueByName("Light1Tex");
    m_light2Tex = m_fx->GetTechniqueByName("Light2Tex");
    m_light3Tex = m_fx->GetTechniqueByName("Light3Tex");
    
	m_fxWorldViewProj = m_fx->GetVariableByName("gWorldViewProj")->AsMatrix();
	m_fxWorld = m_fx->GetVariableByName("gWorld")->AsMatrix();
	m_fxWorldInvTranspose = m_fx->GetVariableByName("gWorldInvTranspose")->AsMatrix();
    m_fxTexTransform = m_fx->GetVariableByName("gTexTransform")->AsMatrix();
	m_fxEyePosW = m_fx->GetVariableByName("gEyePosW")->AsVector();
	m_fxDirLight = m_fx->GetVariableByName("gDirLights");
	m_fxMaterial = m_fx->GetVariableByName("gMaterial");
    m_fxDiffuseMap = m_fx->GetVariableByName("gDiffuseMap")->AsShaderResource();

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

	return true;
}

void NtLightShader::Release()
{
	SAFE_RELEASE(m_light1);
	SAFE_RELEASE(m_light2);
	SAFE_RELEASE(m_light3);
    SAFE_RELEASE(m_fxWorldViewProj);
    SAFE_RELEASE(m_fxWorld);
	SAFE_RELEASE(m_fxWorldInvTranspose);
    SAFE_RELEASE(m_fxEyePosW);
    SAFE_RELEASE(m_fxDirLight);
    SAFE_RELEASE(m_fxMaterial);

	NtShader::Release();
}

}	// namespace renderer
}	// namespace nt
