
#include "stdafx.h"
#pragma hdrstop

#include "NtVertexBuffer.h"

namespace NT
{
namespace _dx11
{

NT_IMPLEMENT_OBJECT(NtVertexBuffer, NtBuffer)
NT_IMPLEMENT_FACTORY(NtVertexBuffer)

NtVertexBuffer::NtVertexBuffer(int numVertices, int vertexSize, eUsage usage /* = BUFFER_STATIC */)
	: NtBuffer(numVertices, vertexSize, usage)
{

}

NtVertexBuffer::~NtVertexBuffer()
{

}

NtVertexBuffer::NtVertexBuffer()
{

}

} // namespace _dx11
} // namespace NT
