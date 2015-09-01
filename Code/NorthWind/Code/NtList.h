
#pragma once

#include "NtNode.h"
#include "NtIterator.h"

namespace nt {

template <typename T>
class NtList
{
public:
	typedef T value_type;
	typedef NtListNode<T> NodeType;
	typedef	NtListNode<T>* NodePtr;
	typedef value_type& reference;
	typedef value_type* pointer;

	typedef typename nt::NtListIterator<NtList<T> > Iterator;
	typedef const typename nt::NtListIterator<NtList<T> > const_iterator;

public:
	NtList()
	{
		m_first = new NodeType();
		m_last = new NodeType();

		m_first->m_prev = m_first;
		m_first->m_next = m_last;

		m_last->m_prev = m_first;
		m_last->m_next = m_last;
	}


	NtList(const NtList& rhsList)
	{

	}

	~NtList()
	{
		clear();

		SAFE_DELETE(m_last);
		SAFE_DELETE(m_first);
	}

	//
	void clear()
	{
		NodePtr currNode = m_first->m_next;
		while (currNode != m_last)
		{
			NodePtr nextNode = currNode->m_next;
			SAFE_DELETE(currNode);

			currNode = nextNode;
		}
	}

	//
	void push_back(const T& item)
	{
		NodePtr newNode = new NodeType();
		newNode->m_value = item;

		newNode->m_next = m_last;
		m_last->m_prev->m_next = newNode;

		newNode->m_prev = m_last->m_prev;
		m_last->m_prev = newNode;
	}


	void push_front(const T& item)
	{
		NodePtr newNode = new NodePtr();
	}

	Iterator begin()
	{
		return Iterator(m_first->m_next);
	}

	const_iterator begin() const
	{
		return Iterator(m_first->m_next);
	}

	Iterator end()
	{
		return Iterator(m_last);
	}
	
	const_iterator end() const
	{
		return Iterator(m_last);
	}

private:
	NodePtr m_first;
	NodePtr m_last;
	ntSize m_size;
};

} // namespace nt
