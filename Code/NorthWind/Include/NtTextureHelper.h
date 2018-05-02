
#pragma once

#include "dds.h"

namespace nt {

class NtTextureLoader
{
public:
	static bool CreateD3DResources(ntUint resDim, ntUint width, ntUint height, ntUint depth, ntUint mipCount, 
		ntUint arraySize, DXGI_FORMAT format, D3D11_USAGE usage, ntUint bindFlags, ntUint cpuAccessFlags, ntUint miscFlags, bool forceSRGB,
		bool isCubeMap, D3D11_SUBRESOURCE_DATA* initData, ID3D11Resource** texture, ID3D11ShaderResourceView** textureView);
	static DXGI_FORMAT MakeSRGB(DXGI_FORMAT format);

	// WIC (window imaging component)
	static bool CreateWICTexture(const ntWchar* fileName, ntSize size, ID3D11Resource** texture, ID3D11ShaderResourceView** textureView);
};


/**
 *	@brief		NtDDSLoader :
 */

class NtDDSLoader : public NtTextureLoader
{
public:
	static ntUint BitsPerPixel( DXGI_FORMAT fmt );
	static void GetSurfaceInfo( ntUint width, ntUint height, DXGI_FORMAT fmt, ntUint* outNumBytes, ntUint* outRowBytes, ntUint* outNumRows );
	static bool FillInitData(const DDSInitHeader& initHeader, ntUint& width, ntUint& height, ntUint& depth, ntUint& skipMip, D3D11_SUBRESOURCE_DATA* initData);
	static DXGI_FORMAT GetDXGIFormat( const DirectX::DDS_PIXELFORMAT& ddpf );
	static bool LoadTextureDataFromFile(const ntWchar* fileName, std::unique_ptr<ntUchar[]>& ddsBuffer, DirectX::DDS_HEADER** header, ntUchar** bitData, ntUint* bitSize);
	static bool CreateTextureFromDDS(const DirectX::DDS_HEADER* header, const ntUchar* bitData, ntUint bitSize, ntUint maxSize, D3D11_USAGE usage, ntUint bindFlags, ntUint cpuAccessFlags, ntUint miscFlags, bool forceSRGB, ID3D11Resource** texture, ID3D11ShaderResourceView** textureView);

};


/**
 *	@brief		NtJPGLoader :
 */

class NtJPGLoader : public NtTextureLoader
{
public:
	static bool LoadTextureDataFromFile(const ntWchar* fileName, std::unique_ptr<ntUchar[]>& buffer, ntUchar** bitData, ntUint& maxSize, ntUint& width, ntUint& height, ntSize& textureSize, ntUint& bpp);
	static bool CreateTextureFromJPG(const ntUchar* buffer, const ntUint& width, const ntUint& height, const ntUint bpp, D3D11_USAGE usage, ntUint bindFlags, ntUint cpuAccessFlags, ntUint miscFlags, ID3D11Resource** texture, ID3D11ShaderResourceView** textureView);
};

}	// namespace nt
