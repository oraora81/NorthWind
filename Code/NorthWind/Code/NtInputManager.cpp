
#include "NtCoreLib.h"
#include "NtInputManager.h"

namespace NT
{
namespace INPUT
{

bool NtInputManager::Initialize()
{
	NT::Crt::MemSet(m_keystate, sizeof(m_keystate));

	return true;
}

void NtInputManager::KeyDown(NtUInt key)
{
	NtAsserte(key < MAX_KEY_STATE);

	m_keystate[key] = TRUE;
}

void NtInputManager::KeyUp(NtUInt key)
{
	NtAsserte(key < MAX_KEY_STATE);

	m_keystate[key] = FALSE;
}

bool NtInputManager::IsKeyDown(NtUInt key)
{
	NtAsserte(key < MAX_KEY_STATE);

	return m_keystate[key] == TRUE;
}


}
}