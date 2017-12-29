
#pragma once

namespace nt
{

// 
template <typename T1, typename T2>
class NtDuet
{
	typedef T1 Type1;
	typedef T2 Type2;
public:
	NtDuet()
		: first()
		, second()
	{

	}

	NtDuet(const T1& type1, const T2& type2)
		: first(t1)
		, second(t2)
	{

	}

	Type1 first;
	Type2 second;
};

template <typename T1, typename T2>
inline NtDuet<T1, T2> make_duet(const T1& type1, const T2& type2)
{
	return NtDuet<T1, T2>(type1, type2);
}

} // namespace nt
