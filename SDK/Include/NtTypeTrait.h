
#pragma once

namespace NT
{

template <typename T>
class TypeTraits
{

// for Pointer
private:
	template <typename U>
	struct PointerTrait
	{
		enum { result = false };
		typedef NtNullType PointeeType;
	};

	template <typename U>
	struct PointerTrait<U*>
	{
		enum { result = true };
		typedef U PointeeType;
	};

public:
	enum { isPointee = PointerTrait<T>::result };
	typedef typename PointerTrait<T>::PointeeType PointeeType;

// for reference
private:
	template <typename U>
	struct ReferenceTrait
	{
		enum { result = false };
		typedef NtNullType ReferencedType;
	};

	template <typename U>
	struct ReferenceTrait<U&>
	{
		enum { result = true };
		typedef U ReferencedType;
	};

public:
	enum { isReference = ReferenceTrait<T>::result };
	typedef typename ReferenceTrait<T>::ReferencedType ReferencedType;

// for member function
private:
	template <typename U>
	struct MemberPointerTrait
	{
		enum { result = false };
		typedef NtNullType MemberPointerType;
	};

	template <typename U, typename V>
	struct MemberPointerTrait<U V::*>
	{
		enum { result = true };
		typedef U MemberPointerType;
	};

public:
	enum { isMemberPointer = MemberPointerTrait<T>::result };
	typedef typename MemberPointerTrait<T>::MemberPointerType MemberPointerType;
};


//
template <typename T, typename U>
class Conversion
{
	typedef char Small;
	class Big
	{
		char dummy[2];
	};
	static Small Test(const U&);
	static Big Test(...);
	static T MakeT();

public:
	enum { exist = sizeof(Test(MakeT())) == sizeof(Small) };
	enum { sameType = false };
};

template <typename T>
class Conversion<T, T>
{
public:
	enum { exist = 1, sameType = 1 };
};



//
template <typename T, typename U>
struct NtTypeList
{
	typedef T Head;
	typedef U Tail;
};

template <typename T>
struct NtTypeList<T, NtNullType>
{
	typedef T Head;
	typedef typename NtNullType Tail;
};


}	// namespace NT