#include "NtCoreLib.h"


namespace NT
{

const NtRTTI NtObject::m_RTTI(L"NtObject", nullptr);

NT::NtObjectFactory<NtObject>* NtObject::m_factory = nullptr;
bool NtObject::m_setup = false;

NtObject::NtObject()
{
	
}

NtObject::~NtObject()
{

}

/*virtual*/ const NtRTTI& NtObject::GetRTTI() const
{
	return m_RTTI;
}

bool NtObject::IsExactly(const NtRTTI* rtti) const
{
	return GetRTTI().IsExactly(rtti);
}


bool NtObject::IsDerived(const NtRTTI* rtti) const
{
	return GetRTTI().IsDerived(rtti);
}


const NtString& NtObject::GetObjName() const
{
	return GetRTTI().GetObjName();
}


bool NtObject::RegisterFactory()
{
	if (!m_setup)
	{
		NtInitDelegator::AddInitDelegator(NtObject::InitializeFactory);
		NtInitDelegator::AddReleaseDelegator(NtObject::TerminateFactory);
		m_setup = true;
	}

	return m_setup;
}

void NtObject::InitializeFactory()
{
	if (!m_factory)
	{
		m_factory = new	OBJ_FACTORY;
	}

	m_factory->RegistCreateCallback(m_RTTI.GetObjName(), nullptr);
}

void NtObject::TerminateFactory()
{
	SAFE_DELETE(m_factory);
}


NtObject* NtObject::CreateObject(const NtString& obj_name)
{
	if (!m_factory)
	{
		return nullptr;
	}

	return m_factory->CreateObject(obj_name);
}

}