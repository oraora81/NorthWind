#include "NtCoreLib.h"
#pragma hdrstop

XMMATRIX NtD3dUtil::InverseTranspose(CXMMATRIX M)
{
    // Inverse-transpose is just applied to normals.
    // So zero out translation row so that it doesn't
    // get into out inverse-transpose calculation--we
    // don't want the inverse-transpose of the translation.
    XMMATRIX A = M;
    A.r[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

    XMVECTOR det = XMMatrixDeterminant(A);
    return XMMatrixTranspose(XMMatrixInverse(&det, A));
}
