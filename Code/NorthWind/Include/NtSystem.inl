
template <typename T>
void AllocateMem(int column, int row, T** allocArray)
{
    int columnSize = column;
    int rowSize = row;
    int allSize = columnSize * rowSize;

    allocArray = new T* [rowSize];
    allocArray[0] = new T [allSize];
    ZeroMemory(allocArray, allSize * sizeof(T));

    for (int i = 1; i < rowSize; ++i)
    {
        allocArray[i] = &allocArray[0][rowSize * i];
    }
}

template <typename T>
void DeAllocateMem(T**& allocArray)
{
    delete [] allocArray[0];
    delete [] allocArray;
    allocArray = NULL;
}

