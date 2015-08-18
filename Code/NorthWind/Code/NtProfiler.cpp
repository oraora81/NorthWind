
#include "NtCoreLib.h"
#include "NtProfiler.h"

namespace NT
{
//ntLong NtProfiler::m_callingCount = 0;
//ntLong NtProfiler::m_callingIndex = 0;

NtProfiler::NtProfiler(ntWchar* currentFunc)
{
	sProfileItem item;

	item.funcName = currentFunc;
	//if (0 == GetCallingCount())
	//{
	//	NtProfiler::ClearInfo();
	//	NtProfiler::m_callingIndex = 0;
	//	DWORD_PTR oldMask = ::SetThreadAffinityMask(::GetCurrentThread(), 0);
	//	::QueryPerformanceCounter(&item.startTime);
	//	::SetThreadAffinityMask(::GetCurrentThread(), oldMask);
	//}
	//else
	//{
	//	::QueryPerformanceCounter(&item.startTime);
	//}

	//GetInfo().insert(std::make_pair(NtProfiler::GetCallingIndex(), item));
	//IncCallingCount();
	//IncCallingIndex();
}

NtProfiler::~NtProfiler()
{
	//DecCallingCount();
	//StampProfile();
	//if (0 == GetCallingCount())
	//{
	//	FlushProfile();
	//}
}

void NtProfiler::StampProfile()
{
	//if (0 > GetCallingCount())
	//{
	//	return;
	//}
	//LARGE_INTEGER frequency;
	//QueryPerformanceFrequency(&frequency);

	//// 현재 완벽한 보정은 무리.
	//LARGE_INTEGER end;
	//memset(&end, 0, sizeof(end));

	//DWORD_PTR oldMask = ::SetThreadAffinityMask(::GetCurrentThread(), 0);
	//::QueryPerformanceCounter(&end);
	//::SetThreadAffinityMask(::GetCurrentThread(), oldMask);

	//PROFILE_INFO_ITOR itor = NtProfiler::GetInfo().find());
	//if (itor == NtProfiler::GetInfo().end())
	//{
	//	return;
	//}
	//sProfileItem& item = (*itor).second;

	//double time = ((end.QuadPart - item.startTime.QuadPart) / (double)frequency.QuadPart);

	//item.doration = time;
}

void NtProfiler::FlushProfile()
{
	//PROFILE_INFO_MAP& infoMap = NtProfiler::GetInfo();

	//PROFILE_INFO_ITOR itor = infoMap.begin();

	//ntDouble sumDoration = 0.0;
	//for (; itor != infoMap.end(); ++itor)
	//{
	//	sProfileItem& item = (*itor).second;
	//	std::wcout << L"Function : "
	//			   << item.funcName << L", "
	//			   << L"Doration : " << item.doration << L"\n";

	//	sumDoration += item.doration;
	//}

	//std::wcout << L"All doration : " << sumDoration << L"\n";
}

///*static*/ ntLong NtProfiler::GetCallingCount()
//{
//	return NtProfiler::m_callingCount;
//}
//
///*static*/ void NtProfiler::IncCallingCount()
//{
//	NT::Atom::Inc(m_callingCount);
//}
//
///*static*/ void NtProfiler::DecCallingCount()
//{
//	NT::Atom::Dec(m_callingCount);
//}
//
///*static*/ ntLong NtProfiler::GetCallingIndex()
//{
//	return NtProfiler::m_callingIndex;
//}
//
///*static*/ void NtProfiler::IncCallingIndex()
//{
//	NT::Atom::Inc(m_callingIndex);
//}
//
///*static*/ void NtProfiler::ClearInfo()
//{
//	NtProfiler::GetInfo().clear();
//}

}	// namespace NT