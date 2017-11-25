
#pragma once

#include "NtVertexFormat.h"

namespace nt {

class NtModelParser;

namespace renderer {

class NtRenderer;
class NtTexture;
class NtColorShader;
class NtLightShader;
class NtTextureShader;
class NtLight;

class NtModel
{
public:
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

	NtModel();
	virtual ~NtModel();

    bool InitializeModelData(void* vertices, ntInt vtxSize, ntInt vtxCount, ntUint* indices, ntInt indexCount, const ntWchar* fx);

	void Release();

	virtual void Update(float deltaTime);

	virtual void Render(XMMATRIX& worldViewProj);

	ntInt GetVertexCount() const;

	bool SetFormatType(const ntWchar* usage, const ntWchar* type);

	ntInt GetSumOffset();

	void SetVertexInfo(ntFloat* vtxInfo);

	void SetColorShader(NtColorShader* shader);

	void SetTextureShader(NtTextureShader* shader);

	void SetLightShader(NtLightShader* shader);

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

	std::shared_ptr<NtColorShader> m_colorShader;
	NtTextureShader* m_textureShader;
	NtLightShader* m_lightShader;

	typedef std::vector<NtMeshObj>	MESH_VECTOR;
	MESH_VECTOR m_meshVector;
};


}	// namespace renderer
}	// namespace nt
