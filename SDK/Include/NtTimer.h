
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
	ntFloat GetTime();
	ntFloat	GetFrameTime();

private:
	ntDouble	m_countsPerSecond;
	ntInt64		m_counterStart;
	ntInt64		m_frameTimeOld;
	ntFloat		m_frameTime;

public:
	ntInt		m_frameCount;
	ntInt		m_fps;
};

}
