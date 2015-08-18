
#pragma once

namespace NT
{
	template <typename T>
	class NtSingleton
	{
	public:
		T* Instance()
		{
			return m_pInstance;
		}

		bool IsInstantiated()
		{
			return m_pInstance ? true : false;
		}

	private:
		static T* m_pInstance;
	};

	template <typename T>
	T* NtSingleton<T>::m_pInstance = NULL;
}	// namespace NT

#define SINGLETON_NEW(OBJ)		NT::NtSingleton<OBJ>::Instance() =  new OBJ
#define SINGLETON_DELETE(OBJ)	delete NT::NtSingleton<OBJ>::Instance()