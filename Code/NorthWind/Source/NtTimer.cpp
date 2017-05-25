
#include "NtCoreLib.h"
#pragma hdrstop

#include "NtTimer.h"

namespace nt
{

NtTimer::NtTimer()
	: m_secondsPerCount(0.0)
	, m_deltaTime(-1.0f)
	, m_baseTime(0)
	, m_pausedTime(0)
	, m_prevTime(0)
	, m_currTime(0)
	, m_stopped(false)
{
	ntInt64 countsPerSec;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
	m_secondsPerCount = 1.0 / (double)countsPerSec;
}


NtTimer::~NtTimer()
{

}

void NtTimer::Reset()
{
	ntInt64 currTime = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

	m_baseTime = currTime;
	m_prevTime = currTime;
	m_stopTime = 0;
	m_stopped  = false;
}

void NtTimer::Stop()
{
	// 이미 정지 상태이면 아무것도 안함
	if (m_stopped == false)
	{
		ntInt64 currTime = 0;
		QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

		// 중단 시간을 저장
		m_stopTime = currTime;
		m_stopped  = true;
	}
}

void NtTimer::Start()
{
	ntInt64 startTime = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&startTime);

	// 중단과 시작(재개) 사이에 흐른 시간을 누적
	//
	//                  |<------d----->|
	// -----------------*--------------*-------------> 시간
	//              m_StopTime        startTime
	
	if (m_stopped)
	{
		m_pausedTime += (startTime - m_stopTime);

		// 타이머를 다시 시작하는 것이므로 m_prevTime의 현재 값은 유효하지 않다.
		// 현재 시간으로 다시 설정함
		m_prevTime = startTime;
		m_stopTime = 0;
		m_stopped = false;
	}
}

float NtTimer::TotalTime() const
{
	if (m_stopped)
	{
		// 현재 타이머가 중단 상태이면 중단 후로부터 흐른 시간은 계산하지 말아야 한다.
		// 이전에 일시 정지된 적이 있다면 그 시간도 전체 시간에 포함시키지 않는다
		//
		//                  이전의 일시 정지시간
		//                  |<------d----->|
		// -----------------*--------------*-------------> 시간
		//m_baseTime           m_StopTime        m_currTime

		return (float)(((m_stopTime - m_pausedTime) - m_baseTime) * m_secondsPerCount);
	}
	else
	{
		// 시간차 m_currTime - m_baseTime에는 일시 정지된 시간이 포함되어 있다.
		// 이를 전체 시간에 포함하면 안되니까 그 시간을 m_currTime에서 뺀다.
		return (float)(((m_currTime - m_pausedTime) - m_baseTime) * m_secondsPerCount);
	}
}

void NtTimer::Tick()
{
	if (m_stopped)
	{
		m_deltaTime = 0.0;
		return;
	}

	// 이번 프레임의 시간을 얻는다.
	ntInt64 currTime = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
	m_currTime = currTime;

	// 이 시간과 이전 프레임의 시간 차이를 구한다.
	m_deltaTime = (m_currTime - m_prevTime) * m_secondsPerCount;

	// 다음 프레임을 준비한다.
	m_prevTime = m_currTime;

	// 음수가 되지 않도록 한다.
	// SDK에 있는 CDXUTTimer따르면 프로세서가 절전모드로 들어가거나
	// 다른 프로세서와 엉키는 경우 deltaTime이 음수가 될 수 있다고 한다.
	if (m_deltaTime < 0.0)
	{
		m_deltaTime = 0.0;
	}
}


}
