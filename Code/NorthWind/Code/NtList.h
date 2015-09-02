
#pragma once

#include "NtNode.h"
#include "NtIterator.h"

namespace nt {

template <typename T>
class NtList
{
public:
	typedef T value_type;
	typedef NtList<T> MyList;
	typedef NtListNode<T> NodeType;
	typedef	NtListNode<T>* NodePtr;
	typedef value_type& reference;
	typedef value_type* pointer;

	typedef typename nt::NtListIterator<NtList<T> > Iterator;
	typedef const typename nt::NtListIterator<NtList<T> > const_iterator;

public:
	NtList()
	{
		Initialize();
	}


	NtList(const MyList& rhs)
	{
		Initialize();

		*this = rhs;
	}

	NtList(MyList&& rhs)
	{
		Initialize();

		*this = std::move(rhs);
	}

	~NtList()
	{
		clear();

		SAFE_DELETE(m_last);
		SAFE_DELETE(m_first);
	}

	void Initialize()
	{
		m_first = new NodeType();
		m_last = new NodeType();

		m_first->m_prev = m_first;
		m_first->m_next = m_last;

		m_last->m_prev = m_first;
		m_last->m_next = m_last;

		m_size = 0;
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

		// reconnect m_first <-> m_last
		m_first->m_next = m_last;
		m_last->m_prev = m_first;
	}

	//
	void push_back(T&& item)
	{
		NodePtr newNode = new NodeType();
		newNode->m_value = item;

		InsertBack(newNode);
		++m_size;
	}
	//
	void push_back(const T& item)
	{
		NodePtr newNode = new NodeType();
		newNode->m_value = item;

		InsertBack(newNode);
		++m_size;
	}

private:
	void InsertBack(NodePtr& node)
	{
		node->m_next = m_last;
		m_last->m_prev->m_next = node;

		node->m_prev = m_last->m_prev;
		m_last->m_prev = node;
	}

public:
	void push_front(T&& item)
	{
		NodePtr newNode = new NodePtr();
		newNode->m_value = item;

		InsertFront(newNode);
		++m_size;
	}

	void push_front(const T& item)
	{
		NodePtr newNode = new NodePtr();
		newNode->m_value = item;

		InsertFront(newNode);
		++m_size;
	}
private:
	void InsertFront(NodePtr& node)
	{
		node->m_prev = m_first;
		m_first->m_next->m_prev = node;

		node->m_next = m_first->m_next;
		m_first->m_next = node;
	}

public:
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

	MyList& operator=(const MyList& rhs)
	{
		if (this == &rhs)
		{
			return *this;
		}

		clear();
		
		NodePtr currNode = rhs.m_first->m_next;
		while (currNode != rhs.m_last)
		{
			NodePtr newNode = new NodeType();
			newNode->m_value = std::forward<T>(currNode->m_value);

			InsertBack(newNode);
			++m_size;

			currNode = currNode->m_next;
		}

		return *this;
	}

	MyList& operator=(MyList&& rhs)
	{
		if (this == &rhs)
		{
			return *this;
		}

		clear();
		
		NodePtr currNode = rhs.m_first->m_next;
		while (currNode != rhs.m_last)
		{
			NodePtr newNode = new NodeType();
			newNode->m_value = std::forward<T>(currNode->m_value);

			InsertBack(newNode);
			++m_size;

			currNode = currNode->m_next;
		}
		
		return *this;
	}

private:
	NodePtr m_first;
	NodePtr m_last;
	//
	ntSize m_size;
};

} // namespace nt
