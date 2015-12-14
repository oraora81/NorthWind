
#pragma once

namespace nt {

template <typename T>
ntIndex BinarySearch(const T& dataSet, ntSize size, const T target)
{
	ntIndex left = 0;
	ntIndex right = size - 1;
	ntIndex mid = 0;

	while(left <= right)
	{
		mid = (left + right) / 2;

		if (target == dataSet[mid])
		{
			return mid;
		}
		else if (target > dataSet[mid])
		{
			left = mid + 1;
		}
		else
		{
			right = mid - 1;
		}
	}

	return -1;
}

template <typename T>
struct BSTNode
{
	BSTNode*	left;
	BSTNode*	right;
	T			data;
};

/**
  *		@brief : NorthWind Binary Search Tree
  */
template <typename T>
class NtBSTree
{
	typedef T ElementType;
public:
	NtBSTree();
	explicit NtBSTree(ElementType data);

	BSTNode<T>*	InsertRoot(ElementType data);
	BSTNode<T>*	InsertNode(BSTNode<T>* parent, BSTNode<T>* node);
	BSTNode<T>*	DeleteNode(ElementType data);
	BSTNode<T>*	DeleteNode(BSTNode<T>* target, BSTNode<T>* parent, ElementType data);
	void		Clear(BSTNode<T>* node);

	BSTNode<T>*	SearchNode(ElementType target) const;
	BSTNode<T>*	SearchNode(BSTNode<T>* node, ElementType target) const;
	BSTNode<T>*	SearchMinNode(BSTNode<T>* node) const;

	template <typename Fn>
	BSTNode<T>*	SearchNode(Fn funcObject, BSTNode<T>* node, ElementType target);


	BSTNode<T>*	GetRoot() const;

#ifdef _DEBUG
	template <typename Fn>
	void		InOrderPrintTree(Fn printObject, BSTNode<T>* node);
#endif // _DEBUG

	BSTNode<T>*	CreateNode(ElementType data);

private:
	BSTNode<T>*	m_root;
};


#include "NtSearch.inl"

}	// namespace nt
