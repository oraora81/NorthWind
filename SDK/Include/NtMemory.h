
#pragma once

#include <crtdbg.h>
#include <hash_map>

namespace NT
{

namespace Memory
{
	void SetHeapChecker()
	{
		int flag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);

		// Turn on leak check bit
		flag |= _CRTDBG_LEAK_CHECK_DF;
		_CrtSetDbgFlag(flag);
	}

	struct NtMemBlock
	{
		void* rawMemory;		// 미리 잡아놓은 메모리 주소를 지시하는 포인터
		unsigned index : 7;		
		unsigned used : 1;
	};

	class NtHeapAlloc
	{
	public:
		virtual ~NtHeapAlloc()
		{
			
		}

		static void* operator new(size_t size);
		static void operator delete(void* obj);
		static void* operator new [](size_t size);
		static void operator delete [](void* obj);

		static void DumpMemory()
		{
			std::for_each(m_memAddressMap.begin(), m_memAddressMap.end(), [](std::pair<const void*, NtMemBlock> addValue)
			{
				std::cout << addValue.second.rawMemory << std::endl;
			} );
		}

	private:
		typedef stdext::hash_map<const void*, NtMemBlock>	MEMORY_MAP;
		
		static MEMORY_MAP m_memAddressMap;
	};


	//
	// Small Object MemBlock Manager
	class CSmallObjBlockManager
	{
	public:
		explicit CSmallObjBlockManager(NtUChar initSize)
		{

		}
		
		~CSmallObjBlockManager()
		{

		}

		typedef std::vector<NtMemBlock> MEM_LIST;
		MEM_LIST m_blockList;

		NtUChar* m_srcMemory;
	};

	
	template <typename T>
	bool isHeap(T& obj)
	{
		const void* caster = dynamic_cast<const void*>(obj);
		if (nullptr == caster)
		{
			return false;
		}

		return true;
	}
}	// namespace Memory

}	// namespace NT