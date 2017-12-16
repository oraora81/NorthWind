
#include "NtCoreLib.h"
#pragma hdrstop

#include "NtImage.h"

namespace nt { namespace renderer {

NtImage::NtImage()
: m_texHandle(INVALID_TEXTURE_HANDLE)
, m_vertexBuffer(nullptr)
, m_indexBuffer(nullptr)
, m_vertexCount(0)
, m_indexCount(0)

{
	Crt::MemSet(&m_screenSize, sizeof(m_screenSize));
	Crt::MemSet(&m_prevPos, sizeof(m_prevPos));
}

NtImage::~NtImage()
{

}

bool NtImage::Initialize(const ntWchar* fileName, ntInt screenWidth, ntInt screenHeight)
{
	m_texHandle = g_resMgr->LoadTexture(fileName);
	if (m_texHandle == INVALID_TEXTURE_HANDLE)
	{
		return false;
	}
	
	if (InitializeBuffer() == false)
	{
		return false;
	}

	//if (m_texture->Initialize(renderer, fileName) == false)
	//{
	//	return false;
	//}

	// post action
	m_screenSize.x = screenWidth;
	m_screenSize.y = screenHeight;

	m_prevPos.x = -1;
	m_prevPos.y = -1;

	return true;
}

void NtImage::Release()
{
	g_resMgr->ReleaseTexture(m_texHandle);
}


bool NtImage::InitializeBuffer()
{
	return true;
}

} }

