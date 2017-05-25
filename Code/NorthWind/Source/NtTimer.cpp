
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
	// �̹� ���� �����̸� �ƹ��͵� ����
	if (m_stopped == false)
	{
		ntInt64 currTime = 0;
		QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

		// �ߴ� �ð��� ����
		m_stopTime = currTime;
		m_stopped  = true;
	}
}

void NtTimer::Start()
{
	ntInt64 startTime = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&startTime);

	// �ߴܰ� ����(�簳) ���̿� �帥 �ð��� ����
	//
	//                  |<------d----->|
	// -----------------*--------------*-------------> �ð�
	//              m_StopTime        startTime
	
	if (m_stopped)
	{
		m_pausedTime += (startTime - m_stopTime);

		// Ÿ�̸Ӹ� �ٽ� �����ϴ� ���̹Ƿ� m_prevTime�� ���� ���� ��ȿ���� �ʴ�.
		// ���� �ð����� �ٽ� ������
		m_prevTime = startTime;
		m_stopTime = 0;
		m_stopped = false;
	}
}

float NtTimer::TotalTime() const
{
	if (m_stopped)
	{
		// ���� Ÿ�̸Ӱ� �ߴ� �����̸� �ߴ� �ķκ��� �帥 �ð��� ������� ���ƾ� �Ѵ�.
		// ������ �Ͻ� ������ ���� �ִٸ� �� �ð��� ��ü �ð��� ���Խ�Ű�� �ʴ´�
		//
		//                  ������ �Ͻ� �����ð�
		//                  |<------d----->|
		// -----------------*--------------*-------------> �ð�
		//m_baseTime           m_StopTime        m_currTime

		return (float)(((m_stopTime - m_pausedTime) - m_baseTime) * m_secondsPerCount);
	}
	else
	{
		// �ð��� m_currTime - m_baseTime���� �Ͻ� ������ �ð��� ���ԵǾ� �ִ�.
		// �̸� ��ü �ð��� �����ϸ� �ȵǴϱ� �� �ð��� m_currTime���� ����.
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

	// �̹� �������� �ð��� ��´�.
	ntInt64 currTime = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
	m_currTime = currTime;

	// �� �ð��� ���� �������� �ð� ���̸� ���Ѵ�.
	m_deltaTime = (m_currTime - m_prevTime) * m_secondsPerCount;

	// ���� �������� �غ��Ѵ�.
	m_prevTime = m_currTime;

	// ������ ���� �ʵ��� �Ѵ�.
	// SDK�� �ִ� CDXUTTimer������ ���μ����� �������� ���ų�
	// �ٸ� ���μ����� ��Ű�� ��� deltaTime�� ������ �� �� �ִٰ� �Ѵ�.
	if (m_deltaTime < 0.0)
	{
		m_deltaTime = 0.0;
	}
}


}
