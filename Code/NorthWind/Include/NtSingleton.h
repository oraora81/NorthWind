
#pragma once

#include <mutex>

namespace nt {
	
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



template <typename T>
class NtDynSingleton
{
public:
	T& Instance()
	{
		std::call_once(ms_flag, []() 
		{
			ms_inst.reset(new T);
		});

		return *(ms_inst.get());
	}

private:
	static std::unique_ptr<T> ms_inst;
	static std::once_flag ms_flag;
};

template <typename T>
std::unique_ptr<T> NtDynSingleton<T>::ms_inst;

template <typename T>
std::once_flag NtDynSingleton<T>::ms_flag;

}	// namespace nt

#define SINGLETON_NEW(OBJ)		nt::NtSingleton<OBJ>::Instance() =  new OBJ
#define SINGLETON_DELETE(OBJ)	delete nt::NtSingleton<OBJ>::Instance()

