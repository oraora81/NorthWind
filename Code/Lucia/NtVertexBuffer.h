
#pragma once

#include "NtBuffer.h"

namespace NT
{
namespace _dx11
{

class NtVertexBuffer : public NtBuffer
{
	NT_DECLARE_OBJECT

public:
	NtVertexBuffer(int numVertices, int vertexSize, eUsage usage = BUFFER_STATIC);
	virtual ~NtVertexBuffer();

protected:
	explicit NtVertexBuffer();
};


NT_REGISTER_FACTORY(NtVertexBuffer)

}	// namespace _dx11
}	// namespace NT
