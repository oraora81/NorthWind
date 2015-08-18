
template <typename NKEY, typename NVALUE>
NtHashTable<NKEY, NVALUE>::NtHashTable(ntInt tableSize)
{
    assert(tableSize > 0);

    m_tableSize     = tableSize;
    m_currSize      = 0;
    m_currIndex     = 0;
    m_currNode      = NULL;
    m_hashTable     = new NtHashNode* [m_tableSize];
    ZeroMemory(m_hashTable, sizeof(NtHashNode*) * m_tableSize);
}

template <typename NKEY, typename NVALUE>
NtHashTable<NKEY, NVALUE>::~NtHashTable()
{
    Clear();
}

template <typename NKEY, typename NVALUE>
bool NtHashTable<NKEY, NVALUE>::Insert(const NKEY& key, const NVALUE& value)
{
    ntInt index = HashCode(key);
    NtHashNode* node = m_hashTable[index];

    while (node)
    {
        if (node->m_key == key)
        {
            return false;
        }

        node = node->m_nextNode;
    }

    node = new NtHashNode;
    node->m_key         = key;
    node->m_value       = value;
    node->m_nextNode    = m_hashTable[index];
    m_hashTable[index] = node;
    ++m_currSize;

    return true;
}


template <typename NKEY, typename NVALUE>
bool NtHashTable<NKEY, NVALUE>::Erase(const NKEY& key)
{
    ntInt index = HashCode(key);
    NtHashNode* currNode = m_hashTable[index];
    if (!currNode)
    {
        return false;
    }

    if (currNode->m_key == key)
    {
        NtHashNode* tempNode = currNode;
        m_hashTable[index] = currNode->m_nextNode;
        delete tempNode;
        --m_currSize;
        return true;
    }

    NtHashNode* beforeNode = currNode;
    currNode = currNode->m_nextNode;
    while (currNode)
    {
        if (currNode->m_key == key)
        {
            beforeNode->m_nextNode = currNode->m_nextNode;
            delete currNode;
            --m_currSize;

            return true;
        }

        beforeNode = currNode;
        currNode = currNode->m_nextNode;
    }

    return false;
}

template <typename NKEY, typename NVALUE>
void NtHashTable<NKEY, NVALUE>::Clear()
{
    if (Empty())
    {
        return;
    }

    NtHashNode* tempNode;
    NtHashNode* currNode;
    for (ntInt i = 0; i < m_tableSize; ++i)
    {
        currNode = m_hashTable[i];

        while (currNode)
        {
            tempNode = currNode->m_nextNode;
            delete currNode;

            currNode = tempNode;
        }
    }
     
    delete [] m_hashTable;
}

template <typename NKEY, typename NVALUE>
NVALUE* NtHashTable<NKEY, NVALUE>::Find(const NKEY& key)
{
    ntInt index = HashCode(key);
    NtHashNode* currNode = m_hashTable[index];

    while (currNode)
    {
        if (currNode->m_key == key)
        {
            return &currNode->m_value;
        }
        m_currNode = m_currNode->m_nextNode;
    }

    return NULL;
}

template <typename NKEY, typename NVALUE>
NVALUE* NtHashTable<NKEY, NVALUE>::GetFirst(NKEY* key)
{
    if (!Empty())
    {
        for (m_currIndex = 0; m_currIndex < m_tableSize; ++m_currIndex)
        {
            if (m_hashTable[m_currIndex])
            {
                m_currNode = m_hashTable[m_currIndex];
                key = &m_currNode->m_key;

                return &m_currNode->m_value;
            }
        }
    }

    return NULL;
}

template <typename NKEY, typename NVALUE>
NVALUE* NtHashTable<NKEY, NVALUE>::GetNext(NKEY* key)
{
    for (; m_currIndex < m_tableSize; ++m_currIndex)
    {
        if (m_hashTable[m_currNode])
        {
            m_currNode = m_hashTable[m_currIndex];
            key = &m_currNode->m_key;

            return &m_currNode->m_value;
        }
    }
    return NULL;
}

template <typename NKEY, typename NVALUE>
ntInt NtHashTable<NKEY, NVALUE>::Size()
{
    return m_currSize;
}

template <typename NKEY, typename NVALUE>
bool NtHashTable<NKEY, NVALUE>::Empty()
{
    return m_currSize ? false : true;
}

template <typename NKEY, typename NVALUE>
ntInt NtHashTable<NKEY, NVALUE>::HashCode(const NKEY& key)
{
    // default hash function
    static double s_dHashMultiplier = 0.5*(sqrt(5.0)-1.0);
    unsigned ntInt uiKey;
	memcpy(&uiKey, &key, sizeof(ntUint));
    uiKey %= m_tableSize;
    double dFraction = fmod(s_dHashMultiplier*uiKey,1.0);
    return (ntInt)floor(m_tableSize*dFraction);
}
