
#pragma once

namespace nt {

template <typename T>
class NtList
{
public:
    NtList();
    NtList(const NtList& rhsList);
    ~NtList();

    void Push_back(const T& item);

private:
    class NtListNode
    {
    public:
        NtListNode()
            : m_nextNode(NULL)
        {
        }

        T             m_value;
        NtListNode*  m_nextNode;
    };

    NtListNode* m_nodeList;
};

#include "NtList.inl"

} // namespace nt
