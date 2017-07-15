#include "NtCoreLib.h"
#pragma hdrstop

#include "NtGeometryGenerator.h"

void NtGeometryGenerator::CreateGrid(ntFloat width, ntFloat depth, ntUint m, ntUint n)
{
    ntUint vertexCount = m * n;
    ntUint faceCount = (m - 1) * (n - 1) * 2;

    // making vertices
    ntFloat dx = width / (n - 1);
    ntFloat dz = depth / (m - 1);

    ntFloat du = 1.0f / (n - 1);
    ntFloat dv = 1.0f / (m - 1);
}