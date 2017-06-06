
#include "NtCoreLib.h"
#pragma hdrstop

#include "NtResourceManager.h"

#include "NtTexture.h"
#include "NtRenderer.h"

namespace nt { namespace fs {

NtResourceManager::NtResourceManager()
{
	// Construct
}

NtResourceManager::~NtResourceManager()
{
	std::for_each(std::begin(m_texHandleSource), std::end(m_texHandleSource), [](const NtTexHandle* texHandle)
	{
		SAFE_DELETE(texHandle);
	});

	std::for_each(std::begin(m_texTable), std::end(m_texTable), [](TEXTURE_TABLE::value_type& texvaluePair)
	{
		if (texvaluePair.second)
		{
			SAFE_DELETE(texvaluePair.second);
		}
	});
}


bool NtResourceManager::Initialize(const NtString& str)
{
	m_env = str;

	UpdateWholeDirectoryFiles(m_env.Buffer());

	m_texReferenceArray.reserve(INIT_TEX_HANDLE_SIZE);

	// tex handle 참조용 원본 리스트 생성
	AddTexHandleObj();
	
	return true;
}


void NtResourceManager::UpdateWholeDirectoryFiles(const ntWchar* directory)
{
	if (m_env.Empty())
	{
		return;
	}

	ntWchar filter[256], name[256];
	WIN32_FIND_DATA fd;
	HANDLE find;

	Crt::MemSet(filter, sizeof(filter));
	Crt::MemSet(name, sizeof(name));

	Crt::StrCpy(filter, directory, _countof(filter));
	Crt::StrCat(filter, _countof(filter), L"*.*");;

	find = FindFirstFile(filter, &fd);

	if (find != (HANDLE)-1)
	{
		do
		{
			if (fd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
			{
				if (Crt::StrCmp(fd.cFileName, L".") && Crt::StrCmp(fd.cFileName, L".."))
				{
					swprintf_s(name, _countof(name), L"%s%s/", directory, fd.cFileName);
					UpdateWholeDirectoryFiles(name);
				}
			}
			else
			{
				swprintf_s(name, _countof(name), L"%s%s", directory, fd.cFileName);
				AddMappingInfo(fd.cFileName, name);
			}

		}	while (FindNextFile(find, &fd));
	}

	FindClose(find);
}

// ex : [abc.txt], [c:\target\abc.txt]
void NtResourceManager::AddMappingInfo(const ntWchar* fileName, ntWchar* wholePath)
{
	Crt::ToLower(wholePath, Crt::StrLen(wholePath));	

	NtString cpStr(fileName);
	cpStr.Lowercase();

	auto res = m_resourceTable.find(cpStr);
	if (res == m_resourceTable.end())
	{
		NtResource* resource = new NtResource;
		Crt::StrCpy(resource->wholePath, wholePath, _countof(resource->wholePath));
		resource->next = nullptr;
		resource->fileNum = 1;
		m_resourceTable.insert(std::make_pair(cpStr, resource));
	}
	else
	{
		NtResource* srcRource = (*res).second;
		srcRource->fileNum++;

		NtResource* dupl = new NtResource;
		dupl->fileNum = 0;
		Crt::StrCpy(dupl->wholePath, wholePath, _countof(dupl->wholePath));
		dupl->next = nullptr;

		srcRource->next = dupl;
	}
}


void NtResourceManager::SetEnvDirectory(const NtString& str)
{
	m_env = str;
}


ntUint NtResourceManager::LoadTexture(const ntWchar* fileName)
{
	NtString texName(fileName);
	auto findRes = m_texTable.find(texName);
	if (findRes != m_texTable.end())
	{
		NtTexture* tex = (*findRes).second;
		tex->IncreaseUseCount();
		return tex->TexHandle();
	}

	NtTexture* tex = new NtTexture;
	bool res = tex->Initialize(fileName);
	if (false == res)
	{
		return INVALID_TEXTURE_HANDLE;
	}

	// find usable handle
	NtTexHandle* texHandle = FindUsableHandle();

	tex->IncreaseUseCount();
	tex->SetTexHandle(texHandle->GetHandle());

	texHandle->SetTexture(tex);

	m_texTable.insert(std::make_pair(fileName, tex));

	return tex->TexHandle();
}


NtTexture* NtResourceManager::AcquireTexture(ntUint handle)
{
	if (handle == INVALID_TEXTURE_HANDLE ||
		(ntUint)m_texReferenceArray.size() <= handle)
	{
		return nullptr;
	}

	return m_texReferenceArray[handle]->GetTexture();
}


void NtResourceManager::ReleaseTexture(ntUint handle)
{
	NtTexture* texture = AcquireTexture(handle);
	if (texture == nullptr)
	{
		NtAsserte(false);
		return;
	}

	if (texture->UseCount() == 1)
	{
		SAFE_DELETE(texture);
	}
	else
	{
		texture->DecreaseUseCount();
	}
}


NtFile* NtResourceManager::GetFile(const ntWchar* fileName)
{
	const NtResource* resource = FindResource(fileName);
	if (resource == nullptr)
	{
		return nullptr;
	}

	NtFile* fp = new NtFile();
	if (false == fp->Execute(resource->wholePath))
	{
		return nullptr;
	}

	return fp;
}


const ntWchar* NtResourceManager::GetWholePath(const ntWchar* fileName)
{
	const NtResource* resource = FindResource(fileName);
	return resource != nullptr ? resource->wholePath : nullptr;
}


const NtResource* NtResourceManager::FindResource(const ntWchar* fileName)
{
	NtString name(fileName);
	auto res = m_resourceTable.find(name.Buffer());
	if (res == m_resourceTable.end())
	{
		return nullptr;
	}

	return (*res).second;
}


NtTexHandle* NtResourceManager::FindUsableHandle()
{
	auto pred = [](const NtTexHandle* handleObj)
	{
		return handleObj->IsUsable() ? true : false;
	};

	auto res = std::find_if(std::begin(m_texReferenceArray), std::end(m_texReferenceArray), pred);

	// 발견하지 못한다면 버퍼를 늘려준다.
	if (res == m_texReferenceArray.end())
	{
		AddTexHandleObj();

		auto found = std::find_if(std::begin(m_texReferenceArray), std::end(m_texReferenceArray), pred);
		NtAsserte(found != m_texReferenceArray.end());
		
		return (*found);
	}

	return (*res);
}


void NtResourceManager::AddTexHandleObj()
{
	NtTexHandle* handleList = new NtTexHandle[INIT_TEX_HANDLE_SIZE];
	
	ntInt size = static_cast<ntInt>(m_texHandleSource.size());
	for (ntInt i = 0; i < INIT_TEX_HANDLE_SIZE; ++i)
	{
		handleList[i].SetHandle(size * INIT_TEX_HANDLE_SIZE + i);
		m_texReferenceArray.push_back(&handleList[i]);
	}

	m_texHandleSource.push_back(handleList);
}

}	//namespace fs
}	// namespace nt
