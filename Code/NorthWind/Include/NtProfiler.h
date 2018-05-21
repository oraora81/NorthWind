
#pragma once

#include "NtString.h"
#include <map>
#include <Windows.h>

namespace nt
{

// Ʈ�� ������ �ٲ��� ������ �׳� ������ ��ü �ð� üũ �ۿ� ������ ������ ������.

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
	// ���߿� Customize �������� ��������.
	//typedef std::map<ntIndex, sProfileItem>	PROFILE_INFO_MAP;
	//typedef PROFILE_INFO_MAP::iterator		PROFILE_INFO_ITOR;

	NtProfiler(ntWchar* currentFunc);

	// �Ҹ��ڿ��� �������ϸ� ������ �ٽ� üũ
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
