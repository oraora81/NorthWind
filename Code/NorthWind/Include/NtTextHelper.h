#pragma once

namespace nt { namespace renderer {

class NtTextHelper
{
public:
    NtTextHelper();

    bool Initialize();

    void Release();

    void Begin();

    void End();

    void RenderTextLine(const ntWchar* str);

    void RenderTextLine(RECT& rc, const ntWchar* str);

    void RenderText11(const ntWchar* strText, RECT rcScreen, XMCOLOR vFontColor, float fBBWidth, float fBBHeight, bool bCenter);

    void EndText11();

    inline void SetInsertionPos(int x, int y)
    {
        m_pt.x = x; m_pt.y = y;
    }

    inline void SetForegroundColor(XMCOLOR color)
    {
        m_color = color;
    }

private:
    ID3D11Buffer* m_fontBuffer;
    ID3D11ShaderResourceView* m_fontSRV11 = NULL;
    XMCOLOR m_color;
    POINT m_pt;
    ntUint m_lineHeight;
    int m_fontBufferBytes11;

    std::vector<Vertex::SpriteVertex> m_vertexArray;
};

}}
