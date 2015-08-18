
#include "NtCoreLib.h"
#pragma hdrstop

#include "NtTimer.h"

namespace NT
{

NtTimer::NtTimer()
{
	m_countsPerSecond = 0.0f;
	m_counterStart = 0;
	m_frameCount = 0;
	m_fps = 0;
	m_frameTimeOld = 0;
	m_frameTime = 0.0f;
}



NtTimer::~NtTimer()
{

}

void NtTimer::StartTimer()
{
	LARGE_INTEGER frequencyCount;
	::QueryPerformanceFrequency(&frequencyCount);

	m_countsPerSecond = static_cast<NtDouble>(frequencyCount.QuadPart);

	::QueryPerformanceCounter(&frequencyCount);
	m_counterStart = frequencyCount.QuadPart;
}

NtFloat NtTimer::GetTime()
{
	NtAsserte(m_countsPerSecond != 0.0f);

	LARGE_INTEGER currentTime;
	::QueryPerformanceCounter(&currentTime);
	return static_cast<NtFloat>((currentTime.QuadPart - m_counterStart) / m_countsPerSecond);
}


NtFloat NtTimer::GetFrameTime()
{
	LARGE_INTEGER currentTime;
	NtInt64 tickCount;
	::QueryPerformanceCounter(&currentTime);

	tickCount = currentTime.QuadPart - m_frameTimeOld;
	m_frameTimeOld = currentTime.QuadPart;

	if (tickCount < 0)
	{
		tickCount = 0;
	}

	NtAsserte(m_countsPerSecond != 0.0f);

	return static_cast<NtFloat>(tickCount / m_countsPerSecond);
}


}
