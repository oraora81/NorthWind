
#pragma once

namespace NT
{

#define NT_2D_VERTEX_Z (1e-3f)
#define NT_RGBA(r,g,b,a) \
	(NtULong)(((NtUChar)(r))|((NtUChar)(g)<<8)|((NtUChar)(b)<<16)|((NtUChar)(a)<<24))

const NtUInt INVALID_TEXTURE_HANDLE = 0xffffffff;

struct NtColorVertex
{
	XMFLOAT3A pos;
	XMFLOAT4A color;
};

struct NtTexVertex
{
	XMFLOAT3A pos;
	XMFLOAT2A uv;
};

struct NtPos
{
	NtFloat x;
	NtFloat y;
	NtFloat z;
};

struct NtTris
{
	NtUInt a;
	NtUInt b;
	NtUInt c;
};

struct NtFace : public NtTris
{
	NtUInt	m_subMaterialID;
	NtUInt	m_texHandle;
	NtULong	m_diffuse;
	NtInt	m_texType;
	NtInt	m_drawFlags;
};

struct NtNormal
{
	NtFloat x;
	NtFloat y;
	NtFloat z;
};

struct NtUV
{
	NtFloat tu;
	NtFloat tv;
};


struct NtMesh;
struct NtRigid
{
	NtString m_objName;
	NtFloat m_blendWeight;
	NtSize	m_rigidSize;
	NtMesh*	m_meshObj;

	NtRigid() : m_blendWeight(0.0f)
		, m_rigidSize(0)
		, m_meshObj(nullptr)
	{

	}

	NtRigid(const NtRigid& r)
	{
		*this = r;
	}

	NtRigid& operator =(const NtRigid& r)
	{
		m_objName = r.m_objName;
		m_blendWeight = r.m_blendWeight;
		m_rigidSize = r.m_rigidSize;
		m_meshObj = r.m_meshObj;

		return *this;
	}
};


struct NtVertex
{
	NtPos		m_p;
	NtNormal	m_n;
	NtSize		m_rigidNum;
	NtRigid*	m_rigid;
	NtFloat		m_weight;
	NtString	m_objName;

	~NtVertex()
	{
		SAFE_DELETE_ARRAY(m_rigid);
	}
};


struct NtMesh
{
	NtString m_name;
	NtString m_parentName;
	NtSize	m_vertexNum;
	NtSize	m_faceNum;
	NtSize m_tvertexNum;

	NtVertex* m_vertex;
	NtUV*	m_tv;
	NtFace* m_rawIndex;
	NtTris* m_rawTFace;
	
	NtMesh*	m_linkParent;
	NtMesh* m_linkChild;

	NtFloat rot[9];
	NtFloat pos[3];
	NtFloat scl [3];

	NtMesh()
	{
		m_vertexNum = 0;
		m_faceNum = 0;
		m_tvertexNum = 0;

		m_vertex = nullptr;
		m_rawTFace = nullptr;
		m_tv	   = nullptr;
		m_rawIndex = nullptr;
		
		m_linkParent = nullptr;
		m_linkChild = nullptr;

		Crt::MemSet(rot, sizeof(rot));
		Crt::MemSet(pos, sizeof(pos));
		Crt::MemSet(scl, sizeof(scl));
	}

	~NtMesh()
	{
		SAFE_DELETE_ARRAY(m_vertex);

		SAFE_DELETE_ARRAY(m_rawIndex);

		SAFE_DELETE_ARRAY(m_rawTFace);

		SAFE_DELETE_ARRAY(m_tv);
	}
};


struct NtMaterial
{
	enum eMaterialClass
	{
		MC_STANDARD,
		MC_MULTISUB,
		MC_DIRECTX_SHADER,
		MAX_MC,
	};

	NtString m_materialName;
	NtString m_bitmapName;
	eMaterialClass m_materialClass;
	NtULong m_diffuse;
	NtFloat	m_specular[3];
	NtUInt m_texHandle;
	NtInt m_texType;
	NtFloat m_scaleU;
	NtFloat m_scaleV;
	NtUInt	m_drawFlag;
	NtInt m_subMaterialNum;
	NtMaterial* m_subMaterial;

	NtMaterial()
		: m_materialName("")
		, m_materialClass(MC_STANDARD)
		, m_diffuse(0)
		, m_texHandle(0)
		, m_texType(0)
		, m_scaleU(1.0f)
		, m_scaleV(1.0f)
		, m_drawFlag(0)
		, m_subMaterialNum(0)
		, m_subMaterial(nullptr)
	{

	}

	~NtMaterial()
	{
		SAFE_DELETE_ARRAY(m_subMaterial);
	}
};


/**
 *	@brief		NtMeshObj :
 */

#ifdef _NT_DX11
struct NtMeshObj
{
	NtVertexBuffer* vtxBuffer;
	NtIndexBuffer* idxBuffer;
	NtInt		  idxCount;
	D3D11_BUFFER_DESC desc;
	NtUInt		texHandle;
};
#endif


/**
 *	@brief		sDDSInitHeader :
 */

struct sDDSInitHeader
{
	NtUInt m_width;
	NtUInt m_height;
	NtUInt m_depth;
	NtUInt m_mipCount;
	NtUInt m_arraySize;
	NtUInt m_bitSize;
	NtUInt m_maxSize;
	const NtUChar* m_bitData;
	DXGI_FORMAT m_format;
};


/**
 *	@brief		NtPoint :
 */

struct NtPoint
{
	NtPoint();
	NtPoint(NtFloat pX, NtFloat pY);
	NtPoint(const NtPoint& point);

	NtPoint& operator=(const NtPoint& point);
	NtPoint& operator += (const NtPoint& point);
	NtPoint& operator -= (const NtPoint& point);
	NtPoint& operator *= (const NtPoint& point);
	NtPoint& operator /= (const NtPoint& point);
	NtPoint& operator *= (NtFloat s);

	const NtPoint operator+(const NtPoint& point);
	const NtPoint operator-(const NtPoint& point);
	const NtPoint operator*(const NtPoint& point);
	const NtPoint operator/(const NtPoint& point);
	const NtPoint operator-();

	bool operator==(const NtPoint& point);
	bool operator!=(const NtPoint& point);

	NtFloat x, y;
};


// for demonstrate screen coordinate
struct NtPosition
{
	NtInt	x;
	NtInt	y;
};


#include "NtRenderType.inl"

}
