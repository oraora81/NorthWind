#pragma once

namespace nt {  namespace renderer {

class NtGeometryGenerator
{
public:
    struct gVertexBase
    {
        gVertexBase() {}

        gVertexBase(const XMFLOAT3& p)
            : Position(p)
        {

        }

        gVertexBase(float nx, float ny, float nz)
            : Position(nx, ny, nz)
        {

        }

        XMFLOAT3 Position;
    };

	struct gVertex : public gVertexBase
	{
		gVertex() : gVertexBase() {}
		gVertex(const XMFLOAT3& p, const XMFLOAT3& n, const XMFLOAT3& t, const XMFLOAT2 uv)
			: gVertexBase(p), Normal(n), TangentU(t), TexC(uv)
		{

		}
		gVertex(float px, float py, float pz,
			float nx, float ny, float nz,
			float tx, float ty, float tz,
			float u, float v)
			: gVertexBase(px, py, pz)
			, Normal(nx, ny, nz)
			, TangentU(tx, ty, tz)
			, TexC(u, v)
		{
		}
		
		XMFLOAT3 Normal;
		XMFLOAT3 TangentU;
		XMFLOAT2 TexC;
	};

    struct gVertexClr : public gVertexBase
    {
        gVertexClr() : gVertexBase() {}
        gVertexClr(const XMFLOAT3 &p, const XMCOLOR& clr)
            : gVertexBase(p)
            , Color(clr)
        {

        }
        gVertexClr(float px, float py, float pz, const XMCOLOR& clr)
            : gVertexBase(px, py, pz)
            , Color(clr)
        {

        }

        XMCOLOR Color;
    };

	struct MeshData
	{
		std::vector<NtGeometryGenerator::gVertex> Vertices;
		std::vector<ntUint> Indices;
	};

    struct MeshDataClr
    {
        std::vector<NtGeometryGenerator::gVertexClr> Vertices;
        std::vector<ntUint> Indices;
    };

public:

	void CreateBox(float width, float height, float depth, MeshData& meshData);

	void CreateSphere(float radius, ntUint slideCount, ntUint stackCount, MeshData& meshData);

	void CreateGeosphere(float radius, ntUint numSubdivision, MeshData& meshData);

	void CreateCylinder(float bottomRadius, float topRadius, float height, ntUint sliceCount, ntUint stackCount, MeshData& meshData);

	void CreateGrid(float width, float depth, ntUint m, ntUint n, MeshData& meshData);

	void CreateFullScreenQuad(MeshDataClr& meshData, const XMCOLOR& clr);

private:
	void BuildCylinderTopCap(float bottomRaius, float topRadius, float height, ntUint sliceCount, ntUint stackCount, MeshData& meshData);
	
	void BuildCylinderBottomCap(float bottomRaius, float topRadius, float height, ntUint sliceCount, ntUint stackCount, MeshData& meshData);

	void SubDivide(MeshData& meshData);
};

}}
