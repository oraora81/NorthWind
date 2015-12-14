
#pragma once

#include "NtRTTI.h"

namespace nt {
	
class NtObject
{
public:
	virtual ~NtObject();

	virtual const NtRTTI& GetRTTI() const;
	
	bool IsExactly(const NtRTTI* rtti) const;
	bool IsDerived(const NtRTTI* rtti) const;

	const NtString& GetObjName() const;

	// Factory Func
	typedef NtObjectFactory<NtObject> OBJ_FACTORY;

	static bool RegisterFactory();
	static void InitializeFactory();
	static void TerminateFactory();
	static NtObject* CreateObject(const NtString& obj_name);
	
protected:
	static OBJ_FACTORY* m_factory;
	
private:
	static bool m_setup;

protected:
	NtObject();

public:
	static const NtRTTI m_RTTI;
};

#include "NtObject.inl"

}	// namespace nt

//--------------------------------------------------------------
/**
	
*/

#define NT_DECLARE_OBJECT \
	NT_DECLARE_RTTI \
	static bool RegisterFactory(); \
	static void InitializeFactory(); \
	static NtObject* Factory(); \
private: \
	static bool m_setup;

//--------------------------------------------------------------
/**
*/

#define NT_IMPLEMENT_OBJECT(class_name, parent_name) \
	NT_IMPLEMENT_RTTI(class_name, parent_name) \
bool class_name::m_setup = false; \
bool class_name::RegisterFactory() \
{ \
	if (!m_setup) \
	{ \
		NtInitDelegator::AddInitDelegator(class_name::InitializeFactory); \
	} \
	return m_setup; \
} \
void class_name::InitializeFactory() \
{ \
	if (!m_factory) \
	{ \
		m_factory = new NtObject::OBJ_FACTORY; \
	} \
	m_factory->RegistCreateCallback(m_RTTI.GetObjName(), &class_name::Factory); \
}

//--------------------------------------------------------------
/**
*/

#define NT_IMPLEMENT_ABSTRACT_FACTORY(class_name) \
NtObject* class_name::Factory() \
{ \
	NtAsserte(0 && "abstract class have no factory"); \
	return nullptr; \
}

#define NT_IMPLEMENT_FACTORY(class_name) \
NtObject* class_name::Factory() \
{ \
	class_name* obj = new class_name; \
	return obj; \
}

#define NT_REGISTER_FACTORY(class_name) \
static bool g_registered_##class_name = class_name::RegisterFactory();



#define NT_PREVENT_OBJECT_COPY(class_name) \
private: \
	class_name(const class_name&); \
	class_name& operator=(const class_name&);
