#pragma once

namespace nt {

class NtFrameSkip
{
public:
	NtFrameSkip()
	{
		Clear();
	}

	virtual ~NtFrameSkip()
	{

	}

	void Clear()
	{
		SetFramePerSec(60.0f);
		m_timer = 0.0f;
	}

	void SetFramePerSec(float fps)
	{
		m_secPerFrame = 1.0f / fps;
	}

	// 원하는 프레임보다 너무 빠르면,
	// false를 반환하여 코드를 동작시키지 않도록 함
	// dt는 '초'단위(밀리초 아님!!!)
	bool Update(float deltaTime)
	{
		m_timer += deltaTime;
		if (m_timer < 0.0f)
		{
			return false;
		}

		m_timer -= m_secPerFrame;
		return true;
	}

	bool IsFrameSkip()
	{
		return m_timer >= 0.0f;
	}

protected:
	float m_timer;
	float m_secPerFrame;
};

}
