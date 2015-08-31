
#pragma once

namespace nt {

class NtInitDelegator
{
public:
	typedef void (*InitDelegator)();
	static void AddInitDelegator(InitDelegator func);
	static void ExecuteInitDelegator();

	typedef void (*ReleaseDelegator)();
	static void AddReleaseDelegator(ReleaseDelegator func);
	static void ExecuteReleaseDelegator();

private:

	enum { MAX_DELEGATOR_COUNT = 256 };

	static ntInt m_initCount;
	static InitDelegator m_initDelegatorList[MAX_DELEGATOR_COUNT];

	static ntInt m_releaseCount;
	static ReleaseDelegator m_releaseDelegatorList[MAX_DELEGATOR_COUNT];
};

}	// namespace nt

// Init
#define NT_DECLARE_INIT_DELEGATE \
public: \
	static bool RegisterInitDelegator(); \
	static void Init(); \
private: \
	static bool m_initialized;

//
#define NT_IMPLEMENT_INIT_DELEGATE(classname) \
bool classname::m_initialized = false; \
bool classname::RegisterInitDelegator() \
{ \
	if (!m_initialized) \
	{ \
		NtInitDelegator::AddInitDelegator(classname::Init); \
		m_initialized = true; \
	} \
	return m_initialized; \
}

#define NT_REGISTER_INIT_DELEGATE(classname) \
static bool g_InitializeRegister_classname = \
	classname::RegisterInitDelegator();

// Release
#define NT_DECLARE_RELEASE_DELEGATE \
public: \
	static bool RegisterReleaseDelegator(); \
	static void Release(); \
private: \
	static bool m_released;

//
#define NT_IMPLEMENT_RELEASE_DELEGATE(classname) \
bool classname::m_released = false; \
bool classname::RegisterReleaseDelegator() \
{ \
	if (!m_released) \
	{ \
		NtInitDelegator::AddReleaseDelegator(classname::Release); \
		m_released = true; \
	} \
	return m_released; \
}

#define NT_REGISTER_RELEASE_DELEGATE(classname) \
static bool g_ReleaseRegister_classname = \
	classname::RegisterReleaseDelegator();
