
#pragma once

namespace NT
{

template <typename T>
struct NtNode
{
	T	m_value;
};

// NormalNode
template <typename T>
struct NtNNode : public NtNode<T>
{
	NtIndex m_level;
	NtNNode* m_nextNode;
	NtNNode* m_siblingNode;
};

template <typename T>
struct NtBiNode : public NtNode<T>
{
	NtBiNode*	m_left;
	NtBiNode*	m_right;
};

}	// namespace NT