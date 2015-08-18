
#pragma once

namespace NT
{

class NtSetup
{
public:
	static void	Setup();

public:
	static NtBool volatile m_systemInitialize;
};

}