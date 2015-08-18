


template <typename T>
NtBTree<T>::NtBTree()
: m_root(NULL)
, m_currentNode(NULL)
{

}

template <typename T>
NtBTree<T>::~NtBTree()
{

}

template <typename T>
bool NtBTree<T>::SetRootData(const T& insertData)
{
	if (NULL == m_root)
	{
		m_root = new NtBiNode;
		m_root->m_value = insertData;
		m_root->m_left = NULL;
		m_root->m_right = NULL;
	}
	else
	{
		m_root->m_value = insertData;
	}

	m_currentNode = m_root;

	return true;
}

template <typename T>
bool NtBTree<T>::InsertLeftNode(const T& insertData)
{
	if (NULL == m_currentNode)
	{
		return false;
	}

	if (NULL == m_currentNode->m_left)
	{
		m_currentNode->m_left = CreateNewNode(insertData);
		m_currentNode = m_currentNode->m_left;
	}
	else
	{
		return false;
	}

	return true;
}

template <typename T>
bool NtBTree<T>::InsertRightNode(const T& insertData)
{
	if (NULL == m_currentNode)
	{
		return false;
	}

	if (NULL == m_currentNode->m_right)
	{
		m_currentNode->m_right = CreateNewNode(insertData);
		m_currentNode = m_currentNode->m_right;
	}
	else
	{
		return false;
	}

	return true;
}

template <typename T>
const T* NtBTree<T>::LevelUp()
{
	return NULL;
}

template <typename T>
const T* NtBTree<T>::LevelDown()
{
	return NULL;
}

template <typename T>
const T* NtBTree<T>::NextSibling()
{
	return NULL;
}

template <typename T>
NtTreeNode* NtBTree<T>::CreateNewNode(const T& insertData)
{
	NtBiNode* newNode = new NtBiNode;
	newNode->m_value = insertData;
	newNode->m_left = NULL;
	newNode->m_right = NULL;

	return newNode;
}