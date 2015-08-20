
#pragma once

namespace nt
{

template <typename NKEY, typename NVALUE>
class NtHashTable
{
public:
    explicit NtHashTable(int tableSize);
    ~NtHashTable();

    bool	Insert(const NKEY& key, const NVALUE& value);
	bool	Erase(const NKEY& key);
    void    Clear();

    NVALUE* Find(const NKEY& key);
    NVALUE* GetFirst(NKEY* key);
    NVALUE* GetNext(NKEY* key);

    ntInt	Size();
    bool	Empty();

private:
    ntInt     HashCode(const NKEY& key);

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

	
	ntInt			m_tableSize;
	ntInt			m_currSize;
	ntInt			m_currIndex;
    NtHashNode**    m_hashTable;
    NtHashNode*     m_currNode;
};

#include "NtHashTable.inl"

} // namespace nt
