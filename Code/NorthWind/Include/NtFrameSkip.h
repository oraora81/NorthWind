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

	void SetFramePerSec(ntFloat fps)
	{
		m_secPerFrame = 1.0f / fps;
	}

	// ���ϴ� �����Ӻ��� �ʹ� ������,
	// false�� ��ȯ�Ͽ� �ڵ带 ���۽�Ű�� �ʵ��� ��
	// dt�� '��'����(�и��� �ƴ�!!!)
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
