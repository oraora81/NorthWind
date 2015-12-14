
#pragma once

namespace nt { namespace INPUT {

class NtInputManager
{
	enum { MAX_KEY_STATE = 256 };

public:
	NtInputManager() {}
	~NtInputManager() {}

	bool Initialize();

	void KeyDown(ntUint key);
	void KeyUp(ntUint key);
	bool IsKeyDown(ntUint key);

private:
	ntChar m_keystate[MAX_KEY_STATE];
};

}	// namespace INPUT
}	// namespace nt
