
#pragma once

namespace nt
{

template <typename OBJECT_TYPE>
class NtObjectFactory
{
public:
	typedef OBJECT_TYPE* (*OBJECT_CREATOR_CALLBACK)();

	// regist
	bool RegistCreateCallback(const NtString& object_name, OBJECT_CREATOR_CALLBACK fn)
	{
		NtSetup::Setup();

		// map에 있는 요소는 유니크해야 한다.
		OBJECT_CALLBACK_MAP::iterator itor = m_factory.find(object_name);
		if (itor != m_factory.end())
		{
			return false;
		}

		m_factory.insert(std::make_pair(object_name, fn));
		return true;
	}

	// create
	OBJECT_TYPE* CreateObject(const NtString& object_name)
	{
		OBJECT_CALLBACK_MAP::const_iterator ctor = m_factory.find(object_name);
		if (ctor == m_factory.end())
		{
			// throw exception!
			return nullptr;
		}

		OBJECT_CREATOR_CALLBACK fn = (*ctor).second;

		return (*fn)();
	}

private:
	typedef std::map<NtString, OBJECT_CREATOR_CALLBACK> OBJECT_CALLBACK_MAP;

	OBJECT_CALLBACK_MAP m_factory;
};	
}	// namespace nt
