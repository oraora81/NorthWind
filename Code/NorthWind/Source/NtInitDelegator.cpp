
#include "NtCoreLib.h"
#include "NtInitDelegator.h"

namespace nt {

int NtInitDelegator::m_initCount = 0;
int NtInitDelegator::m_releaseCount = 0;
NtInitDelegator::InitDelegator NtInitDelegator::m_initDelegatorList[MAX_DELEGATOR_COUNT];
NtInitDelegator::ReleaseDelegator NtInitDelegator::m_releaseDelegatorList[MAX_DELEGATOR_COUNT];

void NtInitDelegator::AddInitDelegator(InitDelegator func)
{
	if (m_initCount < MAX_DELEGATOR_COUNT)
	{
		m_initDelegatorList[m_initCount++] = func;
	}
	else
	{
		NtAsserte(false);
	}
}

void NtInitDelegator::ExecuteInitDelegator()
{
	ntInt index = 0;
	for (; index < m_initCount; ++index)
	{
		m_initDelegatorList[index]();
	}
}

void NtInitDelegator::AddReleaseDelegator(ReleaseDelegator func)
{
	if (m_releaseCount < MAX_DELEGATOR_COUNT)
	{
		m_releaseDelegatorList[m_releaseCount++] = func;
	}
	else
	{
		NtAsserte(false);
	}
}

void NtInitDelegator::ExecuteReleaseDelegator()
{
	ntInt index = 0;
	for (; index < m_releaseCount; ++index)
	{
		m_releaseDelegatorList[index]();
	}
}

}	// namespace nt
