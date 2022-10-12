#include <cstring>

#include "../include/DvigEngine.h"

DvigEngine::HASH_MAP_DATA_SLOT::HASH_MAP_DATA_SLOT(DvigEngine::String* key, void* value)
{
    m_Key = STRING_DATA((const char*)(&key->GetData()->m_Chars[0]));
    m_Value = value;
}

DvigEngine::dvuint32 DvigEngine::HashMap::Hash(String* input)
{
    dvuint32 hash = 0u;
    dvuchar* string = ((DvigEngine::STRING_DATA*)input->GetData())->m_Chars;
    dvusize stringSize = ((DvigEngine::STRING_DATA*)input->GetData())->m_ByteWidth;
    dvuint32 mask = DV_MEMORY_COMMON_HASH_MAP_TABLE_BYTE_WIDTH / DV_MACRO_ARCH_WORD_BYTE_WIDTH;
    dvuint32 bitsPerEntry = DV_TRAILING_ZEROS_COUNT(mask);
    for (dvisize i = 0; i < stringSize; ++i)
    {
        dvuint32 shift = i & (bitsPerEntry - 8u);
        hash = (hash ^ (string[i] << shift)) & (mask - 1u);
    }

    return hash;
}

void DvigEngine::HashMap::Insert(String* key, void* value)
{
    dvuint32 hash = HashMap::Hash(key);
    if (m_Data.m_HashTable[hash] == DV_NULL)
    {
        //void* cursor = (void*)((dvmachword)m_Data.m_AssocAddress + (m_Data.m_AssocEntrySize * m_Data.m_ListEntryCount));
        //m_Data.m_List[m_Data.m_ListEntryCount++] = HASH_MAP_DATA_SLOT(key, value);
        m_Data.m_HashTable[hash] = (dvusize)value;//m_Data.m_ListEntryCount;
    }
    else
    {
        // Possible collision
        STRING_DATA* keyData = key->GetData();
        for (dvisize i = 0; i < m_Data.m_ListEntryCount; ++i)
        {
            // if (DvigEngine::String::Compare(&m_Data.m_List[i].m_Key, keyData) == DV_TRUE)
            // {
            //     // Just update value for key
            //     m_Data.m_List[i].m_Value = value;
            //     return;
            // }
            ICommon* ptrAsCommon = (ICommon*)((dvmachword)m_Data.m_AssocAddress + (m_Data.m_AssocEntrySize * i));
            dvisize cursor = 0;
            while (ptrAsCommon->m_SID[cursor] == keyData->m_Chars[cursor] && cursor++ < keyData->m_ByteWidth);
            if (cursor == keyData->m_ByteWidth)
            {
                // Just update value for key
                // m_Data.m_List[i].m_Value = value;
                m_Data.m_HashTable[hash] = (dvusize)value;//m_Data.m_ListEntryCount;
                return;
            }
        }

        // Collision create new entry
        // m_Data.m_List[m_Data.m_ListEntryCount++] = HASH_MAP_DATA_SLOT(key, value);
        m_Data.m_HashTable[hash] = (dvusize)value;//m_Data.m_ListEntryCount;
    }
}

void* DvigEngine::HashMap::Find(String* key)
{
    dvuint32 hash = HashMap::Hash(key);
    STRING_DATA* keyData = key->GetData();
    // STRING_DATA* opCompare = &m_Data.m_List[m_Data.m_HashTable[hash] - 1].m_Key;
    ICommon* assocPtr = (ICommon*)(m_Data.m_HashTable[hash]);
    dvisize cursor = 0;
    std::cout<<assocPtr->m_SID[cursor]<<" "<<assocPtr->m_SID[1]<<" "<<assocPtr->m_SID[2]<<std::endl;
    while (assocPtr->m_SID[cursor] == keyData->m_Chars[cursor] && ++cursor < keyData->m_ByteWidth){
        std::cout<<assocPtr->m_SID[cursor]<<std::endl;
    }
    if (cursor == keyData->m_ByteWidth)
    {
        // Just update value for key
        return (void*)assocPtr;
    }
    else
    {
        for (dvisize i = 0; i < m_Data.m_ListEntryCount; ++i)
        {
            ICommon* ptrAsCommon = (ICommon*)((dvmachword)m_Data.m_AssocAddress + (m_Data.m_AssocEntrySize * i));
            dvisize cursor = 0;
            while (ptrAsCommon->m_SID[cursor] == keyData->m_Chars[cursor] && cursor++ < keyData->m_ByteWidth);
            if (cursor == keyData->m_ByteWidth)
            {
                return (void*)ptrAsCommon;
            }
        }
    }
    // if (DvigEngine::String::Compare(opCompare, keyData) == DV_TRUE)
    // {
    //     return m_Data.m_List[m_Data.m_HashTable[hash] - 1].m_Value;
    // }
    // else
    // {
    //     for (dvisize i = 0; i < m_Data.m_ListEntryCount; ++i)
    //     {
    //         if (DvigEngine::String::Compare(&m_Data.m_List[i].m_Key, keyData) == DV_TRUE)
    //         {
    //             return m_Data.m_List[i].m_Value;
    //         }
    //     }
    // }

    return nullptr;
}