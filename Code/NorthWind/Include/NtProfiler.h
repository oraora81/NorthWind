
#pragma once

#include "NtString.h"
#include <map>
#include <Windows.h>

namespace nt
{

// 트리 구조로 바꾸지 않으면 그냥 찍어봐야 전체 시간 체크 밖에 유용한 정보가 없을듯.

class NtProfiler
{
	struct sProfileItem
	{
		NtString		funcName;
		LARGE_INTEGER	startTime;
		double		doration;

		sProfileItem()
			: funcName()
			, doration(0.0)
		{
			memset(&startTime, 0, sizeof(LARGE_INTEGER));
		}
	};

public:
	// 나중에 Customize 버전으로 수정하자.
	//typedef std::map<ntIndex, sProfileItem>	PROFILE_INFO_MAP;
	//typedef PROFILE_INFO_MAP::iterator		PROFILE_INFO_ITOR;

	NtProfiler(ntWchar* currentFunc);

	// 소멸자에서 프로파일링 중인지 다시 체크
	~NtProfiler();

	// End
	void		StampProfile();
	void		FlushProfile();

	//static ntLong GetCallingCount();
	//static void IncCallingCount();
	//static void DecCallingCount();

	//static ntLong GetCallingIndex();
	//static void IncCallingIndex();
	//static void ClearInfo();
	//static PROFILE_INFO_MAP& GetInfo() 
	//{
	//	static PROFILE_INFO_MAP m_info;

	//	return m_info;
	//}

	sProfileItem	m_profile;
	//static ntLong	m_callingCount;
	//static ntLong	m_callingIndex;
};


class NtProfileManager
{
public:
	NtProfileManager();
	~NtProfileManager();

private:

};
}	// namespace nt
