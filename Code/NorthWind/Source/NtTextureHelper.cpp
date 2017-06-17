
#include "NtCoreLib.h"
#pragma hdrstop

#include "NtTextureHelper.h"
#include "NtD3DRenderer.h"

#pragma warning(push)
#pragma warning(disable: 4005)
#include <wincodec.h>
#pragma warning(pop)

#define __HRESULT_FROM_WIN32(x) ((HRESULT)(x) <= 0 ? ((HRESULT)(x)) : ((HRESULT) (((x) & 0x0000FFFF) | (FACILITY_WIN32 << 16) | 0x80000000)))

using namespace DirectX;

namespace nt {

	struct scopeGuard
	{
		scopeGuard(HANDLE h) : m_handle(h) {}
		~scopeGuard() { if (m_handle) CloseHandle(m_handle); }

		HANDLE m_handle;
	};

	//-------------------------------------------------------------------------------------
	// WIC Pixel Format Translation Data
	//-------------------------------------------------------------------------------------
	struct WICTranslate
	{
		GUID                wic;
		DXGI_FORMAT         format;
	};

	static WICTranslate g_WICFormats[] = 
	{
		{ GUID_WICPixelFormat128bppRGBAFloat,       DXGI_FORMAT_R32G32B32A32_FLOAT },

		{ GUID_WICPixelFormat64bppRGBAHalf,         DXGI_FORMAT_R16G16B16A16_FLOAT },
		{ GUID_WICPixelFormat64bppRGBA,             DXGI_FORMAT_R16G16B16A16_UNORM },

		{ GUID_WICPixelFormat32bppRGBA,             DXGI_FORMAT_R8G8B8A8_UNORM },
		{ GUID_WICPixelFormat32bppBGRA,             DXGI_FORMAT_B8G8R8A8_UNORM }, // DXGI 1.1
		{ GUID_WICPixelFormat32bppBGR,              DXGI_FORMAT_B8G8R8X8_UNORM }, // DXGI 1.1

		{ GUID_WICPixelFormat32bppRGBA1010102XR,    DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM }, // DXGI 1.1
		{ GUID_WICPixelFormat32bppRGBA1010102,      DXGI_FORMAT_R10G10B10A2_UNORM },
		{ GUID_WICPixelFormat32bppRGBE,             DXGI_FORMAT_R9G9B9E5_SHAREDEXP },

#ifdef DXGI_1_2_FORMATS

		{ GUID_WICPixelFormat16bppBGRA5551,         DXGI_FORMAT_B5G5R5A1_UNORM },
		{ GUID_WICPixelFormat16bppBGR565,           DXGI_FORMAT_B5G6R5_UNORM },

#endif // DXGI_1_2_FORMATS

		{ GUID_WICPixelFormat32bppGrayFloat,        DXGI_FORMAT_R32_FLOAT },
		{ GUID_WICPixelFormat16bppGrayHalf,         DXGI_FORMAT_R16_FLOAT },
		{ GUID_WICPixelFormat16bppGray,             DXGI_FORMAT_R16_UNORM },
		{ GUID_WICPixelFormat8bppGray,              DXGI_FORMAT_R8_UNORM },

		{ GUID_WICPixelFormat8bppAlpha,             DXGI_FORMAT_A8_UNORM },
	};


	//-------------------------------------------------------------------------------------
	// WIC Pixel Format nearest conversion table
	//-------------------------------------------------------------------------------------

	struct WICConvert
	{
		GUID        source;
		GUID        target;
	};

	static WICConvert g_WICConvert[] = 
	{
		// Note target GUID in this conversion table must be one of those directly supported formats (above).

		{ GUID_WICPixelFormatBlackWhite,            GUID_WICPixelFormat8bppGray }, // DXGI_FORMAT_R8_UNORM

		{ GUID_WICPixelFormat1bppIndexed,           GUID_WICPixelFormat32bppRGBA }, // DXGI_FORMAT_R8G8B8A8_UNORM 
		{ GUID_WICPixelFormat2bppIndexed,           GUID_WICPixelFormat32bppRGBA }, // DXGI_FORMAT_R8G8B8A8_UNORM 
		{ GUID_WICPixelFormat4bppIndexed,           GUID_WICPixelFormat32bppRGBA }, // DXGI_FORMAT_R8G8B8A8_UNORM 
		{ GUID_WICPixelFormat8bppIndexed,           GUID_WICPixelFormat32bppRGBA }, // DXGI_FORMAT_R8G8B8A8_UNORM 

		{ GUID_WICPixelFormat2bppGray,              GUID_WICPixelFormat8bppGray }, // DXGI_FORMAT_R8_UNORM 
		{ GUID_WICPixelFormat4bppGray,              GUID_WICPixelFormat8bppGray }, // DXGI_FORMAT_R8_UNORM 

		{ GUID_WICPixelFormat16bppGrayFixedPoint,   GUID_WICPixelFormat16bppGrayHalf }, // DXGI_FORMAT_R16_FLOAT 
		{ GUID_WICPixelFormat32bppGrayFixedPoint,   GUID_WICPixelFormat32bppGrayFloat }, // DXGI_FORMAT_R32_FLOAT 

#ifdef DXGI_1_2_FORMATS

		{ GUID_WICPixelFormat16bppBGR555,           GUID_WICPixelFormat16bppBGRA5551 }, // DXGI_FORMAT_B5G5R5A1_UNORM

#else

		{ GUID_WICPixelFormat16bppBGR555,           GUID_WICPixelFormat32bppRGBA }, // DXGI_FORMAT_R8G8B8A8_UNORM
		{ GUID_WICPixelFormat16bppBGRA5551,         GUID_WICPixelFormat32bppRGBA }, // DXGI_FORMAT_R8G8B8A8_UNORM
		{ GUID_WICPixelFormat16bppBGR565,           GUID_WICPixelFormat32bppRGBA }, // DXGI_FORMAT_R8G8B8A8_UNORM

#endif // DXGI_1_2_FORMATS

		{ GUID_WICPixelFormat32bppBGR101010,        GUID_WICPixelFormat32bppRGBA1010102 }, // DXGI_FORMAT_R10G10B10A2_UNORM

		{ GUID_WICPixelFormat24bppBGR,              GUID_WICPixelFormat32bppRGBA }, // DXGI_FORMAT_R8G8B8A8_UNORM 
		{ GUID_WICPixelFormat24bppRGB,              GUID_WICPixelFormat32bppRGBA }, // DXGI_FORMAT_R8G8B8A8_UNORM 
		{ GUID_WICPixelFormat32bppPBGRA,            GUID_WICPixelFormat32bppRGBA }, // DXGI_FORMAT_R8G8B8A8_UNORM 
		{ GUID_WICPixelFormat32bppPRGBA,            GUID_WICPixelFormat32bppRGBA }, // DXGI_FORMAT_R8G8B8A8_UNORM 

		{ GUID_WICPixelFormat48bppRGB,              GUID_WICPixelFormat64bppRGBA }, // DXGI_FORMAT_R16G16B16A16_UNORM
		{ GUID_WICPixelFormat48bppBGR,              GUID_WICPixelFormat64bppRGBA }, // DXGI_FORMAT_R16G16B16A16_UNORM
		{ GUID_WICPixelFormat64bppBGRA,             GUID_WICPixelFormat64bppRGBA }, // DXGI_FORMAT_R16G16B16A16_UNORM
		{ GUID_WICPixelFormat64bppPRGBA,            GUID_WICPixelFormat64bppRGBA }, // DXGI_FORMAT_R16G16B16A16_UNORM
		{ GUID_WICPixelFormat64bppPBGRA,            GUID_WICPixelFormat64bppRGBA }, // DXGI_FORMAT_R16G16B16A16_UNORM

		{ GUID_WICPixelFormat48bppRGBFixedPoint,    GUID_WICPixelFormat64bppRGBAHalf }, // DXGI_FORMAT_R16G16B16A16_FLOAT 
		{ GUID_WICPixelFormat48bppBGRFixedPoint,    GUID_WICPixelFormat64bppRGBAHalf }, // DXGI_FORMAT_R16G16B16A16_FLOAT 
		{ GUID_WICPixelFormat64bppRGBAFixedPoint,   GUID_WICPixelFormat64bppRGBAHalf }, // DXGI_FORMAT_R16G16B16A16_FLOAT 
		{ GUID_WICPixelFormat64bppBGRAFixedPoint,   GUID_WICPixelFormat64bppRGBAHalf }, // DXGI_FORMAT_R16G16B16A16_FLOAT 
		{ GUID_WICPixelFormat64bppRGBFixedPoint,    GUID_WICPixelFormat64bppRGBAHalf }, // DXGI_FORMAT_R16G16B16A16_FLOAT 
		{ GUID_WICPixelFormat64bppRGBHalf,          GUID_WICPixelFormat64bppRGBAHalf }, // DXGI_FORMAT_R16G16B16A16_FLOAT 
		{ GUID_WICPixelFormat48bppRGBHalf,          GUID_WICPixelFormat64bppRGBAHalf }, // DXGI_FORMAT_R16G16B16A16_FLOAT 

		{ GUID_WICPixelFormat128bppPRGBAFloat,      GUID_WICPixelFormat128bppRGBAFloat }, // DXGI_FORMAT_R32G32B32A32_FLOAT 
		{ GUID_WICPixelFormat128bppRGBFloat,        GUID_WICPixelFormat128bppRGBAFloat }, // DXGI_FORMAT_R32G32B32A32_FLOAT 
		{ GUID_WICPixelFormat128bppRGBAFixedPoint,  GUID_WICPixelFormat128bppRGBAFloat }, // DXGI_FORMAT_R32G32B32A32_FLOAT 
		{ GUID_WICPixelFormat128bppRGBFixedPoint,   GUID_WICPixelFormat128bppRGBAFloat }, // DXGI_FORMAT_R32G32B32A32_FLOAT 

		{ GUID_WICPixelFormat32bppCMYK,             GUID_WICPixelFormat32bppRGBA }, // DXGI_FORMAT_R8G8B8A8_UNORM 
		{ GUID_WICPixelFormat64bppCMYK,             GUID_WICPixelFormat64bppRGBA }, // DXGI_FORMAT_R16G16B16A16_UNORM
		{ GUID_WICPixelFormat40bppCMYKAlpha,        GUID_WICPixelFormat64bppRGBA }, // DXGI_FORMAT_R16G16B16A16_UNORM
		{ GUID_WICPixelFormat80bppCMYKAlpha,        GUID_WICPixelFormat64bppRGBA }, // DXGI_FORMAT_R16G16B16A16_UNORM

#if (_WIN32_WINNT >= 0x0602 /*_WIN32_WINNT_WIN8*/) || defined(_WIN7_PLATFORM_UPDATE)
		{ GUID_WICPixelFormat32bppRGB,              GUID_WICPixelFormat32bppRGBA }, // DXGI_FORMAT_R8G8B8A8_UNORM
		{ GUID_WICPixelFormat64bppRGB,              GUID_WICPixelFormat64bppRGBA }, // DXGI_FORMAT_R16G16B16A16_UNORM
		{ GUID_WICPixelFormat64bppPRGBAHalf,        GUID_WICPixelFormat64bppRGBAHalf }, // DXGI_FORMAT_R16G16B16A16_FLOAT 
#endif

		// We don't support n-channel formats
	};

