
template <typename T>
NtBSTree<T>::NtBSTree()
: m_root(NULL)
{

}

template <typename T>
NtBSTree<T>::NtBSTree(ElementType data)
: m_root(NULL)
{
	m_root = CreateNode(data);
}

template <typename T>
BSTNode<T>* NtBSTree<T>::InsertRoot(ElementType data)
{
	if (m_root == NULL)
	{
		m_root = CreateNode(data);
	}

	return InsertNode(NULL, m_root);
}

template <typename T>
BSTNode<T>* NtBSTree<T>::InsertNode(BSTNode<T>* parent, BSTNode<T>* node)
{
	if (NULL == parent)
	{
		parent = node;
		return node;
	}

	if (parent->data < node->data)
	{
		if (NULL == parent->right)
		{
			parent->right = node;
			return node;
		}
		else
		{
			InsertNode(parent->right, node);
		}
	}
	else
	{
		if (parent->data > node->data)
		{
			if (NULL == parent->left)
			{
				parent->left = node;
				return node;
			}
			else
			{
				InsertNode(parent->left, node);
			}
		}
	}

	return NULL;
}

template <typename T>
BSTNode<T>* NtBSTree<T>::DeleteNode(ElementType data)
{
	if (NULL == parent)
	{
		return NULL;
	}

	return DeleteNode(m_root, NULL, data);
}


template <typename T>
BSTNode<T>* NtBSTree<T>::DeleteNode(BSTNode<T>* target, BSTNode<T>* parent, ElementType data)
{
	if (NULL == target)
	{
		return NULL;
	}

	BSTNode<T>* removeNode = NULL;

	if (target->data > data)
	{
		removeNode = DeleteNode(target->left, target, data);
	}
	else
	{
		if (target->data < data)
		{
			removeNode = DeleteNode(target->right, target, data);
		}
		else
		{
			removeNode = target;

			if (NULL == target->left && NULL == target->right)
			{
				if (parent->left == target)
				{
					parent->left = NULL;
				}
				else
				{
					parent->right = NULL;
				}
			}
			else
			{
				// 양쪽 다 자식이 있는 경우
				if (NULL != target->left && NULL != target->right)
				{
					BSTNode<T>* minNode = SearchMinNode(target->right);
					minNode = DeleteNode(target, NULL, minNode->data);
					target->data = minNode->data;
				}
				else
				{
					// 자식이 하나만 있는 경우
					BSTNode<T>* temp = NULL;
					if (NULL != target->left)
					{
						temp = target->left;
					}
					else
					{
						temp = target->right;
					}

					if (parent->left == target)
					{
						parent->left = temp;
					}
					else
					{
						parent->right = temp;
					}
				}
			}
		}
	}

	return removeNode;
}


template <typename T>
void NtBSTree<T>::Clear(BSTNode<T>* node)
{
	if (NULL == node)
	{
		return;
	}

	if (NULL != node->right)
	{
		Clear(node->right);
	}

	if (NULL != node->left)
	{
		Clear(node->left);
	}

	node->left = NULL;
	node->right = NULL;

	Safe_Delete(node);
}

template <typename T>
BSTNode<T>* NtBSTree<T>::SearchNode(ElementType target) const
{
	if (NULL == m_root)
	{
		return NULL;
	}

	return SearchNode(m_root, target);
}

template <typename T>
BSTNode<T>* NtBSTree<T>::SearchNode(BSTNode<T>* node, ElementType target) const
{
	if (NULL == node)
	{
		return NULL;
	}

	if (node->data < target)
	{
		return SearchNode(node->left, target);
	}
	else
	{
		if (node->data > target)
		{
			return SearchNode(node->right, target);
		}
		else
		{
			return node;
		}
	}

	return NULL;
}

template <typename T>
BSTNode<T>* NtBSTree<T>::SearchMinNode(BSTNode<T>* node) const
{
	if (NULL == node)
	{
		return NULL;
	}

	if (NULL == node->left)
	{
		return node;
	}
	else
	{
		return SearchMinNode(node->left);
	}

	return NULL;
}

template <typename T>
template <typename Fn>
BSTNode<T>* NtBSTree<T>::SearchNode(Fn funcObject, BSTNode<T>* node, ElementType target)
{
	if (NULL == node)
	{
		return NULL;
	}

	int result = funcObject(node->data, target);
	if (result < 0)
	{
		return SearchNode(funcObject, node->left, target);
	}
	else
	{
		if (result > 0)
		{
			return SearchNode(funcObject, node->right, target);
		}
		else
		{
			return node;
		}
	}

	return NULL;
}

template <typename T>
BSTNode<T>* NtBSTree<T>::GetRoot() const
{
	return m_root;
}

#ifdef _DEBUG
template <typename T>
template <typename Fn>
void NtBSTree<T>::InOrderPrintTree(Fn printObject, BSTNode<T>* node)
{
	if (NULL == node)
	{
		return;
	}

	InOrderPrintTree(printObject, node->left);

	printObject(node->data);

	InOrderPrintTree(printObject, node->right);
}
#endif // _DEBUG

template <typename T>
BSTNode<T>* NtBSTree<T>::CreateNode(ElementType data)
{
	BSTNode<T>* node = new BSTNode<T>;
	node->data = data;
	node->left = NULL;
	node->right = NULL;

	return node;
}