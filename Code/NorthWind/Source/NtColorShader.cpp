#include "NtCoreLib.h"
#pragma hdrstop

#include "NtColorShader.h"
#include "NtD3DRenderer.h"
#include "NtShaderHandler.h"

namespace nt { namespace renderer {


NtColorShader::NtColorShader()
	: NtShader()
	, m_matrixBuffer(nullptr)
{

}

NtColorShader::~NtColorShader()
{

}

bool NtColorShader::InitializeFx(const ntWchar* fx)
{
	NtAsserte(fx != nullptr);

    if (NtShader::InitializeFx(fx) == false)
    {
        return false;
    }

	ColorTech = m_fx->GetTechniqueByName("ColorTech");
	m_fxWorldViewProj = m_fx->GetVariableByName("gWorldViewProj")->AsMatrix();

    //m_fxTime = m_fx->GetVariableByName("gTime")->AsScalar();

	return true;
}

void NtColorShader::Release()
{
    NtShader::Release();

    SAFE_RELEASE(m_matrixBuffer);
}

bool NtColorShader::RenderFx(ntInt indexCount, const XMMATRIX& worldViewProj)
{
	g_renderer->DeviceContext()->IASetInputLayout(m_layout);
	
	m_fxWorldViewProj->SetMatrix(reinterpret_cast<const float*>(&worldViewProj));

    //m_fxTime->SetFloat(g_app->Timer().TotalTime());

	D3DX11_TECHNIQUE_DESC techDesc;
    NtShaderHandler::ColorShader->ColorTech->GetDesc(&techDesc);
	for (ntUint i = 0; i < techDesc.Passes; i++)
	{
        NtShaderHandler::ColorShader->ColorTech->GetPassByIndex(i)->Apply(0, g_renderer->DeviceContext());

		g_renderer->DeviceContext()->DrawIndexed(indexCount, 0, 0);
	}

	return true;
}

} }
