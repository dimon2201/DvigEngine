#include <cstring>

#include "../include/DvigEngine.hpp"

DvigEngine::HASH_MAP_DATA_ENTRY::HASH_MAP_DATA_ENTRY(DvigEngine::String* key, void* value)
{
    m_Key = STRING_DATA((const char*)(&key->GetData()->m_Chars[0]));
    m_Value = value;
}

DvigEngine::dvuint32 DvigEngine::HashMap::Hash(STRING_DATA* input)
{
    dvuint32 hash = 0u;
    dvuchar* string = input->m_Chars;
    dvusize stringSize = input->m_ByteWidth;
    // dvuchar* string = (dvuchar*)input;
    // const dvusize stringSize = String::CharactersCount((dvuchar*)string);
    dvuint32 mask = DV_MEMORY_COMMON_HASH_MAP_TABLE_BYTE_WIDTH / DV_MACRO_ARCH_WORD_BYTE_WIDTH;
    dvuint32 bitsPerEntry = DV_TRAILING_ZEROS_COUNT(mask);
    for (dvisize i = 0; i < stringSize; ++i)
    {
        dvuint32 shift = i & (bitsPerEntry - 8u);
        hash = (hash ^ (string[i] << shift)) & (mask - 1u);
    }

    return hash;
}

void DvigEngine::HashMap::Init(void* const assocAddress, const dvusize assocEntryByteWidth)
{
    m_Data.m_AssocAddress = assocAddress;
    m_Data.m_AssocEntryByteWidth = assocEntryByteWidth;
    m_Data.m_LinkedList.Init(0);
}

void DvigEngine::HashMap::Insert(STRING_DATA* key, void* value)
{
    dvuint32 hash = HashMap::Hash(key);
    if (m_Data.m_HashTable[hash] == DV_ZERO)
    {
        // Empty slot
        const dvint32 index = m_Data.m_LinkedList.Insert( (MemoryObject*)key->m_Chars ); // insert value to linked list
        m_Data.m_HashTable[hash] = (dvqword)((index & 0xFFFFFFFF) | (((dvqword)value) << 32));
    }
    else
    {
        // Possible collision
        STRING_DATA* keyData = key;
        const dvint32 index = m_Data.m_LinkedList.FindValue( (MemoryObject*)value );

        if (index != DV_NULL)
        {
            // Just update value for key
            m_Data.m_HashTable[hash] = (dvusize)value;
            m_Data.m_LinkedList.Replace( index, (MemoryObject*)value );
            return;
        }

        // Collision create new entry
        m_Data.m_HashTable[hash] = (dvusize)value;
        m_Data.m_LinkedList.Insert( (MemoryObject*)value );

        // for (dvisize i = 0; i < m_Data.m_ListEntryCount; ++i)
        // {
        //     ICommon* ptrAsCommon = (ICommon*)((dvmachword)m_Data.m_AssocAddress + (m_Data.m_AssocEntryByteWidth * i));
        //     if (String::CompareCharacters(ptrAsCommon->GetSID(), keyData->m_Chars) == DV_TRUE)
        //     {
        //         // Just update value for key
        //         m_Data.m_HashTable[hash] = (dvusize)value;
        //         return;
        //     }
        // }

        // // Collision create new entry
        // m_Data.m_HashTable[hash] = (dvusize)value;
    }
}

void* DvigEngine::HashMap::Find(STRING_DATA* key)
{
    dvuint32 hash = HashMap::Hash(key);
    STRING_DATA* keyData = key;
    const dvqword pair = m_Data.m_HashTable[hash];
    const dvdword index = pair & 0xFFFFFFFF;
    const dvdword value = pair >> 32;

    dvuchar* foundChars = (dvuchar*)m_Data.m_LinkedList.Find( index );

    if (String::CompareCharacters(foundChars, keyData->m_Chars) == DV_TRUE)
    {
        // Just update value for key
        return (void*)(dvmachword)value;
    }
    else
    {
        for (dvisize i = 0; i < m_Data.m_LinkedList.GetData()->m_EntryCount; ++i)
        {
            MemoryObject* value = m_Data.m_LinkedList.Find( i );
            ICommon* ptrAsCommon = (ICommon*)((dvmachword)m_Data.m_AssocAddress + (m_Data.m_AssocEntryByteWidth * i));
            if (String::CompareCharacters(ptrAsCommon->GetSID(), keyData->m_Chars) == DV_TRUE)
            {
                return (void*)ptrAsCommon;
            }
        }
        // for (dvisize i = 0; i < m_Data.m_ListEntryCount; ++i)
        // {
        //     ICommon* ptrAsCommon = (ICommon*)((dvmachword)m_Data.m_AssocAddress + (m_Data.m_AssocEntryByteWidth * i));
        //     if (String::CompareCharacters(ptrAsCommon->GetSID(), keyData->m_Chars) == DV_TRUE)
        //     {
        //         return (void*)ptrAsCommon;
        //     }
        // }
    }

    return nullptr;
}