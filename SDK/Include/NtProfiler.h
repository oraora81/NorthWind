
#pragma once

#include "NtString.h"
#include <map>
#include <Windows.h>

namespace NT
{

// Ʈ�� ������ �ٲ��� ������ �׳� ������ ��ü �ð� üũ �ۿ� ������ ������ ������.
class NtProfiler
{
	struct sProfileItem
	{
		NtString		funcName;
		LARGE_INTEGER	startTime;
		NtDouble		doration;

		sProfileItem()
			: funcName()
			, doration(0.0)
		{
			memset(&startTime, 0, sizeof(LARGE_INTEGER));
		}
	};

public:
	// ���߿� Customize �������� ��������.
	//typedef std::map<NtIndex, sProfileItem>	PROFILE_INFO_MAP;
	//typedef PROFILE_INFO_MAP::iterator		PROFILE_INFO_ITOR;

	NtProfiler(NtWChar* currentFunc);

	// �Ҹ��ڿ��� �������ϸ� ������ �ٽ� üũ
	~NtProfiler();

	// End
	void		StampProfile();
	void		FlushProfile();

	//static NtLong GetCallingCount();
	//static void IncCallingCount();
	//static void DecCallingCount();

	//static NtLong GetCallingIndex();
	//static void IncCallingIndex();
	//static void ClearInfo();
	//static PROFILE_INFO_MAP& GetInfo() 
	//{
	//	static PROFILE_INFO_MAP m_info;

	//	return m_info;
	//}

	sProfileItem	m_profile;
	//static NtLong	m_callingCount;
	//static NtLong	m_callingIndex;
};


class NtProfileManager
{
public:
	NtProfileManager();
	~NtProfileManager();

private:

};
}	// namespace NT