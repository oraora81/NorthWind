
#include "NtCoreLib.h"
#pragma hdrstop

#include "NtPuppet.h"

#include "NtRenderer.h"
#include "NtTexture.h"
#include "NtAse.h"
#include "NtColorShader.h"
#include "NtTextureShader.h"
#include "NtLightShader.h"
#include "NtLight.h"

namespace nt { namespace renderer {

NtPuppet::NtPuppet()
	: m_vertexBuffer(nullptr)
	, m_indexBuffer(nullptr)
	, m_vertexCount(0)
	, m_indexCount(0)
	, m_puppetRawData(nullptr)
{
	m_colorShader = nullptr;
	m_textureShader = nullptr;
	m_lightShader = nullptr;
}

NtPuppet::~NtPuppet()
{

}

bool NtPuppet::Initialize(ntWchar* puppetName)
{
	// load the puppet Data from txt file.
	bool res = InitializeAse(puppetName);
	if (false == res)
	{
		return false;
	}

	// init the vertex and index buffer that hold the geometry for the triangle.
	//res = InitializeBuffer(renderer);
	//if (false == res)
	//{
	//	return false;
	//}

	return true;
}


void NtPuppet::Release()
{
	ReleaseBuffer();
}


void NtPuppet::Render(const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& proj, const NtLight* light)
{
	ntUint stride = sizeof(NtTexVertex);
	ntUint offset = 0;

	/*
	// set the vertex buffer to active in the input assembler so it can be rendered.
	renderer->SetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// set the index buffer to active in the input assembler so it can be rendered.
	renderer->SetIndexBuffers(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	renderer->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	

	//m_lightShader->Render(renderer, m_indexCount, world, view, proj, GetTexture(), light->GetDirection(), light->GetDiffuse());
	m_textureShader->Render(renderer->GetD3DRenderer(), 3, world, view, proj, m_texhandle);
	*/
	


	// draw Mesh

	// set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	//renderer->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	g_renderer->DeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	ntSize size = (ntSize)m_meshVector.size();
	for (ntSize i = 0; i < size; ++i)
	{
		NtMeshObj& mesh = m_meshVector[i];
		g_renderInterface->SetVertexBuffers(0, 1, &mesh.vtxBuffer, &stride, &offset);
		g_renderInterface->SetIndexBuffers(mesh.idxBuffer, DXGI_FORMAT_R32_UINT, 0);

		//bool res = m_colorShader->Render(renderer->GetD3DRenderer(), mesh.idxCount, world, view, proj);
		//if (res == false)
		//{
		//	return;
		//}


		bool res = m_textureShader->Render(mesh.idxCount, world, view, proj, mesh.texHandle);
		if (res == false)
		{
			return;
		}
	}
}

int NtPuppet::GetIndexCount()
{
	return m_indexCount;
}


bool NtPuppet::InitializeAse(ntWchar* puppetName)
{
	// load the raw data from ase file
	NtAse puppetLoader(puppetName);
	bool res = puppetLoader.Parse();
	if (false == res)
	{
		return false;
	}

	// 삼각형 셋팅

	/*
	NtTexVertex vt[3];
	ntUint indices[3];

	vt[0].pos = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);  // Bottom left.
	vt[0].uv = D3DXVECTOR2(0.0f, 1.0f);
	vt[1].pos = D3DXVECTOR3(0.0f, 1.0f, 0.0f);  // Top middle.
	vt[1].uv = D3DXVECTOR2(0.5f, 0.0f);
	vt[2].pos = D3DXVECTOR3(1.0f, -1.0f, 0.0f);  // Bottom right.
	vt[2].uv = D3DXVECTOR2(1.0f, 1.0f);

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;


	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.ByteWidth = sizeof(vt);
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = vt;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// now create the vertex buffer
	if (!renderer->CreateBuffer(bufferDesc, &vertexData, &m_vertexBuffer))
	{
		return false;
	}

	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(indices);
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// give the subresource structure a pointer to the index data
	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// create the index buffer
	if (!renderer->CreateBuffer(indexBufferDesc, &indexData, &m_indexBuffer))
	{
		return false;
	}

	m_texhandle = g_resManager->LoadTexture(L"test_cube.jpg");
	*/





	// 모델 ASE 로딩

	const std::map<NtString, NtMesh*> meshMap =	puppetLoader.GetMeshMap();
	for (auto itor = meshMap.begin(); itor != meshMap.end(); ++itor)
	{
		const NtMesh& mesh = *(*itor).second;

		NTRACE(L"name : %s, vtx : %d, idx : %d\n", mesh.m_name.Buffer(), mesh.m_vertexNum, mesh.m_faceNum);

		if (mesh.m_vertex == nullptr)
		{
			continue;
		}

		// Bone 메시는 아직 그리지 않는다.
		if (Crt::StrStr(const_cast<ntWchar*>(mesh.m_name.Buffer()), L"Bone") != NULL)
		{
			continue;
		}

		if (Crt::StrStr(const_cast<ntWchar*>(mesh.m_name.Buffer()), L"Bip") != NULL)
		{
			continue;
		}

		D3D11_BUFFER_DESC bufferDesc;
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.ByteWidth = sizeof(NtTexVertex) * mesh.m_vertexNum;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = 0;

		NtTexVertex* vt = new NtTexVertex[mesh.m_vertexNum];
		ntUint* indices = new ntUint[mesh.m_faceNum * 3];

		Crt::MemSet(vt, sizeof(NtTexVertex) * mesh.m_vertexNum);

		// Set Position and Normal
		for (ntUint i = 0; i < mesh.m_vertexNum; ++i)
		{
			vt[i].pos.x = mesh.m_vertex[i].m_p.x;
			vt[i].pos.y = mesh.m_vertex[i].m_p.y;
			vt[i].pos.z = mesh.m_vertex[i].m_p.z;

			//vt[i].normal.x = mesh.m_vertex[index].m_n.x;
			//vt[i].normal.y = mesh.m_vertex[index].m_n.y;
			//vt[i].normal.z = mesh.m_vertex[index].m_n.z;
			//vt[i].color = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);
		}

		// 삼각형 개수만큼 루프
		// Set Texture Coord
		for (ntUint i = 0; i < mesh.m_faceNum; ++i)
		{
			const NtTris& uvFace = mesh.m_rawTFace[i];
			const NtTris& vtFace = mesh.m_rawIndex[i];
			// 세 개의 점으로 구성된다.
			// 각각의 점은 하나의 UV좌표를 갖는다.

			// 점 a가 갖는 uv좌표
			if (mesh.m_tv != nullptr)
			{
				vt[vtFace.a].uv.x = mesh.m_tv[uvFace.a].tu;
				vt[vtFace.a].uv.y = mesh.m_tv[uvFace.a].tv;

				// 점 b가 갖는 uv좌표
				vt[vtFace.b].uv.x = mesh.m_tv[uvFace.b].tu;
				vt[vtFace.b].uv.y = mesh.m_tv[uvFace.b].tv;

				// 점 c가 갖는 uv좌표
				vt[vtFace.c].uv.x = mesh.m_tv[uvFace.c].tu;
				vt[vtFace.c].uv.y = mesh.m_tv[uvFace.c].tv;
			}
		}



		for (ntUint i = 0; i < mesh.m_faceNum; ++i)
		{
			indices[i*3] = mesh.m_rawIndex[i].a;
			indices[i*3+1] = mesh.m_rawIndex[i].b;
			indices[i*3+2] = mesh.m_rawIndex[i].c;
		}

		// give the subresource structure a pointer to the vertex data
		D3D11_SUBRESOURCE_DATA vertexData;
		vertexData.pSysMem = vt;
		vertexData.SysMemPitch = 0;
		vertexData.SysMemSlicePitch = 0;

		ID3D11Buffer* vertexBuffer = NULL;
		// now create the vertex buffer

		NtRenderBufferParam param;
		param.m_desc = &bufferDesc;
		param.m_resData = &vertexData;
		param.m_buffer = &vertexBuffer;
		if (!g_renderInterface->CreateBuffer(param))
		{
			return false;
		}

		D3D11_BUFFER_DESC indexBufferDesc;
		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		indexBufferDesc.ByteWidth = sizeof(NtTris) * mesh.m_faceNum;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.CPUAccessFlags = 0;
		indexBufferDesc.MiscFlags = 0;
		indexBufferDesc.StructureByteStride = 0;

		// give the subresource structure a pointer to the index data
		D3D11_SUBRESOURCE_DATA indexData;
		indexData.pSysMem = indices;
		indexData.SysMemPitch = 0;
		indexData.SysMemSlicePitch = 0;

		// create the index buffer
		ID3D11Buffer* indexBuffer = NULL;

		param.m_desc = &indexBufferDesc;
		param.m_resData = &indexData;
		param.m_buffer = &indexBuffer;
		if (!g_renderInterface->CreateBuffer(param))
		{
			return false;
		}

		NtMeshObj drawInfo;
		drawInfo.desc = bufferDesc;
		drawInfo.idxBuffer = indexBuffer;
		drawInfo.vtxBuffer = vertexBuffer;
		drawInfo.idxCount = mesh.m_faceNum * 3;
		if (mesh.m_rawIndex != nullptr)
		{
			drawInfo.texHandle = mesh.m_rawIndex[0].m_texHandle;
		}

		m_meshVector.push_back(drawInfo);

		SAFE_DELETE_ARRAY(vt);
		SAFE_DELETE_ARRAY(indices);
	}

	return true;
}


void NtPuppet::ReleaseBuffer()
{
	SAFE_RELEASE(m_indexBuffer);
	SAFE_RELEASE(m_vertexBuffer);
}


void NtPuppet::SetVertexCount( ntInt count )
{
	NtAsserte(count > 0);

	m_vertexCount = count;
}


ntInt NtPuppet::GetVertexCount() const
{
	return m_vertexCount;
}

bool NtPuppet::SetFormatType( const ntWchar* usage, const ntWchar* type )
{
	return m_vertexFormat.SetFormatType(usage, type);
}

ntInt NtPuppet::GetSumOffset()
{
	return m_vertexFormat.GetSumOffset();
}

void NtPuppet::SetVertexInfo( ntFloat* vtxInfo )
{
	NtAsserte(vtxInfo != NULL);

	m_puppetRawData = vtxInfo;
}


void NtPuppet::SetColorShader(NtColorShader* shader)
{
	m_colorShader = shader;
}

void NtPuppet::SetTextureShader(NtTextureShader* shader)
{
	m_textureShader = shader;
}

void NtPuppet::SetLightShader(NtLightShader* shader)
{
	m_lightShader = shader;
}


}	// namespace renderer
}	// namespace nt
