
#pragma once

#include "NtVertexFormat.h"

namespace NT
{
class NtModelParser;

namespace RENDERER
{
class NtRenderer;
class NtTexture;
class NtColorShader;
class NtLightShader;
class NtTextureShader;
class NtLight;

class NtPuppet
{
	struct NtVertexType
	{
		XMFLOAT3A position;
		XMFLOAT2A texture;
		XMFLOAT3A normal;
	};

public:

	NtPuppet();
	~NtPuppet();

	bool Initialize(NtWChar* puppetName);
	void Release();
	void Render(const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& proj, const NtLight* light);

	int GetIndexCount();

	NtTexture* GetTexture();

	// get , set
	void	SetVertexCount(NtInt count);
	NtInt	GetVertexCount() const;
	bool	SetFormatType(const NtWChar* usage, const NtWChar* type);
	NtInt	GetSumOffset();
	void	SetVertexInfo(NtFloat* vtxInfo);

	void	SetColorShader(NtColorShader* shader);
	void	SetTextureShader(NtTextureShader* shader);
	void	SetLightShader(NtLightShader* shader);

private:
	bool InitializeAse(NtWChar* puppetName);
	void ReleaseBuffer();
	
private:
	ID3D11Buffer* m_vertexBuffer;
	ID3D11Buffer* m_indexBuffer;
	NtUInt		m_texhandle;
	NtInt		m_vertexCount;
	NtInt		m_indexCount;

	NtVertexFormat m_vertexFormat;
	NtFloat*	m_puppetRawData;

	NtColorShader* m_colorShader;
	NtTextureShader* m_textureShader;
	NtLightShader* m_lightShader;

	typedef std::vector<NtMeshObj>	MESH_VECTOR;

	MESH_VECTOR m_meshVector;
};


}	// namespace RENDERER
}	// namespace NT
