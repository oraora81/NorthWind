

#include "NtCoreLib.h"
#pragma hdrstop

#include "NtAse.h"
#include "NtTexture.h"

namespace NT
{
namespace RENDERER
{

static const NtWChar* g_unit_materialClass[NtMaterial::MAX_MC] = 
{
	L"Standard",
	L"Multi/Sub-Object",
	L"DirectX Shader",
};

NtMaterial::eMaterialClass GetMaterialClass(const NtWChar* className)
{
	NtInt size = _countof(g_unit_materialClass);
	for (NtInt i = 0; i < size; ++i)
	{
		if (Crt::StrCmp(className, g_unit_materialClass[i]) == 0)
		{
			return (NtMaterial::eMaterialClass)i;
		}
	}

	return NtMaterial::MAX_MC;
}


NtAse::NtAse( const NtWChar* fileName )
	: NtFile()
	, m_currentMesh(nullptr)
	, m_currentMaterial(nullptr)
	, m_currentSubMaterial(nullptr)
{
	Execute(fileName);
}

NtAse::~NtAse()
{
	std::for_each(std::begin(m_materialVector), std::end(m_materialVector), Safe_Delete());
	for (auto itor = m_meshMap.begin(); itor != m_meshMap.end(); ++itor)
	{
		SAFE_DELETE((*itor).second);
	}
}

bool NtAse::Parse()
{
	if (!IsOpen())
	{
		return false;
	}

	m_mode = PARSE_KEYWORD;
	while(IsEOF() == false)
	{
		if (!ReadLine())
		{
			return false;
		}

		SetKeyword();

		if (CompareKeyword(L"*MATERIAL_LIST"))
		{
			if (false == ParseMaterial())	return false;
		}
		else if (CompareKeyword(L"*GEOMOBJECT"))
		{
			if (false == ParseGeomObject())	return false;
		}
	}

	return true;
}


const std::map<NtString, NtMesh*>& NtAse::GetMeshMap()
{
	return m_meshMap;
}


bool NtAse::FindKeyword(NtWChar* keyword)
{
	NtWChar buffer[READ_BUF_SIZE];
	Crt::MemSet(buffer, sizeof(buffer));

	NtInt res = swscanf_s(GetData(), L"%s", buffer, _countof(buffer));
	if (res == EOF)
	{
		return false;
	}

	if (Crt::StrLen(buffer) < 1)
	{
		return false;
	}

	if (Crt::StrCmp(keyword, buffer) == 0)
	{
		return true;
	}

	return false;
}


bool NtAse::ParseGeomObject()
{
	// geomobject 파싱 시작, 필요한 데이터를 검색한다.
	while (!FindKeyword(L"}"))
	{
		if (!ReadLine()) return false;

		SetKeyword();

		if (CompareKeyword(L"*NODE_NAME"))
		{
			ParseNodeName();
		}
		else if (CompareKeyword(L"*NODE_PARENT"))
		{
			ParseParentName();
		}
		else if (CompareKeyword(L"*NODE_TM"))
		{
			ParseTM();
		}
		else if (CompareKeyword(L"*MESH"))
		{
			ParseMesh();
		}
		else if (CompareKeyword(L"*MATERIAL_REF"))
		{
			ParseMaterialRef();
		}
	}

	if (m_currentMesh->m_vertex == nullptr)
	{
		m_meshMap.erase(m_currentMesh->m_name);
	}

	return true;
}


bool NtAse::ParseMaterial()
{
	while (!FindKeyword(L"}"))
	{
		ReadLine();
		SetKeyword();

		if (CompareKeyword(L"*MATERIAL_COUNT"))
		{
			NtWChar* temp[32] = {0, };
			NtInt materialCount = 0;
			NtInt res = swscanf_s(GetData(), L"%s %d", temp, _countof(temp), &materialCount);
			if (res != 2)	return false;

			if (materialCount != 0)
			{
				m_materialVector.reserve(materialCount);
				for (NtInt i = 0; i < materialCount; ++i)
				{
					NtMaterial* material = new NtMaterial;
					m_materialVector.push_back(material);
				}
			}
		}
		else if (CompareKeyword(L"*MATERIAL"))
		{
			NtWChar* temp[32] = {0, };
			NtInt number = -1;
			NtInt res = swscanf_s(GetData(), L"%s %d", temp, _countof(temp), &number);
			if (res != 2)	return false;

			NtAsserte(number != -1 && number < (NtInt)m_materialVector.size());

			m_currentMaterial = m_materialVector.at(number);
			m_currentSubMaterial = nullptr;
			// Parse detail information
			ParseMaterialElement(m_currentMaterial);
		}
	}
	
	return true;
}


void NtAse::ParseName(NtString& name)
{
	const NtWChar* s = Crt::StrChr(GetData(), L'\"');
	if (s == NULL)
	{
		OutputErrorMsg(L"Name Found to fail");
		return;
	}
	const NtWChar* e = Crt::StrChr(s + 1, L'\"');
	if (e == NULL)
	{
		OutputErrorMsg(L"Name Found to fail");
		return;
	}

	name.AssignSubString(s + 1, e - 1);
	name.Lowercase();
}


void NtAse::ParseNodeName()
{
	NtString nodeName;
	ParseName(nodeName);

	auto findRes = m_meshMap.find(nodeName);
	if (findRes != m_meshMap.end())
	{
		OutputErrorMsg(L"Name Found to fail");
		return;
	}

	NtMesh* mesh = new NtMesh;
	m_meshMap.insert(std::make_pair(nodeName, mesh));


	auto res = m_meshMap.find(nodeName);
	if (res != m_meshMap.end())
	{
		m_currentMesh = (*res).second;
		m_currentMesh->m_name = nodeName;
	}
}


void NtAse::ParseParentName()
{
	ParseName(m_currentMesh->m_parentName);
}


void NtAse::ParseTM()
{
	XMFLOAT3A v[4];
	NtWChar name[32] = {0, };
	float a, b, c;

	do 
	{
		ReadLine();
		SetKeyword();

		if (m_keyword == L"*TM_ROW0")
		{
			NtInt res = swscanf_s(GetData(), L"%s %f %f %f",name, _countof(name), &a, &b, &c);
			if (res != 4)	return;
			v[0] = XMFLOAT3A(a, b, c);
		}
		else if (m_keyword == L"*TM_ROW1")
		{
			NtInt res = swscanf_s(GetData(), L"%s %f %f %f",name, _countof(name), &a, &b, &c);
			if (res != 4)	return;
			v[1] = XMFLOAT3A(a, b, c);
		}
		else if (m_keyword == L"*TM_ROW2")
		{
			NtInt res = swscanf_s(GetData(), L"%s %f %f %f",name, _countof(name), &a, &b, &c);
			if (res != 4)	return;
			v[2] = XMFLOAT3A(a, b, c);
		}
		else if (m_keyword == L"*TM_ROW3")
		{
			NtInt res = swscanf_s(GetData(), L"%s %f %f %f",name, _countof(name), &a, &b, &c);
			if (res != 4)	return;
			v[3] = XMFLOAT3A(a, b, c);
		}
	} while (!FindKeyword(L"}"));

	ClearData();
}


void NtAse::ParseMesh()
{
	NtWChar name[32] = {0, };

	do 
	{
		ReadLine();
		SetKeyword();

		if (CompareKeyword(L"*MESH_NUMVERTEX"))
		{
			NtInt count = 0;
			NtInt res = swscanf_s(GetData(), L"%s %d", name, _countof(name), &count);
			if (res != 2)	return;
			m_currentMesh->m_vertexNum = count;

			if (count != 0)
			{
				m_currentMesh->m_vertex = new NtVertex[count];
			}
		}
		else if (CompareKeyword(L"*MESH_NUMFACES"))
		{
			NtInt count = 0;
			NtInt res = swscanf_s(GetData(), L"%s %d", name, _countof(name), &count);
			if (res != 2)	return;
			m_currentMesh->m_faceNum = count;

			if (count != 0)
			{
				m_currentMesh->m_rawIndex = new NtFace[count];
				m_currentMesh->m_rawTFace = new NtTris[count];

				Crt::MemSet(m_currentMesh->m_rawIndex, sizeof(NtFace) * count);
				Crt::MemSet(m_currentMesh->m_rawTFace, sizeof(NtTris) * count);
			}
		}
		else if (CompareKeyword(L"*MESH_VERTEX_LIST"))
		{
			ParseVertex();
		}
		else if (CompareKeyword(L"*MESH_FACE_LIST"))
		{
			ParseFace();
		}
		else if (CompareKeyword(L"*MESH_NORMALS"))
		{
			ParseNormal();
		}
		else if (CompareKeyword(L"*MESH_TVERTLIST"))
		{
			ParseTexVertex();
		}
		else if (CompareKeyword(L"*MESH_TFACELIST"))
		{
			ParseTexFace();
		}
		else if (CompareKeyword(L"*MESH_NUMTVERTEX"))
		{
			NtInt count = 0;
			NtInt res = swscanf_s(GetData(), L"%s %d", name, _countof(name), &count);
			if (res != 2)	return;
			m_currentMesh->m_tvertexNum = count;
			if (count != 0)
			{
				m_currentMesh->m_tv = new NtUV[count];
				Crt::MemSet(m_currentMesh->m_tv, sizeof(NtUV) * count);
			}
		}
		else if (CompareKeyword(L"*MESH_PHYSIQUE_VERTEX_LIST"))
		{
			ParsePhysiqueVertex();
		}
		
	} while (!FindKeyword(L"}"));

	ClearData();
}


void NtAse::ParseMaterialRef()
{
	NtWChar temp[32] = {0, };
	NtInt index;

	NtInt res = swscanf_s(GetData(), L"%s %d", temp, _countof(temp), &index);
	if (res != 2)	return;

	if (index >= (NtInt)m_materialVector.size())
	{
		OutputErrorMsg(L"Invalid Material Vector access");
		return;
	}

	for (NtSize i = 0; i < m_currentMesh->m_faceNum; ++i)
	{
		NtFace& face = m_currentMesh->m_rawIndex[i];

		const NtMaterial* material = nullptr;
		
		if (m_materialVector[index]->m_subMaterial)
		{
			int subNum = m_currentMesh->m_rawIndex[i].m_subMaterialID % m_materialVector[index]->m_subMaterialNum;
			material = &m_materialVector[index]->m_subMaterial[subNum];
		}
		else
		{
			material =  m_materialVector[index];
		}

		// 로딩한 texture handle setting
		face.m_texHandle = material->m_texHandle;

		if (m_currentMesh->m_tv == nullptr)
		{
			continue;
		}


		// text face를 참고하여 uv scale값 적용
		const NtTris& texFace = m_currentMesh->m_rawTFace[i];
		
		m_currentMesh->m_tv[texFace.a].tu *= material->m_scaleU;
		m_currentMesh->m_tv[texFace.a].tv *= material->m_scaleV;

		m_currentMesh->m_tv[texFace.b].tu *= material->m_scaleU;
		m_currentMesh->m_tv[texFace.b].tv *= material->m_scaleV;

		m_currentMesh->m_tv[texFace.c].tu *= material->m_scaleU;
		m_currentMesh->m_tv[texFace.c].tv *= material->m_scaleV;

		// etc..
		face.m_diffuse = material->m_diffuse;
		face.m_texType = material->m_texType;
		face.m_drawFlags = material->m_drawFlag;
	}
}


void NtAse::ParseVertex()
{
	NtWChar name[32] = {0, };
	NtUInt index;
	NtFloat a, b, c;
	do 
	{
		ReadLine();
		SetKeyword();

		if (m_keyword == L"*MESH_VERTEX")
		{
			NtInt res = swscanf_s(GetData(), L"%s %d %f %f %f", name, _countof(name), &index, &a, &c, &b);
			if (res != 5)	return;

			NtAsserte(m_currentMesh->m_vertexNum > index);

			m_currentMesh->m_vertex[index].m_p.x = a;
			m_currentMesh->m_vertex[index].m_p.y = b;
			m_currentMesh->m_vertex[index].m_p.z = c;

			m_currentMesh->m_vertex[index].m_rigid = nullptr;
		}
	} while (!FindKeyword(L"}"));

	ClearData();
}

void NtAse::ParseFace()
{
	NtWChar temp[16] = {0, };
	NtUInt index, a, b, c, subMat;
	do 
	{
		ReadLine();
		SetKeyword();

		if (CompareKeyword(L"*MESH_FACE"))
		{
			//ex : *MESH_FACE   0:    A:   1 B:   2 C:   3 AB:    1 BC:    0 CA:    1	 *MESH_SMOOTHING 0 	*MESH_MTLID 0

			NtInt res = swscanf_s(GetData(), L"%s %d %s %s %d %s %d %s %d",
				temp, _countof(temp), &index, temp, _countof(temp), temp, _countof(temp), &a, temp, _countof(temp), &c, temp, _countof(temp), &b);	/* A, B, C */
			if (res != 9)	return;

			subMat = 0;
			bool realMatID = false;
			const NtWChar* s = Crt::StrChr(GetData() + 1, L'*');
			while (realMatID == false && s != nullptr)
			{
				int matIdRes = swscanf_s(s, L"%s", temp, _countof(temp));
				if (matIdRes == 0)
				{
					break;
				}

				if (Crt::StrStr(temp, L"MESH_MTLID") != nullptr)
				{
					NtInt lastRes = swscanf_s(s + 1, L"%s %d", temp, _countof(temp), &subMat);
					if (lastRes == 2)
					{
						realMatID = true;
					}
				}
				else
				{
					s = Crt::StrChr(s + 1, L'*');
				}
			}

			NtAsserte(m_currentMesh->m_rawIndex != NULL);
			
			m_currentMesh->m_rawIndex[index].a = a;
			m_currentMesh->m_rawIndex[index].b = b;
			m_currentMesh->m_rawIndex[index].c = c;
			m_currentMesh->m_rawIndex[index].m_subMaterialID = subMat;
		}

	} while (!FindKeyword(L"}"));

	ClearData();
}

void NtAse::ParseNormal()
{
	NtWChar temp[32] = {0, };
	NtInt index;
	NtFloat a, b, c;
	do 
	{
		ReadLine();
		SetKeyword();

		if (CompareKeyword(L"*MESH_VERTEXNORMAL"))
		{
			NtInt res = swscanf_s(GetData(), L"%s %d %f %f %f", temp, _countof(temp), &index, &a, &c, &b);
			if (res != 5)	return;

			NtAsserte(&m_currentMesh->m_vertex[index] != nullptr);

			m_currentMesh->m_vertex[index].m_n.x = a;
			m_currentMesh->m_vertex[index].m_n.y = b;
			m_currentMesh->m_vertex[index].m_n.z = c;
		}

	} while (!FindKeyword(L"}"));

	ClearData();
}


void NtAse::ParseTexVertex()
{
	NtWChar temp[32] = {0, };
	NtInt index;
	NtFloat a, b;

	do 
	{
		ReadLine();
		SetKeyword();

		if (CompareKeyword(L"*MESH_TVERT"))
		{
			NtInt res = swscanf_s(GetData(), L"%s %d %f %f", temp, _countof(temp), &index, &a, &b);
			if (res != 4)	return;

			NtAsserte(m_currentMesh->m_rawTFace != nullptr);

			m_currentMesh->m_tv[index].tu = a;
			m_currentMesh->m_tv[index].tv = 1.0f - b;
		}
	} while (!FindKeyword(L"}"));

	ClearData();
}


void NtAse::ParseTexFace()
{
	NtWChar temp[32] = {0, };
	NtInt index, a, b, c;

	do 
	{
		ReadLine();
		SetKeyword();

		if (CompareKeyword(L"*MESH_TFACE"))
		{
			NtInt res = swscanf_s(GetData(), L"%s %d %d %d %d", temp, _countof(temp), &index, &a, &c, &b);
			if (res != 5)	return;

			NtAsserte(m_currentMesh->m_rawTFace != nullptr);

			m_currentMesh->m_rawTFace[index].a = a;
			m_currentMesh->m_rawTFace[index].b = b;
			m_currentMesh->m_rawTFace[index].c = c;
		}

	} while (!FindKeyword(L"}"));

	ClearData();
}


void NtAse::ParsePhysiqueVertex()
{
	NtWChar temp[32] = {0, };
	NtInt index;
	NtFloat a, b, c;

	do 
	{
		ReadLine();
		SetKeyword();

		if (CompareKeyword(L"*MESH_PHYSIQUE_VERTEX"))
		{
			NtInt res = swscanf_s(GetData(), L"%s %d %f %f %f", temp, _countof(temp), &index, &a, &c, &b);
			if (res != 5)	return;

			m_currentMesh->m_vertex[index].m_p.x = a;
			m_currentMesh->m_vertex[index].m_p.y = b;
			m_currentMesh->m_vertex[index].m_p.z = c;
			

			const NtWChar* findRes = Crt::StrStr(GetData(), L"RIGID_TYPE");
			if (findRes != nullptr)
			{
				const NtWChar* s = Crt::StrChr(GetData(), L'\"');
				if (s == NULL)
				{
					OutputErrorMsg(L"Name Found to fail");
					return;
				}

				const NtWChar* e = Crt::StrChr(s + 1, L'\"');
				m_currentMesh->m_vertex[index].m_objName.AssignSubString(s + 1, e - 1);
				m_currentMesh->m_vertex[index].m_rigid = nullptr;
			}
			else
			{
				// BLEND_TYPE
				ParseBlendWeight(index);
			}

			// Get Weight
			const NtWChar* weightPtr = Crt::StrStr(GetData(), L"*WEIGHT");
			if (weightPtr != nullptr)
			{
				NtFloat weightVal = 0.0f;
				NtInt res = swscanf_s(weightPtr, L"%s %f", temp, _countof(temp), &weightVal);
				if (res != 2)	return;

				m_currentMesh->m_vertex[index].m_weight = weightVal;
			}
			else
			{
				m_currentMesh->m_vertex[index].m_weight = 1.0f;
			}
		}

	} while (!FindKeyword(L"}"));

	ClearData();
}


void NtAse::ParseBlendWeight(NtInt index)
{
	const NtWChar* res = Crt::StrStr(GetData(), L"BLENDED_TYPE");
	if (res == nullptr)
	{
		OutputErrorMsg(L"String Found to fail");
		return;
	}

	NtRigid rigidBuffer[128];
	NtSize bufferSize = _countof(rigidBuffer);

	const NtWChar* s = GetData();
	const NtWChar* e = nullptr;
	NtFloat t = 0.0f;
	NtSize i = 0;
	for (; i < bufferSize && t < 1.0f; ++i)
	{
		s = Crt::StrChr(s, L'\"');
		if (s == NULL)
		{
			break;
		}

		e = Crt::StrChr(s + 1, L'\"');

		rigidBuffer[i].m_objName.AssignSubString(s + 1, e - 1);

		// e + 2 위치에 weight값
		rigidBuffer[i].m_blendWeight = Crt::Atof(e + 2);
		t += rigidBuffer[i].m_blendWeight;

		s = e + 1;
	}

	if (t < 1.0f)
	{
		rigidBuffer[i > 0 ? i - 1 : 0].m_blendWeight += (1.0f - t);
	}
	else if (t > 1.0f)
	{
		rigidBuffer[i > 0 ? i - 1 : 0].m_blendWeight -= (t - 1.0f);
	}

	for (NtUInt r1 = 0; r1 < i; ++r1)
	{
		for (NtUInt r2 = r1 + 1; r2 < i; ++r2)
		{
			if (rigidBuffer[r1].m_objName == rigidBuffer[r2].m_objName)
			{
				rigidBuffer[r1].m_blendWeight += rigidBuffer[r2].m_blendWeight;
				rigidBuffer[r2].m_blendWeight = 0.0f;
			}
		}
	}
	
	t = 0.0f;
	for (NtUInt r1 = 0; r1 < i; ++r1)
	{
		t += rigidBuffer[r1].m_blendWeight;
		if (NtMath<NtFloat>::IsZero(rigidBuffer[r1].m_blendWeight))
		{
			if (i - r1 - 1 > 0)
			{
				rigidBuffer[r1] = rigidBuffer[r1 + 1];
			}

			--i;
			--r1;
			continue;
		}
	}

	if (i == 1)
	{
		m_currentMesh->m_vertex[index].m_objName = rigidBuffer[0].m_objName;
		NtAsserte(m_currentMesh->m_vertex[index].m_objName.Size() != 0);

		m_currentMesh->m_vertex[index].m_rigid = nullptr;
		m_currentMesh->m_vertex[index].m_rigidNum = 0;
	}
	else
	{
		NtAsserte(i < _countof(rigidBuffer) && i > 1);

		if (i <= 1)
		{
			m_currentMesh->m_vertex[index].m_objName = rigidBuffer[0].m_objName;
			m_currentMesh->m_vertex[index].m_rigid = nullptr;
			m_currentMesh->m_vertex[index].m_rigidNum = 0;
		}
		else
		{
			std::sort(std::begin(rigidBuffer), std::end(rigidBuffer), [](const NtRigid& a1, const NtRigid& a2) -> bool
			{
				if (a1.m_blendWeight < a2.m_blendWeight)			return false;
				else if (a1.m_blendWeight > a2.m_blendWeight)		return true;

				if (a1.m_objName.Size() == 0 && a2.m_objName.Size() != 0)	return true;
				if (a1.m_objName.Size() != 0 && a2.m_objName.Size() == 0)	return false;

				if ((a1.m_blendWeight == a2.m_blendWeight) && (a1.m_objName == a2.m_objName))	return false;


				return a1.m_objName < a2.m_objName ? false : true;
			});

			m_currentMesh->m_vertex[index].m_rigidNum = i;
			m_currentMesh->m_vertex[index].m_rigid = new NtRigid[i];

			for (i = 0; i < m_currentMesh->m_vertex[index].m_rigidNum; ++i)
			{
				m_currentMesh->m_vertex[index].m_rigid[i] = rigidBuffer[i];
			}

			m_currentMesh->m_vertex[index].m_objName = m_currentMesh->m_vertex[index].m_rigid[0].m_objName;
		}
	}
}


void NtAse::ParseMaterialElement(NtMaterial* material)
{
	do 
	{
		ReadLine();
		SetKeyword();

		if (CompareKeyword(L"*MATERIAL_NAME"))
		{
			ParseName(material->m_materialName);
		}
		else if (CompareKeyword(L"*MATERIAL_CLASS"))
		{
			NtString className;
			ParseName(className);

			material->m_materialClass = GetMaterialClass(className.Buffer());
		}
		else if (CompareKeyword(L"*MATERIAL_AMBIENT") || CompareKeyword(L"*MATERIAL_DIFFUSE"))
		{
			NtFloat r, g, b;
			NtWChar temp[32] = {0, };
			NtInt res = swscanf_s(GetData(), L"%s %f %f %f", temp, _countof(temp), &b, &g, &r);
			if (res != 4)	return;

			material->m_diffuse = NT_RGBA(r*255.0f, g*255.0f, b*255.0f, 255);
		}
		else if (CompareKeyword(L"MATERIAL_SPECULAR"))
		{
			NtFloat r, g, b;
			NtWChar temp[32] = {0, };
			NtInt res = swscanf_s(GetData(), L"%s %f %f %f", temp, _countof(temp), &b, &g, &r);
			if (res != 4)	return;

			material->m_specular[0] = r;
			material->m_specular[1] = g;
			material->m_specular[2] = b;
		}
		else if (CompareKeyword(L"*MATERIAL_TRANSPARENCY"))
		{
			NtFloat a;
			NtWChar temp[32] = {0, };
			NtInt res = swscanf_s(GetData(), L"%s %f", temp, _countof(temp), &a);
			if (res != 2)	return;

			material->m_diffuse = (material->m_diffuse & (0x00ffffff)) | NT_RGBA(0, 0, 0, (255 - (NtInt)(a * 255.0f)));
		}
		else if (CompareKeyword(L"*MATERIAL_SHINE"))
		{
			NtFloat shine;
			NtWChar temp[32];
			NtInt res = swscanf_s(GetData(), L"%s %f", temp, _countof(temp), &shine);
			if (res != 2)	return;


		}
		else if (CompareKeyword(L"*MATERIAL_SHINESTRENGTH"))
		{
			NtFloat shinestrength;
			NtWChar temp[32];
			NtInt res = swscanf_s(GetData(), L"%s %f", temp, _countof(temp), &shinestrength);
			if (res != 2)	return;

			material->m_specular[0] = material->m_specular[0] * shinestrength / 100.0f;
			material->m_specular[1] = material->m_specular[1] * shinestrength / 100.0f;
			material->m_specular[2] = material->m_specular[2] * shinestrength / 100.0f;
		}
		else if (CompareKeyword(L"*MAP_DIFFUSE"))
		{
			ParseMapDiffuse(material);
		}
		else if (CompareKeyword(L"*MAP_OPACITY"))
		{
			ParseMapOpacity();
		}
		else if (CompareKeyword(L"*NUMSUBMTLS"))
		{
			NtInt subMatNum = 0;
			NtWChar temp[32];
			NtInt res = swscanf_s(GetData(), L"%s %d", temp, _countof(temp), &subMatNum);
			if (res != 2)	return;

			if (subMatNum != 0)
			{
				material->m_subMaterial = new NtMaterial[subMatNum];
				material->m_subMaterialNum = subMatNum;
			}
		}
		else if (CompareKeyword(L"*SUBMATERIAL"))
		{
			NtInt subMatIndex = 0;
			NtWChar temp[32];
			NtInt res = swscanf_s(GetData(), L"%s %d", temp, _countof(temp), &subMatIndex);
			if (res != 2)	return;

			if (subMatIndex > m_currentMaterial->m_subMaterialNum)
			{
				NtAsserte(subMatIndex < m_currentMaterial->m_subMaterialNum);
				return;
			}

			m_currentSubMaterial = &m_currentMaterial->m_subMaterial[subMatIndex];

			ParseMaterialElement(m_currentSubMaterial);
		}
	} while (!FindKeyword(L"}"));

	ClearData();
}


void NtAse::ParseMapDiffuse(NtMaterial* material)
{
	do 
	{
		ReadLine();
		SetKeyword();

		if (CompareKeyword(L"*BITMAP"))
		{
			NtString bitMap;
			ParseName(bitMap);
			
			material->m_bitmapName = bitMap;

			NtInt lastPos = material->m_bitmapName.FindLast('\\');
			if (lastPos == -1)
			{
				continue;
			}

			NtString fileName(material->m_bitmapName.Buffer(), lastPos+1, material->m_bitmapName.Size());
			material->m_bitmapName = fileName;

			NtUInt handle = g_resManager->LoadTexture(fileName.Buffer());
			if (INVALID_TEXTURE_HANDLE == handle)
			{
				OutputErrorMsg(Crt::MakeString(L"Texture found failed! : %s", fileName.Buffer()));
				return;
			}
			material->m_texHandle = handle;
		}
		else if (CompareKeyword(L"*UVW_U_TILING"))
		{
			float scale = 0.0f;
			NtWChar temp[32];
			NtInt res = swscanf_s(GetData(), L"%s %f", temp, _countof(temp), &scale);
			if (res != 2)	return;

			material->m_scaleU = scale;
		}
		else if (CompareKeyword(L"*UVW_V_TILING"))
		{
			float scale = 0.0f;
			NtWChar temp[32];
			NtInt res = swscanf_s(GetData(), L"%s %f", temp, _countof(temp), &scale);
			if (res != 2)	return;

			material->m_scaleV = scale;
		}

	} while (!FindKeyword(L"}"));

	ClearData();
}


void NtAse::ParseMapOpacity()
{
	do 
	{
		ReadLine();
		SetKeyword();

	} while (!FindKeyword(L"}"));

	ClearData();
}


void NtAse::SetKeyword()
{
	NtWChar buffer[READ_BUF_SIZE];
	Crt::MemSet(buffer, sizeof(buffer));

	NtInt res = swscanf_s(GetData(), L"%s", buffer, _countof(buffer));
	if (res == EOF)
	{
		return;
	}

	if (Crt::StrLen(buffer) < 1)
	{
		return;
	}

	m_keyword = buffer;
}


bool NtAse::CompareKeyword(const NtWChar* keyword)
{
	return m_keyword == keyword ? true : false;
}


void NtAse::MakeHierarchy()
{
	//auto itor = m_meshMap.begin();
	//for (; itor != m_meshMap.end(); ++itor)
	//{
	//	NtMesh* currentObj = (*itor).second;

	//	// search parent Node
	//	auto parentItor = m_meshMap.find(currentObj->m_parentName);
	//	if (parentItor != m_meshMap.end())
	//	{
	//		NtMesh* parentObj = (*parentItor).second;
	//		currentObj->m_linkParent = parentObj;

	//		parentObj->m_linkChild
	//	}
	//}
}


void NtAse::OutputErrorMsg(const NtWChar* msg)
{
	MessageBox(NULL, msg, L"warning", MB_OK);
}


}	// namespace RENDERER
}	// namespace NT
