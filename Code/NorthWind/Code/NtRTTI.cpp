
#include "NtCoreLib.h"

namespace nt {

NtRTTI::NtRTTI(const ntWchar* rtti_name, const NtRTTI* parent)
{
	NtSetup::Setup();

	m_objectName = rtti_name;
	m_parent = parent;
}

NtRTTI::~NtRTTI()
{

}

const NtString&	NtRTTI::GetObjName() const
{
	return m_objectName;
}

void NtRTTI::SetObjName(NtString& name)
{
	m_objectName = name;
}

bool NtRTTI::SetParent(const NtRTTI* rtti)
{
	m_parent = rtti;
	return true;
}

const NtRTTI* NtRTTI::GetParent() const
{
	return m_parent;
}

bool NtRTTI::IsExactly(const NtRTTI* rtti) const
{
	return m_parent == rtti;
}

bool NtRTTI::IsDerived(const NtRTTI* rtti) const 
{
	NtAsserte(this != nullptr);
	NtAsserte(rtti != nullptr);
	const NtRTTI* parent = this;
	while (parent != nullptr)
	{
		if (parent == rtti)
		{
			return true;
		}

		parent = parent->GetParent();
	}

	return false;
}

const NtRTTI* NtRTTI::DynamicCast(const NtRTTI* rtti) const
{
	NtAsserte(this != nullptr);
	NtAsserte(rtti != nullptr);

	const NtRTTI* result = nullptr;
	if (GetParent() == nullptr)
	{
		// �ٿ� ĳ���� �˻� ���� : rtti�� �ڽ�Ŭ������ ������.
		if (nullptr == rtti->GetParent())
		{
			return nullptr;
		}

		if (this == rtti->GetParent())
		{
			return rtti;
		}
		else
		{
			result = DynamicCast(rtti->GetParent());
		}
	}
	else
	{
		// �� ĳ���� �˻� ���� : rtti�� �⺻Ŭ������ ������.
		// ���� Ŭ�����ΰ�?
		if (this == rtti)
		{
			return this;
		}
		else
		{
			result = GetParent()->DynamicCast(rtti);
		}
	}

	return result;
}

}
