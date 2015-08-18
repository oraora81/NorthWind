
#include "NtCoreLib.h"

namespace NT
{

bool volatile NtSetup::m_systemInitialize = false;

void NtSetup::Setup()
{
	if (!m_systemInitialize)
	{
		m_systemInitialize = true;

		NtString::InitEmptyBuffer();
	}
}

}