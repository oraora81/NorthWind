#pragma once

namespace nt {  namespace renderer

class NtGeometryGenerator
{
public:
    struct Vertex
    {
        Vertex() {}
        Vertex(const XMFLOAT3& p, const XMFLOAT3& n, const XMFLOAT3& t, const XMFLOAT2 uv)
            : Position(p), Normal(n), TangentU(t), TexC(uv)
        {
        }
        Vertex(ntFloat px, ntFloat py, ntFloat pz,
            ntFloat nx, ntFloat ny, ntFloat nz,
            ntFloat tx, ntFloat ty, ntFloat tz,
            ntFloat u, ntFloat v)
            : Position(px, py, pz),
            , Normal(nx, ny, nz)
            , TangentU(tx, ty, tz)
            , TexC(u, v)
        {
        }
        
        XMFLOAT3 Position;
        XMFLOAT3 Normal;
        XMFLOAT3 TangentU;
        XMFLOAT2 TexC;
    }

    struct MeshData
    {
        NtArray<Vertex> Vertices;
        NtArray<ntUint> Indices;
    }

public:
    void CreateGrid(ntFloat width, ntFloat depth, ntUint m, ntUint n);
}

}}