
#pragma once

namespace NT
{

template <typename NKEY, typename NVALUE>
class NtHashTable
{
public:
    explicit NtHashTable(int tableSize);
    ~NtHashTable();

    bool    Insert(const NKEY& key, const NVALUE& value);
    bool    Erase(const NKEY& key);
    void    Clear();

    NVALUE* Find(const NKEY& key);

    NVALUE* GetFirst(NKEY* key);
    NVALUE* GetNext(NKEY* key);

    int     Size();
    bool    Empty();

private:
    int     HashCode(const NKEY& key);

private:
    class NtHashNode
    {
    public:
        NtHashNode()
            : m_nextNode(NULL)
        {

        }

        NtHashNode* m_nextNode;
        NKEY       m_key;
        NVALUE     m_value;
    };

    NtHashNode**     m_hashTable;
    int             m_tableSize;
    int             m_currSize;
    int             m_currIndex;
    NtHashNode*      m_currNode;
};

#include "NtHashTable.inl"

} // namespace NT