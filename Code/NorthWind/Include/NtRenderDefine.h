
#pragma once

#ifdef USE_DIRECTX
		typedef ID3D11Buffer		NtVertexBuffer;
		typedef ID3D11Buffer		NtIndexBuffer;
		typedef D3D11_USAGE			NtResourceUsage;
		typedef D3D11_BIND_FLAG		NtBindFlag;
		typedef DXGI_FORMAT			NTCOLORFMT;

#ifdef USE_DIRECTX
	#include "NtD3DRenderer.h"
	//typedef nt::RENDERER::NtD3DRenderer	NtRenderEngine;
		#include "NtD3D11.h"
		struct NtRenderBufferParam
		{
			const D3D11_BUFFER_DESC* m_desc;
			D3D11_SUBRESOURCE_DATA* m_resData;
			ID3D11Buffer** m_buffer;
		};
#endif

#endif	// USE_DIRECTX


		enum eColorFormat
		{
			NT_FMT_NONE,			// D3DFMT_UNKNOWN	DXGI_FORMAT_UNKNOWN
			NT_FMT_A8R8G8B8,		// D3DFMT_A8R8G8B8	DXGI_FORMAT_B8G8R8A8_UNORM & DXGI_FORMAT_B8G8R8A8_UNORM_SRGB1
			NT_FMT_X8R8G8B8,		// D3DFMT_X8R8G8B8	DXGI_FORMAT_B8G8R8X8_UNORM & DXGI_FORMAT_B8G8R8X8_UNORM_SRGB1
			NT_FMT_D16,				// D3DFMT_D16	DXGI_FORMAT_D16_UNORM
			NT_D24S8,				// D3DFMT_S8D24	DXGI_FORMAT_D24_UNORM_S8_UINT
			NT_FMT_MAX,
		};
			
		enum eVSShaderModel
		{
			NT_VS_1_1,
			NT_VS_2_0,
			NT_VS_3_0,
			NT_VS_4_0,
			NT_VS_5_0,
		
			NT_MAX_VSSHADER_MODEL,
		};

		enum ePSShaderModel
		{
			NT_PS_1_1,
			NT_PS_2_0,
			NT_PS_3_0,
			NT_PS_4_0,
			NT_PS_5_0,

			NT_MAX_PSSHADER_MODEL,
		};

			//D3DFMT_R5G6B5	DXGI_FORMAT_B5G6R5_UNORM2
			//D3DFMT_A1R5G5B5	DXGI_FORMAT_B5G5R5A1_UNORM2
			//D3DFMT_A4R4G4B4	DXGI_FORMAT_B4G4R4A4_UNORM3
			//D3DFMT_A8B8G8R8	DXGI_FORMAT_R8G8B8A8_UNORM & DXGI_FORMAT_R8G8B8A8_UNORM_SRGB
			//D3DFMT_G16R16	DXGI_FORMAT_R16G16_UNORM
			//D3DFMT_A16B16G16R16	DXGI_FORMAT_R16G16B16A16_UNORM
			//D3DFMT_V8U8	DXGI_FORMAT_R8G8_SNORM
			////D3DFMT_DXT1	DXGI_FORMAT_BC1_UNORM & DXGI_FORMAT_BC1_UNORM_SRGB
			////D3DFMT_DXT2	DXGI_FORMAT_BC1_UNORM & DXGI_FORMAT_BC1_UNORM_SRGB Note: DXT1 and DXT2 are the same from an API/hardware perspective... only difference was 'premultiplied alpha', which can be tracked by an application and doesn't need a separate format.
			////D3DFMT_DXT3	DXGI_FORMAT_BC2_UNORM & DXGI_FORMAT_BC2_UNORM_SRGB
			////D3DFMT_DXT4	DXGI_FORMAT_BC2_UNORM & DXGI_FORMAT_BC2_UNORM_SRGB Note: DXT3 and DXT4 are the same from an API/hardware perspective... only difference was 'premultiplied alpha', which can be tracked by an application and doesn't need a separate format.
			////D3DFMT_DXT5	DXGI_FORMAT_BC3_UNORM & DXGI_FORMAT_BC3_UNORM_SRGB
			//D3DFMT_D16 & D3DFMT_D16_LOCKABLE	DXGI_FORMAT_D16_UNORM
			//
			
			//D3DFMT_D32F_LOCKABLE	DXGI_FORMAT_D32_FLOAT
			//D3DFMT_INDEX16	DXGI_FORMAT_R16_UINT
			//D3DFMT_INDEX32	DXGI_FORMAT_R32_UINT
			//D3DFMT_Q16W16V16U16	DXGI_FORMAT_R16G16B16A16_SNORM
			//D3DFMT_A16B16G16R16F	DXGI_FORMAT_R16G16B16A16_FLOAT
			//D3DFMT_R32F	DXGI_FORMAT_R32_FLOAT
			//D3DFMT_A32B32G32R32F	DXGI_FORMAT_R32G32B32A32_FLOAT

		/*
		BYTE gDX9AttributeType[VertexFormat::AT_QUANTITY] =
		{
			255,                        // AT_NONE (unsupported)
			D3DDECLTYPE_FLOAT1,         // AT_FLOAT1
			D3DDECLTYPE_FLOAT2,         // AT_FLOAT2
			D3DDECLTYPE_FLOAT3,         // AT_FLOAT3
			D3DDECLTYPE_FLOAT4,         // AT_FLOAT4
			255,                        // AT_HALF1 (unsupported)
			D3DDECLTYPE_FLOAT16_2,      // AT_HALF2
			255,                        // AT_HALF3 (unsupported)
			D3DDECLTYPE_FLOAT16_4,      // AT_HALF4
			D3DDECLTYPE_D3DCOLOR,       // AT_UBYTE4
			255,                        // AT_SHORT1 (unsupported)
			D3DDECLTYPE_SHORT2,         // AT_SHORT2
			D3DDECLTYPE_SHORT4          // AT_SHORT4
		};

		BYTE gDX9AttributeUsage[VertexFormat::AU_QUANTITY] =
		{
			255,                        // AU_NONE (unsupported)
			D3DDECLUSAGE_POSITION,      // AU_POSITION
			D3DDECLUSAGE_NORMAL,        // AU_NORMAL
			D3DDECLUSAGE_TANGENT,       // AU_TANGENT
			D3DDECLUSAGE_BINORMAL,      // AU_BINORMAL
			D3DDECLUSAGE_TEXCOORD,      // AU_TEXCOORD
			D3DDECLUSAGE_COLOR,         // AU_COLOR
			D3DDECLUSAGE_BLENDINDICES,  // AU_BLENDINDICES
			D3DDECLUSAGE_BLENDWEIGHT,   // AU_BLENDWEIGHT
			D3DDECLUSAGE_TESSFACTOR,    // AU_FOGCOORD
			D3DDECLUSAGE_PSIZE          // AU_PSIZE
		};

		DWORD gDX9BufferUsage[Buffer::BU_QUANTITY] =
		{
			D3DUSAGE_WRITEONLY,         // BU_STATIC
			D3DUSAGE_DYNAMIC,           // BU_DYNAMIC
			D3DUSAGE_RENDERTARGET,      // BU_RENDERTARGET
			D3DUSAGE_DEPTHSTENCIL,      // BU_DEPTHSTENCIL
			0                           // BU_TEXTURE
		};

		DWORD gDX9BufferLocking[Buffer::BL_QUANTITY] =
		{
			D3DLOCK_READONLY,           // BL_READ_ONLY
			0,                          // BL_WRITE_ONLY
			0                           // BL_READ_WRITE
		};

		D3DFORMAT gDX9TextureFormat[Texture::TF_QUANTITY] =
		{
			D3DFMT_UNKNOWN,             // TF_NONE
			D3DFMT_R5G6B5,              // TF_R5G6B5
			D3DFMT_A1R5G5B5,            // TF_A1R5G5B5
			D3DFMT_A4R4G4B4,            // TF_A4R4G4B4
			D3DFMT_A8,                  // TF_A8
			D3DFMT_L8,                  // TF_L8
			D3DFMT_A8L8,                // TF_A8L8
			D3DFMT_R8G8B8,              // TF_R8G8B8
			D3DFMT_A8R8G8B8,            // TF_A8R8G8B8
			D3DFMT_A8B8G8R8,            // TF_A8B8G8R8
			D3DFMT_L16,                 // TF_L16
			D3DFMT_G16R16,              // TF_G16R16
			D3DFMT_A16B16G16R16,        // TF_A16B16G16R16
			D3DFMT_R16F,                // TF_R16F
			D3DFMT_G16R16F,             // TF_G16R16F
			D3DFMT_A16B16G16R16F,       // TF_A16B16G16R16F
			D3DFMT_R32F,                // TF_R32F
			D3DFMT_G32R32F,             // TF_G32R32F
			D3DFMT_A32B32G32R32F,       // TF_A32B32G32R32F
			D3DFMT_DXT1,                // TF_DXT1
			D3DFMT_DXT3,                // TF_DXT3
			D3DFMT_DXT5,                // TF_DXT5
			D3DFMT_D24S8                // TF_D24S8
		};

		DWORD gDX9MinFilter[Shader::SF_QUANTITY] =
		{
			D3DTEXF_NONE,               // Shader::SF_NONE
			D3DTEXF_POINT,              // Shader::SF_NEAREST
			D3DTEXF_LINEAR,             // Shader::SF_LINEAR
			D3DTEXF_POINT,              // Shader::SF_NEAREST_NEAREST
			D3DTEXF_POINT,              // Shader::SF_NEAREST_LINEAR
			D3DTEXF_LINEAR,             // Shader::SF_LINEAR_NEAREST
			D3DTEXF_LINEAR              // Shader::SF_LINEAR_LINEAR
		};

		DWORD gDX9MipFilter[Shader::SF_QUANTITY] =
		{
			D3DTEXF_NONE,               // Shader::SF_NONE
			D3DTEXF_NONE,               // Shader::SF_NEAREST
			D3DTEXF_NONE,               // Shader::SF_LINEAR
			D3DTEXF_POINT,              // Shader::SF_NEAREST_NEAREST
			D3DTEXF_LINEAR,             // Shader::SF_NEAREST_LINEAR
			D3DTEXF_POINT,              // Shader::SF_LINEAR_NEAREST
			D3DTEXF_LINEAR              // Shader::SF_LINEAR_LINEAR
		};

		DWORD gDX9WrapMode[Shader::SC_QUANTITY] =
		{
			0,                          // Shader::SC_NONE
			D3DTADDRESS_CLAMP,          // Shader::SC_CLAMP
			D3DTADDRESS_WRAP,           // Shader::SC_REPEAT
			D3DTADDRESS_MIRROR,         // Shader::SC_MIRRORED_REPEAT
			D3DTADDRESS_BORDER,         // Shader::SC_CLAMP_BORDER
			D3DTADDRESS_CLAMP           // Shader::SC_CLAMP_EDGE
		};

		DWORD gDX9AlphaSrcBlend[AlphaState::SBM_QUANTITY] =
		{
			D3DBLEND_ZERO,              // AlphaState::SBM_ZERO
			D3DBLEND_ONE,               // AlphaState::SBM_ONE
			D3DBLEND_DESTCOLOR,         // AlphaState::SBM_DST_COLOR
			D3DBLEND_INVDESTCOLOR,      // AlphaState::SBM_ONE_MINUS_DST_COLOR
			D3DBLEND_SRCALPHA,          // AlphaState::SBM_SRC_ALPHA
			D3DBLEND_INVSRCALPHA,       // AlphaState::SBM_ONE_MINUS_SRC_ALPHA
			D3DBLEND_DESTALPHA,         // AlphaState::SBM_DST_ALPHA
			D3DBLEND_INVDESTALPHA,      // AlphaState::SBM_ONE_MINUS_DST_ALPHA
			D3DBLEND_SRCALPHASAT,       // AlphaState::SBM_SRC_ALPHA_SATURATE
			0,                          // AlphaState::SBM_CONSTANT_COLOR
			0,                          // AlphaState::SBM_ONE_MINUS_CONSTANT_COLOR
			0,                          // AlphaState::SBM_CONSTANT_ALPHA
			0                           // AlphaState::SBM_ONE_MINUS_CONSTANT_ALPHA
		};

		DWORD gDX9AlphaDstBlend[AlphaState::DBM_QUANTITY] =
		{
			D3DBLEND_ZERO,              // AlphaState::DBM_ZERO
			D3DBLEND_ONE,               // AlphaState::DBM_ONE
			D3DBLEND_SRCCOLOR,          // AlphaState::DBM_SRC_COLOR
			D3DBLEND_INVSRCCOLOR,       // AlphaState::DBM_ONE_MINUS_SRC_COLOR
			D3DBLEND_SRCALPHA,          // AlphaState::DBM_SRC_ALPHA
			D3DBLEND_INVSRCALPHA,       // AlphaState::DBM_ONE_MINUS_SRC_ALPHA
			D3DBLEND_DESTALPHA,         // AlphaState::DBM_DST_ALPHA
			D3DBLEND_INVDESTALPHA,      // AlphaState::DBM_ONE_MINUS_DST_ALPHA
			0,                          // AlphaState::DBM_CONSTANT_COLOR
			0,                          // AlphaState::DBM_ONE_MINUS_CONSTANT_COLOR
			0,                          // AlphaState::DBM_CONSTANT_ALPHA
			0                           // AlphaState::DBM_ONE_MINUS_CONSTANT_ALPHA
		};

		DWORD gDX9AlphaCompare[AlphaState::CM_QUANTITY] =
		{
			D3DCMP_NEVER,               // AlphaState::CM_NEVER
			D3DCMP_LESS,                // AlphaState::CM_LESS
			D3DCMP_EQUAL,               // AlphaState::CM_EQUAL
			D3DCMP_LESSEQUAL,           // AlphaState::CM_LEQUAL
			D3DCMP_GREATER,             // AlphaState::CM_GREATER
			D3DCMP_NOTEQUAL,            // AlphaState::CM_NOTEQUAL
			D3DCMP_GREATEREQUAL,        // AlphaState::CM_GEQUAL
			D3DCMP_ALWAYS               // AlphaState::CM_ALWAYS
		};

		DWORD gDX9CullOrder[2] = 
		{
			D3DCULL_CCW,                // CullState::mCCWOrder = true
			D3DCULL_CW                  // CullState::mCCWOrder = false
		};

		DWORD gDX9DepthCompare[DepthState::CM_QUANTITY] = 
		{
			D3DCMP_NEVER,               // DepthState::CM_NEVER
			D3DCMP_LESS,                // DepthState::CM_LESS
			D3DCMP_EQUAL,               // DepthState::CM_EQUAL
			D3DCMP_LESSEQUAL,           // DepthState::CM_LEQUAL
			D3DCMP_GREATER,             // DepthState::CM_GREATER
			D3DCMP_NOTEQUAL,            // DepthState::CM_NOTEQUAL
			D3DCMP_GREATEREQUAL,        // DepthState::CM_GEQUAL
			D3DCMP_ALWAYS               // DepthState::CM_ALWAYS
		};

		DWORD gDX9StencilCompare[StencilState::CM_QUANTITY] = 
		{
			D3DCMP_NEVER,               // StencilState::CF_NEVER
			D3DCMP_LESS,                // StencilState::CF_LESS
			D3DCMP_EQUAL,               // StencilState::CF_EQUAL
			D3DCMP_LESSEQUAL,           // StencilState::CF_LEQUAL
			D3DCMP_GREATER,             // StencilState::CF_GREATER
			D3DCMP_NOTEQUAL,            // StencilState::CF_NOTEQUAL
			D3DCMP_GREATEREQUAL,        // StencilState::CF_GEQUAL
			D3DCMP_ALWAYS               // StencilState::CF_ALWAYS
		};

		DWORD gDX9StencilOperation[StencilState::OT_QUANTITY] =
		{
			D3DSTENCILOP_KEEP,          // StencilState::OT_KEEP
			D3DSTENCILOP_ZERO,          // StencilState::OT_ZERO
			D3DSTENCILOP_REPLACE,       // StencilState::OT_REPLACE
			D3DSTENCILOP_INCR,          // StencilState::OT_INCREMENT
			D3DSTENCILOP_DECR,          // StencilState::OT_DECREMENT
			D3DSTENCILOP_INVERT         // StencilState::OT_INVERT
		};

		D3DPRIMITIVETYPE gDX9PrimitiveType[Visual::PT_MAX_QUANTITY] =
		{
			D3DPT_FORCE_DWORD,          // PT_NONE (not used)
			D3DPT_POINTLIST,            // PT_POLYPOINT
			D3DPT_LINELIST,             // PT_POLYSEGMENTS_DISJOINT
			D3DPT_LINESTRIP,            // PT_POLYSEGMENTS_CONTIGUOUS
			D3DPT_FORCE_DWORD,          // PT_TRIANGLES (not used)
			D3DPT_TRIANGLELIST,         // PT_TRIMESH
			D3DPT_TRIANGLESTRIP,        // PT_TRISTRIP
			D3DPT_TRIANGLEFAN           // PT_TRIFAN
		};
		*/