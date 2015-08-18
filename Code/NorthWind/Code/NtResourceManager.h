
#pragma once

namespace NT
{

namespace RENDERER
{
	class NtRenderer;
	class NtTexture;
	class NtTexHandle;
}

namespace FS
{

using namespace RENDERER;

struct NtResource
{
	NtWChar	wholePath[1024];
	NtSize	fileNum;
	NtResource* next;
};


class NtResourceManager
{
	enum { INIT_TEX_HANDLE_SIZE = 10000 };

public:
	NtResourceManager();
	~NtResourceManager();

	bool Initialize(const NtString& str);
	void UpdateWholeDirectoryFiles(const NtWChar* directory);
	void AddMappingInfo(const NtWChar* fileName, NtWChar* wholePath);
	void SetEnvDirectory(const NtString& str);

	NtUInt		LoadTexture(const NtWChar* fileName);
	NtTexture*	AcquireTexture(NtUInt handle);
	void		ReleaseTexture(NtUInt handle);


	NtFile*			GetFile(const NtWChar* fileName);
	const NtWChar*	GetWholePath(const NtWChar* fileName);
	

private:
	const NtResource* FindResource(const NtWChar* fileName);
	NtTexHandle*	FindUsableHandle();
	void		AddTexHandleObj();


private:
	typedef std::map<NtString, NtResource*>		RESOURCE_MAPPING_TABLE;
	typedef std::map<NtString, NtTexture*>		TEXTURE_TABLE;
	typedef std::vector<NtTexHandle*>			TEXTURE_REFERENCE_ARRAY;

	RESOURCE_MAPPING_TABLE	m_resourceTable;
	TEXTURE_TABLE			m_texTable;
	TEXTURE_REFERENCE_ARRAY	m_texReferenceArray;
	TEXTURE_REFERENCE_ARRAY	m_texHandleSource;

	NtString				m_env;
};


}	// namespace FS
}	// namespace NT
