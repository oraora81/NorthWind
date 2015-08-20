
#pragma once

#include "NtNode.h"

namespace nt
{

/*
 *	NtNormalTree
 */
template <typename T>
class NtTree
{
public:
	NtTree()
		: m_root(NULL)
		, m_currentNode(NULL)
	{

	}

	~NtTree()
	{

	}

	bool SetRoot(const T& insertData)
	{
		m_root = new NtNNode<T>;
		m_root->m_level = 0;
		m_root->m_value = insertData;
		m_root->m_siblingNode = NULL;
		m_root->m_nextNode = NULL;

		m_currentNode = m_root;
	}

	bool AddLeaf(NtNNode* parent, const T& insertData)
	{
		if (NULL != parent->m_nextNode)
		{
			return false;
		}

		NtNNode* temp = new NtNNode<T>;
		temp->m_level = parent->m_level;
		++temp->m_level;
		temp->m_nextNode = NULL;
		temp->m_siblingNode = NULL;
		temp->m_value = insertData;

		parent->m_nextNode = temp;

		return true;
	}

	bool AddSibling(NtNNode* siblingNode, const T& insertData)
	{
		if (NULL != siblingNode->m_siblingNode)
		{
			return false;
		}

		NtNNode* temp = new NtNNode<T>;
		temp->m_level = siblingNode->m_level;
		temp->m_nextNode = NULL;
		temp->m_siblingNode = NULL;
		temp->m_value = insertData;

		siblingNode->m_siblingNode = temp;

		return true;
	}

private:
	NtNNode*	m_root;
	NtNNode*	m_currentNode;
};


/*
 *
 */
template <typename T>
class NtBTree
{
public:
	NtBTree();
	~NtBTree();	

	bool SetRootData(const T& insertData);
	bool InsertLeftNode(const T& insertData);
	bool InsertRightNode(const T& insertData);

	const T* LevelUp();
	const T* LevelDown();
	const T* NextSibling();

	void PreOrder();
	void InOrder();
	void PostOrder();

private:
	NtBiNode* CreateNewNode(const T& insertData);

private:
	NtBiNode*	m_root;
	NtBiNode*	m_currentNode;
};

#include "NtTree.inl"

}	// namespace nt
