#pragma once

namespace nt {  namespace renderer {

class NtGeometryGenerator
{
public:
	struct gVertex
	{
		gVertex() {}
		gVertex(const XMFLOAT3& p, const XMFLOAT3& n, const XMFLOAT3& t, const XMFLOAT2 uv)
			: Position(p), Normal(n), TangentU(t), TexC(uv)
		{
		}
		gVertex(ntFloat px, ntFloat py, ntFloat pz,
			ntFloat nx, ntFloat ny, ntFloat nz,
			ntFloat tx, ntFloat ty, ntFloat tz,
			ntFloat u, ntFloat v)
			: Position(px, py, pz)
			, Normal(nx, ny, nz)
			, TangentU(tx, ty, tz)
			, TexC(u, v)
		{
		}
		
		XMFLOAT3 Position;
		XMFLOAT3 Normal;
		XMFLOAT3 TangentU;
		XMFLOAT2 TexC;
	};

	struct MeshData
	{
		std::vector<NtGeometryGenerator::gVertex> Vertices;
		std::vector<ntUint> Indices;
	};


public:

	void CreateBox(ntFloat width, ntFloat height, ntFloat depth, MeshData& meshData);

	void CreateSphere(ntFloat radius, ntUint slideCount, ntUint stackCount, MeshData& meshData);

	void CreateGeosphere(ntFloat radius, ntUint numSubdivision, MeshData& meshData);

	void CreateCylinder(ntFloat bottomRadius, ntFloat topRadius, ntFloat height, ntUint sliceCount, ntUint stackCount, MeshData& meshData);

	void CreateGrid(ntFloat width, ntFloat depth, ntUint m, ntUint n, MeshData& meshData);

	void CreateFullScreenQuad(MeshData& meshData);

private:
    void BuildCylinderTopCap(ntFloat bottomRaius, ntFloat topRadius, ntFloat height, ntUint sliceCount, ntUint stackCount, MeshData& meshData);
    void BuildCylinderBottomCap(ntFloat bottomRaius, ntFloat topRadius, ntFloat height, ntUint sliceCount, ntUint stackCount, MeshData& meshData);
};

}}
