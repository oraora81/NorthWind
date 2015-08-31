
#pragma once

namespace nt { namespace renderer {

class NtAse : public nt::FS::NtFile
{
	enum eParseMODE
	{
		PARSE_KEYWORD,
		PARSE_GEOMOBJECT,
	};

public:
	explicit NtAse(const ntWchar* fileName);
	~NtAse();

	// 상위 키워드 탐색 > 키워드에 맞는 파싱 모드 진입
	//					하위 키워드 탐색
	bool Parse();
	const std::map<NtString, NtMesh*>& GetMeshMap();

private:
	bool FindKeyword(ntWchar* keyword);
	
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
	void ParseBlendWeight(ntInt index);

	void ParseMaterialElement(NtMaterial* material);
	void ParseMapDiffuse(NtMaterial* material);
	void ParseMapOpacity();

	void SetKeyword();
	bool CompareKeyword(const ntWchar* keyword);

	void MakeHierarchy();

	void OutputErrorMsg(const ntWchar* msg);

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
