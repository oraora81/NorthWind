
#pragma once


#pragma once

namespace nt
{

class NtTimer
{
public:
	NtTimer();
	~NtTimer();

	void Reset();
	void Start();
	void Stop();
	void Tick();

    float TotalTime() const;

    inline float DeltaTime() const
    {
        return (float)m_deltaTime;
    }

private:
	ntDouble m_secondsPerCount;
	ntDouble m_deltaTime;
	ntInt64 m_baseTime;
	ntInt64 m_pausedTime;
	ntInt64 m_stopTime;
	ntInt64 m_prevTime;
	ntInt64 m_currTime;

    bool m_stopped;
};

}
