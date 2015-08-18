
#pragma once

namespace NT
{

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

	static NtInt m_initCount;
	static InitDelegator m_initDelegatorList[MAX_DELEGATOR_COUNT];

	static NtInt m_releaseCount;
	static ReleaseDelegator m_releaseDelegatorList[MAX_DELEGATOR_COUNT];
};

}	// namespace NT

// Init
#define NT_DECLARE_INIT_DELEGATE \
public: \
	static NtBool RegisterInitDelegator(); \
	static void Init(); \
private: \
	static NtBool m_initialized;

//
#define NT_IMPLEMENT_INIT_DELEGATE(classname) \
NtBool classname::m_initialized = false; \
NtBool classname::RegisterInitDelegator() \
{ \
	if (!m_initialized) \
	{ \
		NtInitDelegator::AddInitDelegator(classname::Init); \
		m_initialized = true; \
	} \
	return m_initialized; \
}

#define NT_REGISTER_INIT_DELEGATE(classname) \
static NtBool g_InitializeRegister_classname = \
	classname::RegisterInitDelegator();

// Release
#define NT_DECLARE_RELEASE_DELEGATE \
public: \
	static NtBool RegisterReleaseDelegator(); \
	static void Release(); \
private: \
	static NtBool m_released;

//
#define NT_IMPLEMENT_RELEASE_DELEGATE(classname) \
NtBool classname::m_released = false; \
NtBool classname::RegisterReleaseDelegator() \
{ \
	if (!m_released) \
	{ \
		NtInitDelegator::AddReleaseDelegator(classname::Release); \
		m_released = true; \
	} \
	return m_released; \
}

#define NT_REGISTER_RELEASE_DELEGATE(classname) \
static NtBool g_ReleaseRegister_classname = \
	classname::RegisterReleaseDelegator();
