
#pragma once

namespace nt
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
}	// namespace nt

#define SINGLETON_NEW(OBJ)		nt::NtSingleton<OBJ>::Instance() =  new OBJ
#define SINGLETON_DELETE(OBJ)	delete nt::NtSingleton<OBJ>::Instance()
