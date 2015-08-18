
#include "NtCoreLib.h"
#include "NtTexture.h"

#include "NtRenderer.h"
#include "NtTextureHelper.h"

using namespace DirectX;


namespace NT
{
namespace RENDERER
{

NtTexture::NtTexture()
	: m_textureView(nullptr)
	, m_texResource(nullptr)
	, m_width(0)
	, m_height(0)
	, m_useCount(0)
	, m_size(0)
	, m_handle(INVALID_TEXTURE_HANDLE)
{
}


NtTexture::~NtTexture()
{
	Release();
}


bool NtTexture::Initialize(const NtWChar* fileName)
{
	// load the texture
	NtWChar ext[_MAX_EXT];
	_wsplitpath_s(fileName, nullptr, 0, nullptr, 0, nullptr, 0, ext, _MAX_EXT);

	if (Crt::StrCmp(ext, L".dds") == 0)
	{
		return CreateTextureFromDDSFile(fileName);
	}
	else if (Crt::StrCmp(ext, L".jpg") == 0)
	{
		return CreateTextureFromResourceFile(fileName);
	}

	return true;
}


void NtTexture::Release()
{
	--m_useCount;
	if (m_useCount == 0)
	{
		SAFE_RELEASE(m_textureView);
	}
}


ID3D11ShaderResourceView* NtTexture::GetTexture()
{
	return m_textureView;
}


bool NtTexture::CreateTextureFromDDSFile(const NtWChar* fileName)
{
	DDS_HEADER* header = nullptr;
	NtUChar* textureBuffer = nullptr;
	NtUInt bitSize = 0;

	std::unique_ptr<NtUChar[]> ddsBuffer;
	bool res = NtDDSLoader::LoadTextureDataFromFile(fileName, ddsBuffer, &header, &textureBuffer, &bitSize);
	if (false == res)
	{
		return false;
	}

	NtUInt maxSize = 0;

	res = NtDDSLoader::CreateTextureFromDDS(header, textureBuffer, bitSize, maxSize, D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE, 0, 0, false, &m_texResource, &m_textureView);
	if (false == res)
	{
		return false;
	}

	m_width = header->width;
	m_height = header->height;
	m_size = header->size;

	return true;
}

bool NtTexture::CreateTextureFromResourceFile(const NtWChar* fileName)
{
	const NtWChar* fullPath = g_resManager->GetWholePath(fileName);
	if (nullptr == fullPath)
	{
		return false;
	}

	if (false == NtSystem::ReadFileSize(fullPath, m_size))
	{
		return false;
	}

	bool res = NtTextureLoader::CreateWICTexture(fullPath, m_size, &m_texResource, &m_textureView);
	if (false == res)
	{
		return false;
	}


	//NtUChar* jpgBuffer = nullptr;
	//NtUInt jpgBufferSize = 0;
	//NtUInt bpp = 0;
	//std::unique_ptr<NtUChar[]> loadingBuffer;
	//bool res = NtJPGLoader::LoadTextureDataFromFile(fileName, loadingBuffer, &jpgBuffer, jpgBufferSize, m_width, m_height, m_size, bpp);
	//if (false == res)
	//{
	//	return false;
	//}

	//res = NtJPGLoader::CreateTextureFromJPG(renderer, jpgBuffer, m_width, m_height, m_size, bpp, D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE, 0, 0, &m_texResource, &m_textureView);
	//if (false == res)
	//{
	//	return false;
	//}



	return true;
}



bool NtTexHandle::operator ==(NtUInt handle)
{
	return GetHandle() == handle ? true : false;
}


}
}
