
#include "NtCoreLib.h"
#include "NtInputManager.h"

namespace nt { namespace input {

bool NtInputManager::Initialize()
{
	nt::Crt::MemSet(m_keystate, sizeof(m_keystate));

	return true;
}

void NtInputManager::KeyDown(ntUint key)
{
	NtAsserte(key < MAX_KEY_STATE);

	m_keystate[key] = TRUE;
}

void NtInputManager::KeyUp(ntUint key)
{
	NtAsserte(key < MAX_KEY_STATE);

	m_keystate[key] = FALSE;
}

bool NtInputManager::IsKeyDown(ntUint key)
{
	NtAsserte(key < MAX_KEY_STATE);

	return m_keystate[key] == TRUE;
}


}
}