	static bool g_WIC2 = false;


/**
 *	@brief		NtTextureLoader :
 */

bool NtTextureLoader::CreateD3DResources(ntUint resDim, ntUint width, ntUint height, ntUint depth, ntUint mipCount, ntUint arraySize, DXGI_FORMAT format, D3D11_USAGE usage,
	ntUint bindFlags, ntUint cpuAccessFlags, ntUint miscFlags, bool forceSRGB, bool isCubeMap, D3D11_SUBRESOURCE_DATA* initData, ID3D11Resource** texture, ID3D11ShaderResourceView** textureView)
{
	if (forceSRGB)
	{
		format = MakeSRGB(format);
	}

	switch(resDim)
	{
	case D3D11_RESOURCE_DIMENSION_TEXTURE1D:
		{
			D3D11_TEXTURE1D_DESC desc;
			desc.Width = static_cast<ntUint>(width);
			desc.MipLevels = static_cast<ntUint>(mipCount);
			desc.ArraySize = static_cast<ntUint>(arraySize);
			desc.Format = format;
			desc.Usage = usage;
			desc.BindFlags = bindFlags;
			desc.CPUAccessFlags = cpuAccessFlags;
			desc.MiscFlags = miscFlags & ~D3D11_RESOURCE_MISC_TEXTURECUBE;

			ID3D11Texture1D* tex = nullptr;
			if (!g_renderInterface->CreateTexture1D(&desc, initData, &tex))
			{
				return false;
			}

			if (tex == nullptr)
			{
				return false;
			}

			D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
			Crt::MemSet(&SRVDesc, sizeof(SRVDesc));

			SRVDesc.Format = format;

			if (arraySize > 1)
			{
				SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1DARRAY;
				SRVDesc.Texture1DArray.MipLevels = desc.MipLevels;
				SRVDesc.Texture1DArray.ArraySize = static_cast<ntUint>(arraySize);
			}
			else
			{
				SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
				SRVDesc.Texture1D.MipLevels = desc.MipLevels;
			}

			if (!g_renderer->CreateShaderResourceView(tex, &SRVDesc, textureView))
			{
				SAFE_RELEASE(tex);
				return false;
			}

			if (texture != NULL)
			{
				*texture = tex;
			}
			else
			{
				SAFE_RELEASE(tex);
			}
		}
		break;

	case D3D11_RESOURCE_DIMENSION_TEXTURE2D:
		{
			D3D11_TEXTURE2D_DESC desc;
			desc.Width = static_cast<ntUint>( width );
			desc.Height = static_cast<ntUint>( height );
			desc.MipLevels = static_cast<ntUint>( mipCount );
			desc.ArraySize = static_cast<ntUint>( arraySize );
			desc.Format = format;
			desc.SampleDesc.Count = 1;
			desc.SampleDesc.Quality = 0;
			desc.Usage = usage;
			desc.BindFlags = bindFlags;
			desc.CPUAccessFlags = cpuAccessFlags;
			if (isCubeMap)
			{
				desc.MiscFlags = miscFlags | D3D11_RESOURCE_MISC_TEXTURECUBE;
			}
			else
			{
				desc.MiscFlags = miscFlags & ~D3D11_RESOURCE_MISC_TEXTURECUBE;
			}

			ID3D11Texture2D* tex = nullptr;
			if (!g_renderInterface->CreateTexture2D(&desc, initData, &tex))
			{
				return false;
			}

			if (tex == nullptr)
			{
				return false;
			}

			D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
			Crt::MemSet(&SRVDesc, sizeof(SRVDesc));
			SRVDesc.Format = format;

			if (isCubeMap)
			{
				if (arraySize > 0)
				{
					SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBEARRAY;
					SRVDesc.TextureCubeArray.MipLevels = desc.MipLevels;

					SRVDesc.TextureCubeArray.NumCubes = static_cast<ntUint>(arraySize / 6);
				}
				else
				{
					SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
					SRVDesc.TextureCube.MipLevels = desc.MipLevels;
				}
			}
			else
			{
				if (arraySize > 1)
				{
					SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
					SRVDesc.Texture2DArray.MipLevels = desc.MipLevels;
					SRVDesc.Texture2DArray.ArraySize = static_cast<ntUint>(arraySize);
				}
				else
				{
					SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
					SRVDesc.Texture2D.MipLevels = desc.MipLevels;
				}
			}

			if (!g_renderer->CreateShaderResourceView(tex, &SRVDesc, textureView))
			{
				SAFE_RELEASE(tex);
				return false;
			}

			if (texture != NULL)
			{
				*texture = tex;
			}
			else
			{
				SAFE_RELEASE(tex);
			}
		}
		break;

	case D3D11_RESOURCE_DIMENSION_TEXTURE3D:
		{
			D3D11_TEXTURE3D_DESC desc;
			desc.Width = static_cast<ntUint>( width );
			desc.Height = static_cast<ntUint>( height );
			desc.Depth = static_cast<ntUint>( depth );
			desc.MipLevels = static_cast<ntUint>( mipCount );
			desc.Format = format;
			desc.Usage = usage;
			desc.BindFlags = bindFlags;
			desc.CPUAccessFlags = cpuAccessFlags;
			desc.MiscFlags = miscFlags & ~D3D11_RESOURCE_MISC_TEXTURECUBE;

			ID3D11Texture3D* tex = nullptr;
			if (!g_renderInterface->CreateTexture3D(&desc, initData, &tex))
			{
				return false;
			}

			if (tex == nullptr)
			{
				return false;
			}

			D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
			Crt::MemSet(&SRVDesc, sizeof(SRVDesc));
			SRVDesc.Format = format;

			SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE3D;
			SRVDesc.Texture3D.MipLevels = desc.MipLevels;

			if (!g_renderer->CreateShaderResourceView(tex, &SRVDesc, textureView))
			{
				SAFE_RELEASE(tex);
				return false;
			}

			if (texture != NULL)
			{
				*texture = tex;
			}
			else
			{
				SAFE_RELEASE(tex);
			}
		}
		break;
	}

	return true;
}

DXGI_FORMAT NtTextureLoader::MakeSRGB(DXGI_FORMAT format)
{
	switch( format )
	{
	case DXGI_FORMAT_R8G8B8A8_UNORM:
		return DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

	case DXGI_FORMAT_BC1_UNORM:
		return DXGI_FORMAT_BC1_UNORM_SRGB;

	case DXGI_FORMAT_BC2_UNORM:
		return DXGI_FORMAT_BC2_UNORM_SRGB;

	case DXGI_FORMAT_BC3_UNORM:
		return DXGI_FORMAT_BC3_UNORM_SRGB;

	case DXGI_FORMAT_B8G8R8A8_UNORM:
		return DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;

	case DXGI_FORMAT_B8G8R8X8_UNORM:
		return DXGI_FORMAT_B8G8R8X8_UNORM_SRGB;

	case DXGI_FORMAT_BC7_UNORM:
		return DXGI_FORMAT_BC7_UNORM_SRGB;

	default:
		return format;
	}
}



IWICImagingFactory* GetWICFactory()
{
	static IWICImagingFactory* factory = nullptr;

	if (factory)
	{
		return factory;
	}

#if (_WIN32_WINNT >= 0x0602 /*_WIN32_WINNT_WIN8*/) || defined(_WIN7_PLATFORM_UPDATE)
	HRESULT res = CoCreateInstance(CLSID_WICImagingFactory2,
		nullptr,
		CLSCTX_INPROC_SERVER,
		__uuidof(IWICImagingFactory2),
		(LPVOID*)&factory);

	if (FAILED(res))
	{
		res = CoCreateInstance(CLSID_WICImagingFactory1,
			nullptr,
			CLSCTX_INPROC_SERVER,
			__uuidof(IWICImagingFactory),
			(LPVOID*)&factory);

		if (FAILED(res))
		{
			factory = nullptr;
			return nullptr;
		}
	}
	else
	{
		g_WIC2 = true;
	}
#else
	HRESULT res = CoCreateInstance(CLSID_WICImagingFactory,
		nullptr,
		CLSCTX_INPROC_SERVER,
		__uuidof(IWICImagingFactory),
		(LPVOID*)&factory);

	if (FAILED(res))
	{
		factory = nullptr;
		return nullptr;
	}
#endif

	return factory;
}

static DXGI_FORMAT _WICToDXGI( const GUID& guid )
{
	for( size_t i=0; i < _countof(g_WICFormats); ++i )
	{
		if ( memcmp( &g_WICFormats[i].wic, &guid, sizeof(GUID) ) == 0 )
			return g_WICFormats[i].format;
	}

#if (_WIN32_WINNT >= 0x0602 /*_WIN32_WINNT_WIN8*/) || defined(_WIN7_PLATFORM_UPDATE)
	if ( g_WIC2 )
	{
		if ( memcmp( &GUID_WICPixelFormat96bppRGBFloat, &guid, sizeof(GUID) ) == 0 )
			return DXGI_FORMAT_R32G32B32_FLOAT;
	}
#endif

	return DXGI_FORMAT_UNKNOWN;
}

static size_t _WICBitsPerPixel( REFGUID targetGuid )
{
	IWICImagingFactory* pWIC = GetWICFactory();
	if ( !pWIC )
		return 0;

	NtScopedObject<IWICComponentInfo> cinfo;
	if ( FAILED( pWIC->CreateComponentInfo( targetGuid, &cinfo ) ) )
		return 0;

	WICComponentType type;
	if ( FAILED( cinfo->GetComponentType( &type ) ) )
		return 0;

	if ( type != WICPixelFormat )
		return 0;

	NtScopedObject<IWICPixelFormatInfo> pfinfo;
	if ( FAILED( cinfo->QueryInterface( __uuidof(IWICPixelFormatInfo), reinterpret_cast<void**>( &pfinfo )  ) ) )
		return 0;

	UINT bpp;
	if ( FAILED( pfinfo->GetBitsPerPixel( &bpp ) ) )
		return 0;

	return bpp;
}

static HRESULT CreateTextureFromWIC(IWICBitmapFrameDecode* frame, ntSize maxSize, D3D11_USAGE usage, ntUint bindFlags, ntUint cpuAccess, ntUint misc, ID3D11Resource** texture,ID3D11ShaderResourceView** textureView)
{
	ntUint width = 0;
	ntUint height = 0;

	HRESULT res = frame->GetSize(&width, &height);
	if (FAILED(res))
	{
		return E_FAIL;
	}

	NtAsserte(width > 0 && height > 0);

	if (!maxSize)
	{
		// This is a bit conservative because the hardware could support larger textures than
		// the Feature Level defined minimums, but doing it this way is much easier and more
		// performant for WIC than the 'fail and retry' model used by DDSTextureLoader
		switch(g_renderInterface->GetFeatureLevel())
		{
		case D3D_FEATURE_LEVEL_9_1:
		case D3D_FEATURE_LEVEL_9_2:
			maxSize = 2048 /*D3D_FL9_1_REQ_TEXTURE2D_U_OR_V_DIMENSION*/;
			break;

		case D3D_FEATURE_LEVEL_9_3:
			maxSize = 4096 /*D3D_FL9_3_REQ_TEXTURE2D_U_OR_V_DIMENSION*/;
			break;

		case D3D_FEATURE_LEVEL_10_0:
		case D3D_FEATURE_LEVEL_10_1:
			maxSize = 8192 /*D3D10_REQ_TEXTURE2D_U_OR_V_DIMENSION*/;
			break;

		default:
			maxSize = D3D11_REQ_TEXTURE2D_U_OR_V_DIMENSION;
			break;
		}
	}

	NtAsserte(maxSize > 0);

	ntUint tempWidth = 0;
	ntUint tempHeight = 0;
	if (width > maxSize || height > maxSize)
	{
		ntFloat ar = static_cast<ntFloat>(height) / static_cast<ntFloat>(width);
		if (width > height)
		{
			tempWidth = static_cast<ntUint>(maxSize);
			tempHeight = static_cast<ntUint>(static_cast<ntFloat>(maxSize) * ar);
		}
		else
		{
			tempHeight = static_cast<ntUint>(maxSize);
			tempWidth = static_cast<ntUint>(static_cast<ntFloat>(maxSize) * ar);
		}

		NtAsserte((ntSize)tempWidth <= maxSize && (ntSize)tempHeight <= maxSize);
	}
	else
	{
		tempWidth = width;
		tempHeight = height;
	}

	// Determine format
	WICPixelFormatGUID pixelFormat;
	res = frame->GetPixelFormat(&pixelFormat);
	if (FAILED(res))
	{
		return E_FAIL;
	}

	WICPixelFormatGUID convertGUID;
	Crt::MemCpy(&convertGUID, &pixelFormat, sizeof(WICPixelFormatGUID));

	ntSize bpp = 0;
	DXGI_FORMAT format = _WICToDXGI(pixelFormat);
	if (format == DXGI_FORMAT_UNKNOWN)
	{
		if ( memcmp( &GUID_WICPixelFormat96bppRGBFixedPoint, &pixelFormat, sizeof(WICPixelFormatGUID) ) == 0 )
		{
#if (_WIN32_WINNT >= 0x0602 /*_WIN32_WINNT_WIN8*/) || defined(_WIN7_PLATFORM_UPDATE)
			if ( g_WIC2 )
			{
				memcpy( &convertGUID, &GUID_WICPixelFormat96bppRGBFloat, sizeof(WICPixelFormatGUID) );
				format = DXGI_FORMAT_R32G32B32_FLOAT;
			}
			else
#endif
			{
				memcpy( &convertGUID, &GUID_WICPixelFormat128bppRGBAFloat, sizeof(WICPixelFormatGUID) );
				format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			}
		}
		else
		{
			for( size_t i=0; i < _countof(g_WICConvert); ++i )
			{
				if ( memcmp( &g_WICConvert[i].source, &pixelFormat, sizeof(WICPixelFormatGUID) ) == 0 )
				{
					memcpy( &convertGUID, &g_WICConvert[i].target, sizeof(WICPixelFormatGUID) );

					format = _WICToDXGI( g_WICConvert[i].target );
					assert( format != DXGI_FORMAT_UNKNOWN );
					bpp = _WICBitsPerPixel( convertGUID );
					break;
				}
			}
		}

		if (format == DXGI_FORMAT_UNKNOWN)
		{
			return HRESULT_FROM_WIN32( ERROR_NOT_SUPPORTED );
		}
	}
	else
	{
		bpp = _WICBitsPerPixel( pixelFormat );
	}

#if (_WIN32_WINNT >= 0x0602 /*_WIN32_WINNT_WIN8*/) || defined(_WIN7_PLATFORM_UPDATE)
	if ( (format == DXGI_FORMAT_R32G32B32_FLOAT) && textureView != 0 )
	{
		// Special case test for optional device support for autogen mipchains for R32G32B32_FLOAT 
		UINT fmtSupport = 0;
		
		res = g_renderer->Device()->CheckFormatSupport(DXGI_FORMAT_R32G32B32_FLOAT, &fmtSupport);
		if ( FAILED(res) || !( fmtSupport & D3D11_FORMAT_SUPPORT_MIP_AUTOGEN ) )
		{
			// Use R32G32B32A32_FLOAT instead which is required for Feature Level 10.0 and up
			memcpy( &convertGUID, &GUID_WICPixelFormat128bppRGBAFloat, sizeof(WICPixelFormatGUID) );
			format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			bpp = 128;
		}
	}
#endif

	if (!bpp)
	{
		return E_FAIL;
	}

	ntUint support = 0;
	res = g_renderer->Device()->CheckFormatSupport(format, &support);
	if (FAILED(res) || !(support &  D3D11_FORMAT_SUPPORT_TEXTURE2D))
	{
		// Fallback to RGBA 32-bit format which is supported by all devices
		memcpy( &convertGUID, &GUID_WICPixelFormat32bppRGBA, sizeof(WICPixelFormatGUID) );
		format = DXGI_FORMAT_R8G8B8A8_UNORM;
		bpp = 32;
	}



	// Allocate temporary memory for image
	ntSize rowPitch = (tempWidth * bpp + 7) / 8;
	ntSize imageSize = rowPitch * tempHeight;

	std::unique_ptr<ntUchar[]> imageBuffer( new (std::nothrow) ntUchar[imageSize] );
	if (!imageBuffer)
	{
		return E_OUTOFMEMORY;
	}

	// load image data
	if (memcmp(&convertGUID, &pixelFormat, sizeof(GUID)) == 0
		&& tempWidth == width
		&& tempHeight == height)
	{
		res = frame->CopyPixels(0, static_cast<ntUint>(rowPitch), static_cast<ntUint>(imageSize), imageBuffer.get());
		if (FAILED(res))
		{
			return E_FAIL;
		}
	}
	else if (tempWidth != width || tempHeight != height)
	{
		// resize
		IWICImagingFactory* wicFactory = GetWICFactory();
		if (!wicFactory)
		{
			return E_NOINTERFACE;
		}

		NtScopedObject<IWICBitmapScaler> scaler;
		res = wicFactory->CreateBitmapScaler(&scaler);
		if (FAILED(res))
		{
			return res;
		}

		res = scaler->Initialize(frame, tempWidth, tempHeight, WICBitmapInterpolationModeFant);
		if (FAILED(res))
		{
			return res;
		}

		WICPixelFormatGUID pfScaler;
		res = scaler->GetPixelFormat(&pfScaler);
		if (FAILED(res))
		{
			return res;
		}

		if (memcmp(&convertGUID, &pfScaler, sizeof(GUID)) == 0)
		{
			res = scaler->CopyPixels(0, static_cast<ntUint>(rowPitch), static_cast<ntUint>(imageSize), imageBuffer.get());
			if (FAILED(res))
			{
				return res;
			}
		}
		else
		{
			NtScopedObject<IWICFormatConverter> FC;
			res = wicFactory->CreateFormatConverter(&FC);
			if (FAILED(res))
			{
				return res;
			}

			res = FC->Initialize(scaler.Get(), convertGUID, WICBitmapDitherTypeErrorDiffusion, 0, 0, WICBitmapPaletteTypeCustom);
			if (FAILED(res))
			{
				return res;
			}

			res = FC->CopyPixels(0, static_cast<ntUint>(rowPitch), static_cast<ntUint>(imageSize), imageBuffer.get());
			if (FAILED(res))
			{
				return res;
			}
		}
	}
	else
	{
		IWICImagingFactory* wicFactory = GetWICFactory();
		if (!wicFactory)
		{
			return E_NOINTERFACE;
		}

		NtScopedObject<IWICFormatConverter> FC;
		res = wicFactory->CreateFormatConverter(&FC);
		if (FAILED(res))
		{
			return res;
		}

		res = FC->Initialize( frame, convertGUID, WICBitmapDitherTypeErrorDiffusion, 0, 0, WICBitmapPaletteTypeCustom );
		if (FAILED(res))
		{
			return res;
		}

		res = FC->CopyPixels( 0, static_cast<ntUint>( rowPitch ), static_cast<ntUint>( imageSize ), imageBuffer.get() );  
		if (FAILED(res))
		{
			return res;
		}
	}

	// see if format is supported for auto-gen mipmaps (varies by feature level)
	bool autogen = false;
	//if (textureView != 0)
	//{
	//	ntUint fmtSupport = 0;
	//	res = renderer->GetD3DRenderer()->Device()->CheckFormatSupport(format, &fmtSupport);
	//	if (SUCCEEDED(res) && (fmtSupport & D3D11_FORMAT_SUPPORT_MIP_AUTOGEN))
	//	{
	//		autogen = true;
	//	}
	//}

	D3D11_TEXTURE2D_DESC desc;
	desc.Width = tempWidth;
	desc.Height = tempHeight;
	desc.MipLevels = (autogen) ? 0 : 1;
	desc.ArraySize = 1;
	desc.Format = format;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = usage;
	desc.CPUAccessFlags = cpuAccess;

	if (autogen)
	{
		desc.BindFlags = bindFlags | D3D11_BIND_RENDER_TARGET;
		desc.MiscFlags = misc | D3D11_RESOURCE_MISC_GENERATE_MIPS;
	}
	else
	{
		desc.BindFlags = bindFlags;
		desc.MiscFlags = misc;
	}

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = imageBuffer.get();
	initData.SysMemPitch = static_cast<ntUint>(rowPitch);
	initData.SysMemSlicePitch = static_cast<ntUint>(imageSize);

	ID3D11Texture2D* tex = nullptr;
	bool sccflag = g_renderInterface->CreateTexture2D(&desc, (autogen) ? nullptr : &initData, &tex);
	if (sccflag && tex != 0)
	{
		if (textureView != 0)
		{
			D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
			Crt::MemSet(&SRVDesc, sizeof(SRVDesc));
			SRVDesc.Format = desc.Format;

			SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			SRVDesc.Texture2D.MipLevels = (autogen) ? -1 : 1;

			sccflag = g_renderer->CreateShaderResourceView(tex, &SRVDesc, textureView);
			if (sccflag == false)
			{
				SAFE_RELEASE(tex);
				return E_FAIL;
			}

			if (autogen)
			{
				g_renderer->DeviceContext()->UpdateSubresource(tex, 0, nullptr, imageBuffer.get(), static_cast<ntUint>(rowPitch), static_cast<ntUint>(imageSize));
				g_renderer->DeviceContext()->GenerateMips(*textureView);
			}
		}

		if (texture != 0)
		{
			*texture = tex;
		}
		else
		{
			SAFE_RELEASE(tex);
		}
	}

	return S_OK;
}


/*static*/ bool NtTextureLoader::CreateWICTexture(const ntWchar* fileName, ntUint size, ID3D11Resource** texture,ID3D11ShaderResourceView** textureView)
{
	if (texture)
	{
		*texture = nullptr;
	}
	if (textureView)
	{
		*textureView = nullptr;
	}

	IWICImagingFactory* wicFactory = GetWICFactory();
	if (wicFactory == nullptr)
	{
		return false;
	}
	
	// initialize WIC
	NtScopedObject<IWICBitmapDecoder> decoder;
	
	HRESULT res = wicFactory->CreateDecoderFromFilename(fileName, 0, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &decoder);
	if (FAILED(res))
	{
		return false;
	}

	NtScopedObject<IWICBitmapFrameDecode> frame;
	res = decoder->GetFrame(0, &frame);
	if (FAILED(res))
	{
		return false;
	}

	res = CreateTextureFromWIC(frame.Get(), size, D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE, 0, 0, texture, textureView);
	if (FAILED(res))
	{
		return false;
	}

	return true;
}


/**
 *	@brief		NtDDSLoader :
 */

ntUint NtDDSLoader::BitsPerPixel( DXGI_FORMAT fmt )
{
	switch( fmt )
	{
	case DXGI_FORMAT_R32G32B32A32_TYPELESS:
	case DXGI_FORMAT_R32G32B32A32_FLOAT:
	case DXGI_FORMAT_R32G32B32A32_UINT:
	case DXGI_FORMAT_R32G32B32A32_SINT:
		return 128;

	case DXGI_FORMAT_R32G32B32_TYPELESS:
	case DXGI_FORMAT_R32G32B32_FLOAT:
	case DXGI_FORMAT_R32G32B32_UINT:
	case DXGI_FORMAT_R32G32B32_SINT:
		return 96;

	case DXGI_FORMAT_R16G16B16A16_TYPELESS:
	case DXGI_FORMAT_R16G16B16A16_FLOAT:
	case DXGI_FORMAT_R16G16B16A16_UNORM:
	case DXGI_FORMAT_R16G16B16A16_UINT:
	case DXGI_FORMAT_R16G16B16A16_SNORM:
	case DXGI_FORMAT_R16G16B16A16_SINT:
	case DXGI_FORMAT_R32G32_TYPELESS:
	case DXGI_FORMAT_R32G32_FLOAT:
	case DXGI_FORMAT_R32G32_UINT:
	case DXGI_FORMAT_R32G32_SINT:
	case DXGI_FORMAT_R32G8X24_TYPELESS:
	case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
	case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
	case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
		return 64;

	case DXGI_FORMAT_R10G10B10A2_TYPELESS:
	case DXGI_FORMAT_R10G10B10A2_UNORM:
	case DXGI_FORMAT_R10G10B10A2_UINT:
	case DXGI_FORMAT_R11G11B10_FLOAT:
	case DXGI_FORMAT_R8G8B8A8_TYPELESS:
	case DXGI_FORMAT_R8G8B8A8_UNORM:
	case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
	case DXGI_FORMAT_R8G8B8A8_UINT:
	case DXGI_FORMAT_R8G8B8A8_SNORM:
	case DXGI_FORMAT_R8G8B8A8_SINT:
	case DXGI_FORMAT_R16G16_TYPELESS:
	case DXGI_FORMAT_R16G16_FLOAT:
	case DXGI_FORMAT_R16G16_UNORM:
	case DXGI_FORMAT_R16G16_UINT:
	case DXGI_FORMAT_R16G16_SNORM:
	case DXGI_FORMAT_R16G16_SINT:
	case DXGI_FORMAT_R32_TYPELESS:
	case DXGI_FORMAT_D32_FLOAT:
	case DXGI_FORMAT_R32_FLOAT:
	case DXGI_FORMAT_R32_UINT:
	case DXGI_FORMAT_R32_SINT:
	case DXGI_FORMAT_R24G8_TYPELESS:
	case DXGI_FORMAT_D24_UNORM_S8_UINT:
	case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
	case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
	case DXGI_FORMAT_R9G9B9E5_SHAREDEXP:
	case DXGI_FORMAT_R8G8_B8G8_UNORM:
	case DXGI_FORMAT_G8R8_G8B8_UNORM:
	case DXGI_FORMAT_B8G8R8A8_UNORM:
	case DXGI_FORMAT_B8G8R8X8_UNORM:
	case DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM:
	case DXGI_FORMAT_B8G8R8A8_TYPELESS:
	case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
	case DXGI_FORMAT_B8G8R8X8_TYPELESS:
	case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:
		return 32;

	case DXGI_FORMAT_R8G8_TYPELESS:
	case DXGI_FORMAT_R8G8_UNORM:
	case DXGI_FORMAT_R8G8_UINT:
	case DXGI_FORMAT_R8G8_SNORM:
	case DXGI_FORMAT_R8G8_SINT:
	case DXGI_FORMAT_R16_TYPELESS:
	case DXGI_FORMAT_R16_FLOAT:
	case DXGI_FORMAT_D16_UNORM:
	case DXGI_FORMAT_R16_UNORM:
	case DXGI_FORMAT_R16_UINT:
	case DXGI_FORMAT_R16_SNORM:
	case DXGI_FORMAT_R16_SINT:
	case DXGI_FORMAT_B5G6R5_UNORM:
	case DXGI_FORMAT_B5G5R5A1_UNORM:

#ifdef DXGI_1_2_FORMATS
	case DXGI_FORMAT_B4G4R4A4_UNORM:
#endif
		return 16;

	case DXGI_FORMAT_R8_TYPELESS:
	case DXGI_FORMAT_R8_UNORM:
	case DXGI_FORMAT_R8_UINT:
	case DXGI_FORMAT_R8_SNORM:
	case DXGI_FORMAT_R8_SINT:
	case DXGI_FORMAT_A8_UNORM:
		return 8;

	case DXGI_FORMAT_R1_UNORM:
		return 1;

	case DXGI_FORMAT_BC1_TYPELESS:
	case DXGI_FORMAT_BC1_UNORM:
	case DXGI_FORMAT_BC1_UNORM_SRGB:
	case DXGI_FORMAT_BC4_TYPELESS:
	case DXGI_FORMAT_BC4_UNORM:
	case DXGI_FORMAT_BC4_SNORM:
		return 4;

	case DXGI_FORMAT_BC2_TYPELESS:
	case DXGI_FORMAT_BC2_UNORM:
	case DXGI_FORMAT_BC2_UNORM_SRGB:
	case DXGI_FORMAT_BC3_TYPELESS:
	case DXGI_FORMAT_BC3_UNORM:
	case DXGI_FORMAT_BC3_UNORM_SRGB:
	case DXGI_FORMAT_BC5_TYPELESS:
	case DXGI_FORMAT_BC5_UNORM:
	case DXGI_FORMAT_BC5_SNORM:
	case DXGI_FORMAT_BC6H_TYPELESS:
	case DXGI_FORMAT_BC6H_UF16:
	case DXGI_FORMAT_BC6H_SF16:
	case DXGI_FORMAT_BC7_TYPELESS:
	case DXGI_FORMAT_BC7_UNORM:
	case DXGI_FORMAT_BC7_UNORM_SRGB:
		return 8;

	default:
		return 0;
	}
}

void NtDDSLoader::GetSurfaceInfo( ntUint width, ntUint height, DXGI_FORMAT fmt, ntUint* outNumBytes, ntUint* outRowBytes, ntUint* outNumRows )
{
	ntUint numBytes = 0;
	ntUint rowBytes = 0;
	ntUint numRows = 0;

	bool bc = false;
	bool packed  = false;
	ntUint bcnumBytesPerBlock = 0;
	switch (fmt)
	{
	case DXGI_FORMAT_BC1_TYPELESS:
	case DXGI_FORMAT_BC1_UNORM:
	case DXGI_FORMAT_BC1_UNORM_SRGB:
	case DXGI_FORMAT_BC4_TYPELESS:
	case DXGI_FORMAT_BC4_UNORM:
	case DXGI_FORMAT_BC4_SNORM:
		bc=true;
		bcnumBytesPerBlock = 8;
		break;

	case DXGI_FORMAT_BC2_TYPELESS:
	case DXGI_FORMAT_BC2_UNORM:
	case DXGI_FORMAT_BC2_UNORM_SRGB:
	case DXGI_FORMAT_BC3_TYPELESS:
	case DXGI_FORMAT_BC3_UNORM:
	case DXGI_FORMAT_BC3_UNORM_SRGB:
	case DXGI_FORMAT_BC5_TYPELESS:
	case DXGI_FORMAT_BC5_UNORM:
	case DXGI_FORMAT_BC5_SNORM:
	case DXGI_FORMAT_BC6H_TYPELESS:
	case DXGI_FORMAT_BC6H_UF16:
	case DXGI_FORMAT_BC6H_SF16:
	case DXGI_FORMAT_BC7_TYPELESS:
	case DXGI_FORMAT_BC7_UNORM:
	case DXGI_FORMAT_BC7_UNORM_SRGB:
		bc = true;
		bcnumBytesPerBlock = 16;
		break;

	case DXGI_FORMAT_R8G8_B8G8_UNORM:
	case DXGI_FORMAT_G8R8_G8B8_UNORM:
		packed = true;
		break;
	}

	if (bc)
	{
		ntUint numBlocksWide = 0;
		if (width > 0)
		{
			numBlocksWide = std::max<ntUint>( 1, (width + 3) / 4 );
		}
		ntUint numBlocksHigh = 0;
		if (height > 0)
		{
			numBlocksHigh = std::max<ntUint>( 1, (height + 3) / 4 );
		}
		rowBytes = numBlocksWide * bcnumBytesPerBlock;
		numRows = numBlocksHigh;
	}
	else if (packed)
	{
		rowBytes = ( ( width + 1 ) >> 1 ) * 4;
		numRows = height;
	}
	else
	{
		size_t bpp = BitsPerPixel( fmt );
		rowBytes = ( width * bpp + 7 ) / 8; // round up to nearest byte
		numRows = height;
	}

	numBytes = rowBytes * numRows;
	if (outNumBytes)
	{
		*outNumBytes = numBytes;
	}
	if (outRowBytes)
	{
		*outRowBytes = rowBytes;
	}
	if (outNumRows)
	{
		*outNumRows = numRows;
	}
}


//--------------------------------------------------------------------------------------
#define ISBITMASK( r,g,b,a ) ( ddpf.RBitMask == r && ddpf.GBitMask == g && ddpf.BBitMask == b && ddpf.ABitMask == a )

bool NtDDSLoader::FillInitData(const sDDSInitHeader& initHeader, ntUint& width, ntUint& height, ntUint& depth, ntUint& skipMip, D3D11_SUBRESOURCE_DATA* initData)
{
	if (!initHeader.m_bitData || !initData)
	{
		return false;
	}

	skipMip = 0;
	width = 0;
	height = 0;
	depth = 0;

	ntUint numBytes = 0;
	ntUint rowBytes = 0;
	ntUint numRow = 0;
	const ntUchar* srcBits = initHeader.m_bitData;
	const ntUchar* endBits = initHeader.m_bitData + initHeader.m_bitSize;

	ntUint index = 0;
	for (ntUint j = 0; j < initHeader.m_arraySize; ++j)
	{
		ntUint w = initHeader.m_width;
		ntUint h = initHeader.m_height;
		ntUint d = initHeader.m_depth;
		for (ntUint i = 0; i < initHeader.m_mipCount; ++i)
		{
			GetSurfaceInfo(w, h, initHeader.m_format, &numBytes, &rowBytes, &numRow);

			if ((initHeader.m_mipCount <= 1) || !(initHeader.m_maxSize) || (w <= initHeader.m_maxSize && d <= initHeader.m_maxSize))
			{
				if (!width)
				{
					width = w;
					height = h;
					depth = d;
				}

				NtAsserte(index < initHeader.m_mipCount * initHeader.m_arraySize);
				initData[index].pSysMem = (const void*)srcBits;
				initData[index].SysMemPitch = static_cast<ntUint>(rowBytes);
				initData[index].SysMemSlicePitch = static_cast<ntUint>(numBytes);
				++index;
			}
			else
			{
				++skipMip;
			}

			if (srcBits + (numBytes * d) > endBits)
			{
				// HRESULT_FROM_WIN32( ERROR_HANDLE_EOF )
				return false;
			}

			srcBits += numBytes * d;

			w = w >> 1;
			h = h >> 1;
			d = d >> 1;
			if (w == 0)
			{
				w = 1;
			}
			if (h == 0)
			{
				h = 1;
			}
			if (d == 0)
			{
				d = 1;
			}
		}
	}

	return index > 0 ? true : false;
}

DXGI_FORMAT NtDDSLoader::GetDXGIFormat( const DDS_PIXELFORMAT& ddpf )
{
	if (ddpf.flags & DDS_RGB)
	{
		// Note that sRGB formats are written using the "DX10" extended header

		switch (ddpf.RGBBitCount)
		{
		case 32:
			if (ISBITMASK(0x000000ff,0x0000ff00,0x00ff0000,0xff000000))
			{
				return DXGI_FORMAT_R8G8B8A8_UNORM;
			}

			if (ISBITMASK(0x00ff0000,0x0000ff00,0x000000ff,0xff000000))
			{
				return DXGI_FORMAT_B8G8R8A8_UNORM;
			}

			if (ISBITMASK(0x00ff0000,0x0000ff00,0x000000ff,0x00000000))
			{
				return DXGI_FORMAT_B8G8R8X8_UNORM;
			}

			// No DXGI format maps to ISBITMASK(0x000000ff,0x0000ff00,0x00ff0000,0x00000000) aka D3DFMT_X8B8G8R8

			// Note that many common DDS reader/writers (including D3DX) swap the
			// the RED/BLUE masks for 10:10:10:2 formats. We assumme
			// below that the 'backwards' header mask is being used since it is most
			// likely written by D3DX. The more robust solution is to use the 'DX10'
			// header extension and specify the DXGI_FORMAT_R10G10B10A2_UNORM format directly

			// For 'correct' writers, this should be 0x000003ff,0x000ffc00,0x3ff00000 for RGB data
			if (ISBITMASK(0x3ff00000,0x000ffc00,0x000003ff,0xc0000000))
			{
				return DXGI_FORMAT_R10G10B10A2_UNORM;
			}

			// No DXGI format maps to ISBITMASK(0x000003ff,0x000ffc00,0x3ff00000,0xc0000000) aka D3DFMT_A2R10G10B10

			if (ISBITMASK(0x0000ffff,0xffff0000,0x00000000,0x00000000))
			{
				return DXGI_FORMAT_R16G16_UNORM;
			}

			if (ISBITMASK(0xffffffff,0x00000000,0x00000000,0x00000000))
			{
				// Only 32-bit color channel format in D3D9 was R32F
				return DXGI_FORMAT_R32_FLOAT; // D3DX writes this out as a FourCC of 114
			}
			break;

		case 24:
			// No 24bpp DXGI formats aka D3DFMT_R8G8B8
			break;

		case 16:
			if (ISBITMASK(0x7c00,0x03e0,0x001f,0x8000))
			{
				return DXGI_FORMAT_B5G5R5A1_UNORM;
			}
			if (ISBITMASK(0xf800,0x07e0,0x001f,0x0000))
			{
				return DXGI_FORMAT_B5G6R5_UNORM;
			}

			// No DXGI format maps to ISBITMASK(0x7c00,0x03e0,0x001f,0x0000) aka D3DFMT_X1R5G5B5

#ifdef DXGI_1_2_FORMATS
			if (ISBITMASK(0x0f00,0x00f0,0x000f,0xf000))
			{
				return DXGI_FORMAT_B4G4R4A4_UNORM;
			}

			// No DXGI format maps to ISBITMASK(0x0f00,0x00f0,0x000f,0x0000) aka D3DFMT_X4R4G4B4
#endif

			// No 3:3:2, 3:3:2:8, or paletted DXGI formats aka D3DFMT_A8R3G3B2, D3DFMT_R3G3B2, D3DFMT_P8, D3DFMT_A8P8, etc.
			break;
		}
	}
	else if (ddpf.flags & DDS_LUMINANCE)
	{
		if (8 == ddpf.RGBBitCount)
		{
			if (ISBITMASK(0x000000ff,0x00000000,0x00000000,0x00000000))
			{
				return DXGI_FORMAT_R8_UNORM; // D3DX10/11 writes this out as DX10 extension
			}

			// No DXGI format maps to ISBITMASK(0x0f,0x00,0x00,0xf0) aka D3DFMT_A4L4
		}

		if (16 == ddpf.RGBBitCount)
		{
			if (ISBITMASK(0x0000ffff,0x00000000,0x00000000,0x00000000))
			{
				return DXGI_FORMAT_R16_UNORM; // D3DX10/11 writes this out as DX10 extension
			}
			if (ISBITMASK(0x000000ff,0x00000000,0x00000000,0x0000ff00))
			{
				return DXGI_FORMAT_R8G8_UNORM; // D3DX10/11 writes this out as DX10 extension
			}
		}
	}
	else if (ddpf.flags & DDS_ALPHA)
	{
		if (8 == ddpf.RGBBitCount)
		{
			return DXGI_FORMAT_A8_UNORM;
		}
	}
	else if (ddpf.flags & DDS_FOURCC)
	{
		if (MAKEFOURCC( 'D', 'X', 'T', '1' ) == ddpf.fourCC)
		{
			return DXGI_FORMAT_BC1_UNORM;
		}
		if (MAKEFOURCC( 'D', 'X', 'T', '3' ) == ddpf.fourCC)
		{
			return DXGI_FORMAT_BC2_UNORM;
		}
		if (MAKEFOURCC( 'D', 'X', 'T', '5' ) == ddpf.fourCC)
		{
			return DXGI_FORMAT_BC3_UNORM;
		}

		// While pre-mulitplied alpha isn't directly supported by the DXGI formats,
		// they are basically the same as these BC formats so they can be mapped
		if (MAKEFOURCC( 'D', 'X', 'T', '2' ) == ddpf.fourCC)
		{
			return DXGI_FORMAT_BC2_UNORM;
		}
		if (MAKEFOURCC( 'D', 'X', 'T', '4' ) == ddpf.fourCC)
		{
			return DXGI_FORMAT_BC3_UNORM;
		}

		if (MAKEFOURCC( 'A', 'T', 'I', '1' ) == ddpf.fourCC)
		{
			return DXGI_FORMAT_BC4_UNORM;
		}
		if (MAKEFOURCC( 'B', 'C', '4', 'U' ) == ddpf.fourCC)
		{
			return DXGI_FORMAT_BC4_UNORM;
		}
		if (MAKEFOURCC( 'B', 'C', '4', 'S' ) == ddpf.fourCC)
		{
			return DXGI_FORMAT_BC4_SNORM;
		}

		if (MAKEFOURCC( 'A', 'T', 'I', '2' ) == ddpf.fourCC)
		{
			return DXGI_FORMAT_BC5_UNORM;
		}
		if (MAKEFOURCC( 'B', 'C', '5', 'U' ) == ddpf.fourCC)
		{
			return DXGI_FORMAT_BC5_UNORM;
		}
		if (MAKEFOURCC( 'B', 'C', '5', 'S' ) == ddpf.fourCC)
		{
			return DXGI_FORMAT_BC5_SNORM;
		}

		// BC6H and BC7 are written using the "DX10" extended header

		if (MAKEFOURCC( 'R', 'G', 'B', 'G' ) == ddpf.fourCC)
		{
			return DXGI_FORMAT_R8G8_B8G8_UNORM;
		}
		if (MAKEFOURCC( 'G', 'R', 'G', 'B' ) == ddpf.fourCC)
		{
			return DXGI_FORMAT_G8R8_G8B8_UNORM;
		}

		// Check for D3DFORMAT enums being set here
		switch( ddpf.fourCC )
		{
		case 36: // D3DFMT_A16B16G16R16
			return DXGI_FORMAT_R16G16B16A16_UNORM;

		case 110: // D3DFMT_Q16W16V16U16
			return DXGI_FORMAT_R16G16B16A16_SNORM;

		case 111: // D3DFMT_R16F
			return DXGI_FORMAT_R16_FLOAT;

		case 112: // D3DFMT_G16R16F
			return DXGI_FORMAT_R16G16_FLOAT;

		case 113: // D3DFMT_A16B16G16R16F
			return DXGI_FORMAT_R16G16B16A16_FLOAT;

		case 114: // D3DFMT_R32F
			return DXGI_FORMAT_R32_FLOAT;

		case 115: // D3DFMT_G32R32F
			return DXGI_FORMAT_R32G32_FLOAT;

		case 116: // D3DFMT_A32B32G32R32F
			return DXGI_FORMAT_R32G32B32A32_FLOAT;
		}
	}

	return DXGI_FORMAT_UNKNOWN;
}


bool NtDDSLoader::LoadTextureDataFromFile(const ntWchar* fileName, std::unique_ptr<ntUchar[]>& ddsBuffer, DDS_HEADER** header, ntUchar** bitData, ntUint* bitSize)
{
	if (!header || !bitData || !bitSize)
	{
		return false;
	}

	const ntWchar* fullPath = g_resManager->GetPath(fileName);
	if (nullptr == fullPath)
	{
		return false;
	}


#if (_WIN32_WINNT >= 0x0602 /*_WIN32_WINNT_WIN8*/)
	scopeGuard hFile(CreateFile2(fullPath, GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING, nullptr));
#else
	scopeGuard hFile(CreateFile(fullPath, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr));
#endif

	if (!hFile.m_handle)
	{
		NtASSERTE(0 && L"file open failed");
		return false;
	}

	LARGE_INTEGER fileSize = {0, };

#if (_WIN32_WINNT >= _WIN32_WINNT_VISTA)
	FILE_STANDARD_INFO fileInfo;
	if ( !GetFileInformationByHandleEx( hFile.m_handle, FileStandardInfo, &fileInfo, sizeof(fileInfo) ) )
	{
		return false;
	}
	fileSize = fileInfo.EndOfFile;
#else
	GetFileSizeEx( hFile.get(), &fileSize );
#endif

	// File is too big for 32-bit allocation, so reject read
	if (fileSize.HighPart > 0)
	{
		NtASSERTE(0 && "File is too big for 32-bit allocation, so reject read");
		return false;
	}

	// Need at least enough data to fill the header and magic number to be a valid DDS
	if (fileSize.LowPart < ( sizeof(DDS_HEADER) + sizeof(uint32_t) ) )
	{
		return false;
	}

	ddsBuffer.reset(new ntUchar [fileSize.LowPart]);
	if (!ddsBuffer)
	{
		return false;
	}

	DWORD bytesRead = 0;
	if (!ReadFile(hFile.m_handle, ddsBuffer.get(), fileSize.LowPart, &bytesRead, nullptr))
	{
		return false;
	}

	if (bytesRead < fileSize.LowPart)
	{
		return false;
	}

	// DDS files always start with the same magic number ("DDS ")
	ntUint magicNumber = *(const ntUint*)(ddsBuffer.get());
	if (magicNumber != DDS_MAGIC)
	{
		return false;
	}

	DDS_HEADER* hdr = reinterpret_cast<DDS_HEADER*>(ddsBuffer.get() + sizeof(ntUint));

	// verify header to validate dds file
	if (hdr->size != sizeof(DDS_HEADER) || hdr->ddspf.size != sizeof(DDS_PIXELFORMAT))
	{
		return false;
	}

	// check for DX10 extension
	bool dxt10Header = false;
	if ((hdr->ddspf.flags & DDS_FOURCC) &&
		(MAKEFOURCC('D', 'X', '1', '0') == hdr->ddspf.fourCC))
	{
		// must be long enough for both headers and magic value
		if (fileSize.LowPart < (sizeof(DDS_HEADER) + sizeof(ntUint) + sizeof(DDS_HEADER_DXT10)))
		{
			return false;
		}

		dxt10Header = true;
	}

	*header = hdr;
	ptrdiff_t offset = sizeof(ntUint) + sizeof(DDS_HEADER) + (dxt10Header ? sizeof(DDS_HEADER_DXT10) : 0);
	*bitData = ddsBuffer.get() + offset;
	*bitSize = fileSize.LowPart - offset;

	return true;
}

bool NtDDSLoader::CreateTextureFromDDS(const DirectX::DDS_HEADER* header, const ntUchar* bitData, ntUint bitSize, ntUint maxSize, 
	D3D11_USAGE usage, ntUint bindFlags, ntUint cpuAccessFlags, ntUint miscFlags, bool forceSRGB, ID3D11Resource** texture, ID3D11ShaderResourceView** textureView)
{
	ntUint width = header->width;
	ntUint height = header->height;
	ntUint depth = header->depth;

	ntUint resDim = D3D11_RESOURCE_DIMENSION_UNKNOWN;
	ntUint arraySize = 1;
	DXGI_FORMAT format = DXGI_FORMAT_UNKNOWN;
	bool isCubeMap = false;

	ntUint mipCount = header->mipMapCount;
	if (0 == mipCount)
	{
		mipCount = 1;
	}

	if ((header->ddspf.flags & DDS_FOURCC) && (MAKEFOURCC('D', 'X', '1', '0') == header->ddspf.fourCC))
	{
		const DDS_HEADER_DXT10* d3d10ext = reinterpret_cast<const DDS_HEADER_DXT10*>((const ntChar*)header + sizeof(DDS_HEADER));

		arraySize = d3d10ext->arraySize;
		if (arraySize == 0)
		{
			return false;
		}

		if (BitsPerPixel(d3d10ext->dxgiFormat) == 0)
		{
			return false;
		}

		format = d3d10ext->dxgiFormat;

		switch(d3d10ext->resourceDimension)
		{
		case D3D11_RESOURCE_DIMENSION_TEXTURE1D:
			{
				// d3dx writes 1D textures with a fixted height of 1
				if ((header->flags & (DDS_HEIGHT) && height != 1))
				{
					// HRESULT_FROM_WIN32( ERROR_INVALID_DATA )
					return false;
				}
				height = depth = 1;
			}
			break;

		case D3D11_RESOURCE_DIMENSION_TEXTURE2D:
			{
				if (d3d10ext->miscFlag & D3D11_RESOURCE_MISC_TEXTURECUBE)
				{
					arraySize *= 6;
					isCubeMap = true;
				}
				depth = 1;
			}
			break;

		case D3D11_RESOURCE_DIMENSION_TEXTURE3D:
			{
				if (!(header->flags & DDS_HEADER_FLAGS_VOLUME))
				{
					// HRESULT_FROM_WIN32( ERROR_INVALID_DATA )
					return false;
				}

				if (arraySize > 1)
				{
					// HRESULT_FROM_WIN32( ERROR_NOT_SUPPORTED )
					return false;
				}
			}
			break;

		default:
			// HRESULT_FROM_WIN32( ERROR_NOT_SUPPORTED )
			return false;
		}

		resDim = d3d10ext->resourceDimension;
	}
	else
	{
		format = GetDXGIFormat(header->ddspf);

		if (format == DXGI_FORMAT_UNKNOWN)
		{
			// HRESULT_FROM_WIN32( ERROR_NOT_SUPPORTED )
			return false;
		}

		if (header->flags & DDS_HEADER_FLAGS_VOLUME)
		{
			resDim = D3D11_RESOURCE_DIMENSION_TEXTURE3D;
		}
		else
		{
			if (header->caps2 & DDS_CUBEMAP)
			{
				// require all six faces to be defined
				if ((header->caps2 & DDS_CUBEMAP_ALLFACES) != DDS_CUBEMAP_ALLFACES)
				{
					// HRESULT_FROM_WIN32( ERROR_NOT_SUPPORTED )
					return false;
				}

				arraySize = 7;
				isCubeMap = true;
			}

			depth = 1;
			resDim = D3D11_RESOURCE_DIMENSION_TEXTURE2D;
		}
	}

	// bound sizes (for security purpose we don't trust DDS file metadata larger than the D3D 11.x hardware requirement)
	if (mipCount > D3D11_REQ_MIP_LEVELS)
	{
		// HRESULT_FROM_WIN32( ERROR_NOT_SUPPORTED )
		return false;
	}

	switch(resDim)
	{
	case D3D11_RESOURCE_DIMENSION_TEXTURE1D:
		{
			if ((arraySize > D3D11_REQ_TEXTURE1D_ARRAY_AXIS_DIMENSION) || (width > D3D11_REQ_TEXTURE1D_U_DIMENSION))
			{
				// HRESULT_FROM_WIN32( ERROR_NOT_SUPPORTED )
				return false;
			}
		}
		break;

	case D3D11_RESOURCE_DIMENSION_TEXTURE2D:
		{
			if (isCubeMap)
			{
				if ((arraySize > D3D11_REQ_TEXTURE2D_ARRAY_AXIS_DIMENSION) || (width > D3D11_REQ_TEXTURECUBE_DIMENSION) || (height > D3D11_REQ_TEXTURECUBE_DIMENSION))
				{
					// HRESULT_FROM_WIN32( ERROR_NOT_SUPPORTED )
					return false;
				}
			}
			else if ((arraySize > D3D11_REQ_TEXTURE1D_ARRAY_AXIS_DIMENSION) || (width > D3D11_REQ_TEXTURE2D_U_OR_V_DIMENSION) || (height > D3D11_REQ_TEXTURE2D_U_OR_V_DIMENSION))
			{
				// HRESULT_FROM_WIN32( ERROR_NOT_SUPPORTED )
				return false;
			}
		}
		break;

	case D3D11_RESOURCE_DIMENSION_TEXTURE3D:
		{
			if ((arraySize > 1) || (width > D3D11_REQ_TEXTURE3D_U_V_OR_W_DIMENSION) || (height > D3D11_REQ_TEXTURE3D_U_V_OR_W_DIMENSION) || (depth > D3D11_REQ_TEXTURE3D_U_V_OR_W_DIMENSION))
			{
				// HRESULT_FROM_WIN32( ERROR_NOT_SUPPORTED )
				return false;
			}
		}
		break;
	}

	// create the texture
	std::unique_ptr<D3D11_SUBRESOURCE_DATA[]> initData( new D3D11_SUBRESOURCE_DATA[mipCount * arraySize] );
	if (!initData)
	{
		return false;
	}

	ntUint skipMip = 0;
	ntUint twidth = 0;
	ntUint theight = 0;
	ntUint tdepth = 0;

	sDDSInitHeader initHeader;
	initHeader.m_width = width;
	initHeader.m_height = height;
	initHeader.m_depth = depth;
	initHeader.m_mipCount = mipCount;
	initHeader.m_arraySize = arraySize;
	initHeader.m_format = format;
	initHeader.m_maxSize = maxSize;
	initHeader.m_bitSize = bitSize;
	initHeader.m_bitData = bitData;
	bool res = FillInitData(initHeader, twidth, theight, tdepth, skipMip, initData.get());
	if (false == res)
	{
		return false;
	}

	res = CreateD3DResources(resDim, twidth, theight, tdepth, mipCount - skipMip, arraySize, format, usage, bindFlags, cpuAccessFlags, miscFlags, forceSRGB, isCubeMap, initData.get(), texture, textureView);
	if (FAILED(res) && !maxSize && (mipCount > 1))
	{
		// retry with a maxsize determined by feature level
		switch (g_renderInterface->GetFeatureLevel())
		{
		case D3D_FEATURE_LEVEL_9_1:
		case D3D_FEATURE_LEVEL_9_2:
			if (isCubeMap)
			{
				maxSize = 512; /*D3D_FL9_1_REQ_TEXTURECUBE_DIMENSION*/
			}
			else
			{
				maxSize = (resDim == D3D11_RESOURCE_DIMENSION_TEXTURE3D) ? 256 /*D3D_FL9_1_REQ_TEXTURE3D_U_V_OR_W_DIMENSION*/
					: 2048; /*D3D_FL9_1_REQ_TEXTURE2D_U_OR_V_DIMENSION*/
			}
			break;

		case D3D_FEATURE_LEVEL_9_3:
			maxSize = (resDim == D3D11_RESOURCE_DIMENSION_TEXTURE3D)
				? 256 /*D3D_FL9_1_REQ_TEXTURE3D_U_V_OR_W_DIMENSION*/
				: 4096 /*D3D_FL9_3_REQ_TEXTURE2D_U_OR_V_DIMENSION*/;
			break;

		default: // D3D_FEATURE_LEVEL_10_0 & D3D_FEATURE_LEVEL_10_1
			maxSize = (resDim == D3D11_RESOURCE_DIMENSION_TEXTURE3D)
				? 2048 /*D3D10_REQ_TEXTURE3D_U_V_OR_W_DIMENSION*/
				: 8192 /*D3D10_REQ_TEXTURE2D_U_OR_V_DIMENSION*/;
			break;
		}

		initHeader.m_maxSize = maxSize;
		res = FillInitData(initHeader, twidth, theight, tdepth, skipMip, initData.get());
		if (SUCCEEDED(res))
		{
			res = CreateD3DResources(resDim, twidth, theight, tdepth, mipCount - skipMip, arraySize, format, usage, bindFlags, cpuAccessFlags, miscFlags, forceSRGB, isCubeMap, initData.get(), texture, textureView);
			if (FAILED(res))
			{
				return false;
			}
		}
	}
	
	return true;
}






/*static*/ bool NtJPGLoader::LoadTextureDataFromFile(const ntWchar* fileName, std::unique_ptr<ntUchar[]>& outBuffer, ntUchar** bitData, ntUint& maxSize, ntUint& width, ntUint& height, ntSize& textureSize, ntUint& bpp)
{
	return true;
}

/*static*/ bool NtJPGLoader::CreateTextureFromJPG(const ntUchar* buffer,
	const ntUint& width,
	const ntUint& height, 
	const ntUint bpp, 
	D3D11_USAGE usage,
	ntUint bindFlags,
	ntUint cpuAccessFlags,
	ntUint miscFlags,
	ID3D11Resource** texture,
	ID3D11ShaderResourceView** textureView)
 {
	DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// Allocate temporary memory for image
	//ntUint rowPitch = (width * bpp + 7) / 8;
	ntUint rowPitch = (width * bpp) / 8;
	ntUint imageSize = rowPitch * height;

	D3D11_TEXTURE2D_DESC desc;
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = format;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = usage;
	desc.CPUAccessFlags = cpuAccessFlags;
	desc.BindFlags = bindFlags;
	desc.MiscFlags = miscFlags;

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = buffer;
	initData.SysMemPitch = rowPitch;
	initData.SysMemSlicePitch = imageSize;

	ID3D11Texture2D* tex = nullptr;
	bool sccflag = g_renderInterface->CreateTexture2D(&desc, &initData, &tex);
	if (sccflag && tex != 0)
	{
		if (textureView != 0)
		{
			D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
			Crt::MemSet(&SRVDesc, sizeof(SRVDesc));
			SRVDesc.Format = desc.Format;

			SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			SRVDesc.Texture2D.MipLevels = 1;

			sccflag = g_renderer->CreateShaderResourceView(tex, &SRVDesc, textureView);
			if (sccflag == false)
			{
				SAFE_RELEASE(tex);
				return false;
			}
		}

		if (texture != 0)
		{
			*texture = tex;
		}
		else
		{
			SAFE_RELEASE(tex);
		}
	}
	
	return true;
}


}
