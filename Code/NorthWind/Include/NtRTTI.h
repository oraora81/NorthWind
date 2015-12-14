
#pragma once

namespace nt {

class NtRTTI
{
public:
	NtRTTI(const ntWchar* rtti_name, const NtRTTI* parent);
	virtual ~NtRTTI();

	const NtString&		GetObjName() const;
	void				SetObjName(NtString& name);

	bool				SetParent(const NtRTTI* rtti);
	const NtRTTI*		GetParent() const;

	bool				IsExactly(const NtRTTI* rtti) const;
	bool				IsDerived(const NtRTTI* rtti) const;
	const NtRTTI*		DynamicCast(const NtRTTI* rtti) const;

private:
	NtString		m_objectName;
	const NtRTTI*	m_parent;
};

}

#define NT_DECLARE_RTTI \
public: \
	static const NtRTTI m_RTTI; \
	virtual const NtRTTI& GetRTTI() const \
	{ \
		return m_RTTI; \
	}

#define NT_IMPLEMENT_RTTI(class_name, parent_class) \
	const NtRTTI class_name::m_RTTI(L#class_name, &parent_class::m_RTTI);
