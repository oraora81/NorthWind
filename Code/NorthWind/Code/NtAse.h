
#pragma once

namespace NT
{
namespace RENDERER
{

class NtAse : public NT::FS::NtFile
{
	enum eParseMODE
	{
		PARSE_KEYWORD,
		PARSE_GEOMOBJECT,
	};

public:
	explicit NtAse(const NtWChar* fileName);
	~NtAse();

	// ���� Ű���� Ž�� > Ű���忡 �´� �Ľ� ��� ����
	//					���� Ű���� Ž��
	bool Parse();
	const std::map<NtString, NtMesh*>& GetMeshMap();

private:
	bool FindKeyword(NtWChar* keyword);
	
	bool ParseGeomObject();
	bool ParseMaterial();

	void ParseName(NtString& name);
	void ParseNodeName();
	void ParseParentName();
	void ParseTM();
	void ParseMesh();
	void ParseMaterialRef();
	void ParseVertex();
	void ParseFace();
	void ParseNormal();
	void ParseTexVertex();
	void ParseTexFace();
	void ParsePhysiqueVertex();
	void ParseBlendWeight(NtInt index);

	void ParseMaterialElement(NtMaterial* material);
	void ParseMapDiffuse(NtMaterial* material);
	void ParseMapOpacity();

	void SetKeyword();
	bool CompareKeyword(const NtWChar* keyword);

	void MakeHierarchy();

	void OutputErrorMsg(const NtWChar* msg);

private:
	eParseMODE m_mode;

	std::map<NtString, NtMesh*> m_meshMap;
	std::vector<NtMaterial*>	m_materialVector;
	NtString m_keyword;
	NtMesh* m_currentMesh;
	NtMaterial* m_currentMaterial;
	NtMaterial* m_currentSubMaterial;
};

}
}
