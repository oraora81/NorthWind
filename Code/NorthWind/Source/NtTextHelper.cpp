#include "NtCoreLib.h"
#pragma hdrstop

#include "NtTextHelper.h"
#include "NtInputLayout.h"
#include "NtUIShader.h"
#include "NtShaderHandler.h"

namespace nt { namespace renderer {

NtTextHelper::NtTextHelper()
    : m_fontBuffer(nullptr)
    , m_screenQuadBuffer(nullptr)
    , m_fontSRV11(nullptr)
    , m_storeDepthStencilState(nullptr)
    , m_storeRasterizeState(nullptr)
    , m_storeBlendState(nullptr)
    , m_storeSamplerState(nullptr)
    , m_stencilRefStored(0)
    , m_sampleMaskStored(0)
    , m_samplerStateStored(0)
    , m_lineHeight(0)
    , m_fontBufferBytes11(0)
{
    m_color = Colors::_White;
    m_pt.x = 0;
    m_pt.y = 0;
    Crt::MemSet(m_blendFactorStored, sizeof(m_blendFactorStored));
}

bool NtTextHelper::Initialize()
{
    D3D11_BUFFER_DESC bufDesc;
    Crt::MemSet(&bufDesc, sizeof(D3D11_BUFFER_DESC));

    bufDesc.ByteWidth = sizeof(Vertex::SpriteVertex) * 4;
    bufDesc.Usage = D3D11_USAGE_DYNAMIC;
    bufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    HRF(g_renderer->Device()->CreateBuffer(&bufDesc, nullptr, &m_screenQuadBuffer));

    HRF(g_renderer->CreateShaderResourceView(L"Font.dds", &m_fontSRV11));

    return true;
}

void NtTextHelper::Release()
{
    SAFE_RELEASE(m_fontBuffer);
    SAFE_RELEASE(m_screenQuadBuffer);
    SAFE_RELEASE(m_fontSRV11);
}

void NtTextHelper::Begin()
{
    
}

void NtTextHelper::End()
{

}

void NtTextHelper::RenderTextLine(const ntWchar* str)
{
    RECT rc;
    SetRect(&rc, m_pt.x, m_pt.y, 0, 0);
    
    RenderText11(str, rc, m_color, 
        (float)g_app->Width(), (float)g_app->Height(), false);

    m_pt.y += m_lineHeight;
}

void NtTextHelper::RenderTextLine(RECT& rc, const ntWchar* str)
{

}

void NtTextHelper::RenderText11(const ntWchar* strText, RECT rcScreen, XMCOLOR vFontColor,
    float fBBWidth, float fBBHeight, bool bCenter)
{
    float fCharTexSizeX = 0.010526315f;
    //float fGlyphSizeX = 14.0f / fBBWidth;
    //float fGlyphSizeY = 32.0f / fBBHeight;
    float fGlyphSizeX = 15.0f / fBBWidth;
    float fGlyphSizeY = 42.0f / fBBHeight;


    float fRectLeft = rcScreen.left / fBBWidth;
    float fRectTop = 1.0f - rcScreen.top / fBBHeight;

    fRectLeft = fRectLeft * 2.0f - 1.0f;
    fRectTop = fRectTop * 2.0f - 1.0f;

    int NumChars = (int)wcslen(strText);
    if (bCenter) {
        float fRectRight = rcScreen.right / fBBWidth;
        fRectRight = fRectRight * 2.0f - 1.0f;
        float fRectBottom = 1.0f - rcScreen.bottom / fBBHeight;
        fRectBottom = fRectBottom * 2.0f - 1.0f;
        float fcenterx = ((fRectRight - fRectLeft) - (float)NumChars*fGlyphSizeX) *0.5f;
        float fcentery = ((fRectTop - fRectBottom) - (float)1 * fGlyphSizeY) *0.5f;
        fRectLeft += fcenterx;
        fRectTop -= fcentery;
    }
    float fOriginalLeft = fRectLeft;
    float fTexTop = 0.0f;
    float fTexBottom = 1.0f;

    float fDepth = 0.5f;
    for (int i = 0; i < NumChars; i++)
    {
        if (strText[i] == '\n')
        {
            fRectLeft = fOriginalLeft;
            fRectTop -= fGlyphSizeY;

            continue;
        }
        else if (strText[i] < 32 || strText[i] > 126)
        {
            continue;
        }

        // Add 6 sprite vertices
        float fRectRight = fRectLeft + fGlyphSizeX;
        float fRectBottom = fRectTop - fGlyphSizeY;
        float fTexLeft = (strText[i] - 32) * fCharTexSizeX;
        float fTexRight = fTexLeft + fCharTexSizeX;

        // tri1
        m_vertexArray.push_back(Vertex::SpriteVertex(fRectLeft, fRectTop, fDepth, 
            vFontColor, fTexLeft, fTexTop));

        m_vertexArray.push_back(Vertex::SpriteVertex(fRectRight, fRectTop, fDepth, 
            vFontColor, fTexRight, fTexTop));

        m_vertexArray.push_back(Vertex::SpriteVertex(fRectLeft, fRectBottom, fDepth,
            vFontColor, fTexLeft, fTexBottom));

        // tri2
        m_vertexArray.push_back(Vertex::SpriteVertex(fRectRight, fRectTop, fDepth,
            vFontColor, fTexRight, fTexTop));

        m_vertexArray.push_back(Vertex::SpriteVertex(fRectRight, fRectBottom, fDepth,
            vFontColor, fTexRight, fTexBottom));

        m_vertexArray.push_back(Vertex::SpriteVertex(fRectLeft, fRectBottom, fDepth,
            vFontColor, fTexLeft, fTexBottom));

        fRectLeft += fGlyphSizeX;
    }

    // We have to end text after every line so that rendering order between sprites and fonts is preserved
    EndText11();
}

void NtTextHelper::EndText11()
{
    // ensure our buffer size can hold our sprites
    ntUint FontDataBytes = static_cast<ntUint>(m_vertexArray.size()) * sizeof(Vertex::SpriteVertex);

    if (m_fontBufferBytes11 < FontDataBytes)
    {
        SAFE_RELEASE(m_fontBuffer);
        m_fontBufferBytes11 = FontDataBytes;

        D3D11_BUFFER_DESC BufferDesc;
        BufferDesc.ByteWidth = m_fontBufferBytes11;
        BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
        BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        BufferDesc.MiscFlags = 0;

        NtRenderBufferParam param;
        param.m_desc = &BufferDesc;
        param.m_resData = nullptr;
        param.m_buffer = &m_fontBuffer;

        g_renderInterface->CreateBuffer(param);
    }

    // Copy the sprites over
    D3D11_BOX destRegion;
    destRegion.left = 0;
    destRegion.right = FontDataBytes;
    destRegion.top = 0;
    destRegion.bottom = 1;
    destRegion.front = 0;
    destRegion.back = 1;
    D3D11_MAPPED_SUBRESOURCE MappedResource;
    
    auto context = g_renderer->DeviceContext();

    HR(context->Map(m_fontBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource));

    Crt::MemCpy(MappedResource.pData, (void*)&m_vertexArray[0], FontDataBytes);
    context->Unmap(m_fontBuffer, 0);

    ID3D11ShaderResourceView* oldSRV = NULL;
    context->PSGetShaderResources(0, 1, &oldSRV);
    context->PSSetShaderResources(0, 1, &m_fontSRV11);

    // Draw
    UINT Stride = sizeof(Vertex::SpriteVertex);
    UINT Offset = 0;
    context->IASetVertexBuffers(0, 1, &m_fontBuffer, &Stride, &Offset);
    context->IASetInputLayout(NtInputLayoutHandler::SpriteInputLayout);
    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    context->Draw((ntUint)m_vertexArray.size(), 0);

    context->PSSetShaderResources(0, 1, &oldSRV);
    SAFE_RELEASE(oldSRV);

    m_vertexArray.clear();
}

void NtTextHelper::StoreRenderState()
{
    ID3D11DeviceContext* context = g_renderer->DeviceContext();

    context->OMGetDepthStencilState(&m_storeDepthStencilState, &m_stencilRefStored);
    context->RSGetState(&m_storeRasterizeState);
    context->OMGetBlendState(&m_storeBlendState, m_blendFactorStored, &m_sampleMaskStored);
    context->PSGetSamplers(0, 1, &m_storeSamplerState);
}

void NtTextHelper::RestoreRenderState()
{
    ID3D11DeviceContext* context = g_renderer->DeviceContext();

    context->OMSetDepthStencilState(m_storeDepthStencilState, m_stencilRefStored);
    //context->RSSetScissorRects(m_storeRasterizeState);
    context->OMSetBlendState(m_storeBlendState, m_blendFactorStored, m_sampleMaskStored);
    context->PSSetSamplers(0, 1, &m_storeSamplerState);

    SAFE_RELEASE(m_storeDepthStencilState);
    SAFE_RELEASE(m_storeRasterizeState);
    SAFE_RELEASE(m_storeBlendState);
    SAFE_RELEASE(m_storeSamplerState);
}

}}
