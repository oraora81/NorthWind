
#include "stdafx.h"

int WINAPI WinMain(__in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in_opt LPSTR lpCmdLine, __in int nShowCmd )
{
    NHashTable<char, int> hashTable(5);
    
    hashTable.Insert('a', 1);

    return 1;
}