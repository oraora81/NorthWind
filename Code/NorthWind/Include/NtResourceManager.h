
#pragma once

namespace nt
{

namespace renderer
{
	class NtRenderer;
	class NtTexture;
	class NtTexHandle;
}

namespace fs
{

using namespace renderer;

struct NtResource
{
	ntWchar	wholePath[1024];
	ntSize	fileNum;
	NtResource* next;
};


class NtResourceManager
{
	enum { INIT_TEX_HANDLE_SIZE = 10000 };

public:
	NtResourceManager();

	~NtResourceManager();

	bool Initialize(const NtString& str);

	void UpdateWholeDirectoryFiles(const ntWchar* directory);

	void AddMappingInfo(const ntWchar* fileName, ntWchar* wholePath);

	void SetEnvDirectory(const NtString& str);

	ntUint LoadTexture(const ntWchar* fileName);

	NtTexture* AcquireTexture(ntUint handle);

	void ReleaseTexture(ntUint handle);

	NtFile*	GetFile(const ntWchar* fileName);

	const ntWchar* GetPath(const ntWchar* fileName);
	
private:
	const NtResource* FindResource(const ntWchar* fileName);

	NtTexHandle* FindUsableHandle();

	void AddTexHandleObj();


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


}	// namespace fs
}	// namespace nt
