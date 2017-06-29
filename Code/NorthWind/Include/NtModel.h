
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

	NtModel();
	~NtModel();

	bool Initialize(const ntWchar* puppetName);

	bool IntializeModelData(NtPCVertex* vertices, ntInt vertexCount, ntUint* indices, ntInt indexCount);

	void Release();

	void Render(const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& proj, const NtLight* light);

	int GetIndexCount();

	NtTexture* GetTexture();

	// get , set
	void	SetVertexCount(ntInt count);
	ntInt	GetVertexCount() const;
	bool	SetFormatType(const ntWchar* usage, const ntWchar* type);
	ntInt	GetSumOffset();
	void	SetVertexInfo(ntFloat* vtxInfo);

	void	SetColorShader(NtColorShader* shader);
	void	SetTextureShader(NtTextureShader* shader);
	void	SetLightShader(NtLightShader* shader);

private:
	bool InitializeAse(const ntWchar* puppetName);
	void ReleaseBuffer();
	
private:
	ID3D11Buffer* m_vertexBuffer;
	ID3D11Buffer* m_indexBuffer;
	ntUint		m_texhandle;
	ntInt		m_vertexCount;
	ntInt		m_indexCount;

	NtVertexFormat m_vertexFormat;
	ntFloat*	m_puppetRawData;

	NtColorShader* m_colorShader;
	NtTextureShader* m_textureShader;
	NtLightShader* m_lightShader;

	typedef std::vector<NtMeshObj>	MESH_VECTOR;

	MESH_VECTOR m_meshVector;
};


}	// namespace renderer
}	// namespace nt
