
#pragma once

#include "dds.h"

namespace NT
{

class NtTextureLoader
{
public:
	static bool CreateD3DResources(NtUInt resDim, NtUInt width, NtUInt height, NtUInt depth, NtUInt mipCount, 
		NtUInt arraySize, DXGI_FORMAT format, D3D11_USAGE usage, NtUInt bindFlags, NtUInt cpuAccessFlags, NtUInt miscFlags, bool forceSRGB,
		bool isCubeMap, D3D11_SUBRESOURCE_DATA* initData, ID3D11Resource** texture, ID3D11ShaderResourceView** textureView);
	static DXGI_FORMAT MakeSRGB(DXGI_FORMAT format);

	// WIC (window imaging component)
	static bool CreateWICTexture(const NtWChar* fileName, NtUInt size, ID3D11Resource** texture, ID3D11ShaderResourceView** textureView);
};


/**
 *	@brief		NtDDSLoader :
 */

class NtDDSLoader : public NtTextureLoader
{
public:
	static NtUInt BitsPerPixel( DXGI_FORMAT fmt );
	static void GetSurfaceInfo( NtUInt width, NtUInt height, DXGI_FORMAT fmt, NtUInt* outNumBytes, NtUInt* outRowBytes, NtUInt* outNumRows );
	static bool FillInitData(const sDDSInitHeader& initHeader, NtUInt& width, NtUInt& height, NtUInt& depth, NtUInt& skipMip, D3D11_SUBRESOURCE_DATA* initData);
	static DXGI_FORMAT GetDXGIFormat( const DirectX::DDS_PIXELFORMAT& ddpf );
	static bool LoadTextureDataFromFile(const NtWChar* fileName, std::unique_ptr<NtUChar[]>& ddsBuffer, DirectX::DDS_HEADER** header, NtUChar** bitData, NtUInt* bitSize);
	static bool CreateTextureFromDDS(const DirectX::DDS_HEADER* header, const NtUChar* bitData, NtUInt bitSize, NtUInt maxSize, D3D11_USAGE usage, NtUInt bindFlags, NtUInt cpuAccessFlags, NtUInt miscFlags, bool forceSRGB, ID3D11Resource** texture, ID3D11ShaderResourceView** textureView);

};


/**
 *	@brief		NtJPGLoader :
 */

class NtJPGLoader : public NtTextureLoader
{
public:
	static bool LoadTextureDataFromFile(const NtWChar* fileName, std::unique_ptr<NtUChar[]>& buffer, NtUChar** bitData, NtUInt& maxSize, NtUInt& width, NtUInt& height, NtSize& textureSize, NtUInt& bpp);
	static bool CreateTextureFromJPG(const NtUChar* buffer, const NtUInt& width, const NtUInt& height, const NtUInt bpp, D3D11_USAGE usage, NtUInt bindFlags, NtUInt cpuAccessFlags, NtUInt miscFlags, ID3D11Resource** texture, ID3D11ShaderResourceView** textureView);
};

}	// namespace NT
