
#pragma once

namespace nt {

#define NT_2D_VERTEX_Z (1e-3f)
#define NT_RGBA(r,g,b,a) \
	(ntUlong)(((ntUchar)(r))|((ntUchar)(g)<<8)|((ntUchar)(b)<<16)|((ntUchar)(a)<<24))

const ntUint INVALID_TEXTURE_HANDLE = 0xffffffff;

namespace Vertex 
{
    struct NtPCVertex
    {
        XMFLOAT3 position;
        XMFLOAT4 color;
    };

    struct NtLVertex
    {
        XMFLOAT3 position;
        XMFLOAT3 normal;
    };
}


#pragma region Regacy
struct NtTexVertex
{
    XMFLOAT3 pos;
    XMFLOAT2 uv;
};

struct NtPos
{
    ntFloat x;
    ntFloat y;
    ntFloat z;
};

struct NtTris
{
    ntUint a;
    ntUint b;
    ntUint c;
};

struct NtFace : public NtTris
{
    ntUint	m_subMaterialID;
    ntUint	m_texHandle;
    ntUlong	m_diffuse;
    ntInt	m_texType;
    ntInt	m_drawFlags;
};

struct NtNormal
{
    ntFloat x;
    ntFloat y;
    ntFloat z;
};

struct NtUV
{
    ntFloat tu;
    ntFloat tv;
};


struct NtMesh;
struct NtRigid
{
    NtString m_objName;
    ntFloat m_blendWeight;
    ntSize	m_rigidSize;
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
    ntSize		m_rigidNum;
    NtRigid*	m_rigid;
    ntFloat		m_weight;
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
    ntSize	m_vertexNum;
    ntSize	m_faceNum;
    ntSize m_tvertexNum;

    NtVertex* m_vertex;
    NtUV*	m_tv;
    NtFace* m_rawIndex;
    NtTris* m_rawTFace;

    NtMesh*	m_linkParent;
    NtMesh* m_linkChild;

    ntFloat rot[9];
    ntFloat pos[3];
    ntFloat scl[3];

    NtMesh()
    {
        m_vertexNum = 0;
        m_faceNum = 0;
        m_tvertexNum = 0;

        m_vertex = nullptr;
        m_rawTFace = nullptr;
        m_tv = nullptr;
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
    ntUlong m_diffuse;
    ntFloat	m_specular[3];
    ntUint m_texHandle;
    ntInt m_texType;
    ntFloat m_scaleU;
    ntFloat m_scaleV;
    ntUint	m_drawFlag;
    ntInt m_subMaterialNum;
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

struct NtMeshObj
{
    NtVertexBuffer* vtxBuffer;
    NtIndexBuffer* idxBuffer;
    ntInt		  idxCount;
    D3D11_BUFFER_DESC desc;
    ntUint		texHandle;
};

#pragma endregion


struct sDDSInitHeader
{
	ntUint m_width;
	ntUint m_height;
	ntUint m_depth;
	ntUint m_mipCount;
	ntUint m_arraySize;
	ntUint m_bitSize;
	ntUint m_maxSize;
	const ntUchar* m_bitData;
	DXGI_FORMAT m_format;
};



struct NtPoint
{
	NtPoint();
	NtPoint(ntFloat pX, ntFloat pY);
	NtPoint(const NtPoint& point);

	NtPoint& operator=(const NtPoint& point);
	NtPoint& operator += (const NtPoint& point);
	NtPoint& operator -= (const NtPoint& point);
	NtPoint& operator *= (const NtPoint& point);
	NtPoint& operator /= (const NtPoint& point);
	NtPoint& operator *= (ntFloat s);

	const NtPoint operator+(const NtPoint& point);
	const NtPoint operator-(const NtPoint& point);
	const NtPoint operator*(const NtPoint& point);
	const NtPoint operator/(const NtPoint& point);
	const NtPoint operator-();

	bool operator==(const NtPoint& point);
	bool operator!=(const NtPoint& point);

	ntFloat x, y;
};


// for demonstrate screen coordinate
struct NtPosition
{
	ntInt	x;
	ntInt	y;
};


#include "NtRenderType.inl"

}

#include "NtLightType.h"