
#include "NtCoreLib.h"

namespace nt
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