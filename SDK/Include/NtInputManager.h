
#pragma once

namespace NT
{
namespace INPUT
{

class NtInputManager
{
	enum { MAX_KEY_STATE = 256 };

public:
	NtInputManager() {}
	~NtInputManager() {}

	bool Initialize();

	void KeyDown(NtUInt key);
	void KeyUp(NtUInt key);
	bool IsKeyDown(NtUInt key);

private:
	NtChar m_keystate[MAX_KEY_STATE];
};

}	// namespace INPUT
}	// namespace NT