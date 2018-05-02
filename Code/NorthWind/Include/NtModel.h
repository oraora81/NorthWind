
#pragma once

#include "NtVertexFormat.h"

namespace nt {

class NtModelParser;

namespace renderer {

class NtRenderer;
class NtColorShader;
class NtLightShader;
class NtTextureShader;
class NtLight;

class NtModel
{
public:
	NtModel();
	virtual ~NtModel();

    bool InitializeModelData(void* vertices, ntInt vtxSize, ntInt vtxCount, ntUint* indices, ntInt indexCount);

	void Release();

	virtual void Update(float deltaTime);

	virtual void Render(XMMATRIX& worldViewProj);

	ntInt GetVertexCount() const;

	bool SetFormatType(const ntWchar* usage, const ntWchar* type);

	ntInt GetSumOffset();

	void SetVertexInfo(ntFloat* vtxInfo);

    float& Theta()
    {
        return m_theta;
    }

    float& Phi()
    {
        return m_phi;
    }

    float Radius()
    {
        return m_radius;
    }

    void Phi(float p)
    {
        m_phi = p;
    }

    void Radius(float r)
    {
        m_radius = r;
    }

protected:
	ID3D11Buffer* MakeVertexBuffer(void* vertices, ntInt vtxSize, ntIndex vtxCount, BufferUsage usage, eCpuAccessFlag cpuFlag);

	ID3D11Buffer* MakeIndexBuffer(ntUint* indices, ntInt indexCount);

private:
	bool InitializeAse(const ntWchar* puppetName);

	void ReleaseBuffer();
	
protected:
	ID3D11Buffer* m_vertexBuffer;
	ID3D11Buffer* m_indexBuffer;
	ntUint		m_texhandle;
	ntInt		m_vertexCount;
	ntInt		m_indexCount;

	NtVertexFormat m_vertexFormat;
	ntFloat*	m_puppetRawData;

	typedef std::vector<NtMeshObj>	MESH_VECTOR;
	MESH_VECTOR m_meshVector;

    XMFLOAT3 m_eyePosW;

    float m_theta;
    float m_phi;
    float m_radius;
};


}	// namespace renderer
}	// namespace nt
