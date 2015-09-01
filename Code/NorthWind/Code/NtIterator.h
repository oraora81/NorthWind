
#pragma once


// 
// NtIterator
//----------------------------------------------------------------------------

//template<class _Category,
//class _Ty,
//class _Pointer = _Ty *,
//class _Reference = _Ty&>
//struct iterator
//{	// base type for iterator classes
//	typedef _Category iterator_category;
//	typedef _Ty value_type;
//	typedef _Pointer pointer;
//	typedef _Reference reference;
//};


template<class _Iter>
struct iterator_traits
{	// get traits from iterator _Iter
	typedef typename _Iter::iterator_category iterator_category;
	typedef typename _Iter::value_type value_type;
	typedef typename _Iter::pointer pointer;
	typedef typename _Iter::reference reference;
};


namespace nt {

// 
// NtIterator
//----------------------------------------------------------------------------
template <typename CONT>
class NtIterator
{
public:
	typedef typename NtIterator<CONT> MyType;
	typedef typename CONT::value_type value_type;
	typedef typename CONT::reference reference;
	typedef typename CONT::pointer pointer;
};


// 
// NtListIterator : NtList에서 사용하는 Iterator
//----------------------------------------------------------------------------

template <typename MyList>
class NtListIterator : NtIterator<MyList>
{
	typedef typename MyList::NodePtr NodePtr;
	typedef NtListIterator<MyList> MyItor;

public:
	NtListIterator()
	{

	}

	NtListIterator(const NodePtr node)
	{
		m_current = node;
	}

	NtListIterator(const MyItor& right)
	{
		m_current = right.m_current;
	}

	reference operator*() const
	{
		return m_current->m_value;
	}

	pointer operator->() const
	{
		return m_current->m_value;
	}

	MyItor& operator++()
	{
		m_current = m_current->m_next;
		return *this;
	}

	MyItor operator++(int)
	{
		NodePtr temp = m_current;
		m_current = m_current->m_next;
		return NtListIterator(temp);
	}

	bool operator==(const MyItor& right) const
	{
		return m_current == right.m_current;
	}

	bool operator!=(const MyItor& right) const
	{
		return m_current != right.m_current;
	}

private:
	NodePtr m_current;
};


} // namespace nt
