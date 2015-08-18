
#pragma once


#pragma once

namespace NT
{

class NtTimer
{
public:
	NtTimer();
	~NtTimer();

	void	StartTimer();
	NtFloat GetTime();
	NtFloat	GetFrameTime();

private:
	NtDouble	m_countsPerSecond;
	NtInt64		m_counterStart;
	NtInt64		m_frameTimeOld;
	NtFloat		m_frameTime;

public:
	NtInt		m_frameCount;
	NtInt		m_fps;
};

}
