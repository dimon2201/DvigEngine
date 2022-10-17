#include <cstring>

#include "../include/DvigEngine.hpp"

DvigEngine::HASH_MAP_DATA_ENTRY::HASH_MAP_DATA_ENTRY(DvigEngine::String* key, void* value)
{
    m_Key = STRING_DATA((const char*)(&key->GetData()->m_Chars[0]));
    m_Value = value;
}

DvigEngine::dvuint32 DvigEngine::HashMap::Hash(dvstring input)
{
    dvuint32 hash = 0u;
    dvuchar* string = (dvuchar*)input;
    const dvisize stringSize = String::CharactersCount((dvuchar*)string);
    dvuint32 mask = DV_MEMORY_COMMON_HASH_MAP_TABLE_BYTE_WIDTH / DV_MACRO_ARCH_WORD_BYTE_WIDTH;
    dvuint32 bitsPerEntry = DV_TRAILING_ZEROS_COUNT(mask);
    for (dvisize i = 0; i < stringSize; ++i)
    {
        dvuint32 shift = i & (bitsPerEntry - 8u);
        hash = (hash ^ (string[i] << shift)) & (mask - 1u);
    }

    return hash;
}

void DvigEngine::HashMap::Init()
{
    m_Data.m_LinkedList.Init(0, 2 * sizeof(dvmachword)); // pair (2 address pointers)
}

void DvigEngine::HashMap::Insert(dvstring key, void* value)
{
    dvuint32 hash = HashMap::Hash(key);
    if (m_Data.m_HashTable[hash] == DV_ZERO)
    {
        // Empty slot
        dvmachword pair[2];
        pair[0] = (dvmachword)key;
        pair[1] = (dvmachword)value;
        const dvint32 index = m_Data.m_LinkedList.Insert( (void* const)&pair[0] ); // insert pair
        m_Data.m_HashTable[hash] = index;
    }
    else
    {
        // Possible collision
        // Search through the linked list for key match
        for (dvisize i = 0; i < (dvisize)m_Data.m_LinkedList.GetData()->m_EntryCount; ++i)
        {
            MemoryObject* memoryObject = (MemoryObject*)m_Data.m_LinkedList.Find( i );
            dvmachword* pair = (dvmachword*)memoryObject->GetData()->m_Address;
            dvuchar* chars = (dvuchar*)pair[0];

            if (String::CompareCharacters(key, chars) == DV_TRUE)
            {
                // Just update value for key
                dvmachword pair[2];
                pair[0] = (dvmachword)key;
                pair[1] = (dvmachword)value;
                m_Data.m_LinkedList.Replace( i, &pair[0] ); // update
                m_Data.m_HashTable[hash] = i;
                return;
            }
        }

        // Nothing found
        // Insert new entry
        dvmachword pair[2];
        pair[0] = (dvmachword)key;
        pair[1] = (dvmachword)value;
        const dvint32 index = m_Data.m_LinkedList.Insert( &pair[0] ); // insert pair
        m_Data.m_HashTable[hash] = index;
    }
}

void* DvigEngine::HashMap::Find(dvstring key)
{
    dvuint32 hash = HashMap::Hash(key);
    const dvqword index = m_Data.m_HashTable[hash];

    MemoryObject* memoryObject = (MemoryObject*)m_Data.m_LinkedList.Find( index );
    dvmachword* pair = (dvmachword*)memoryObject->GetData()->m_Address;
    dvuchar* chars = (dvuchar*)pair[0];
    void* value = (void*)pair[1];

    if (String::CompareCharacters(key, chars) == DV_TRUE)
    {
        // First hit exact match
        return (void*)value;
    }
    else
    {
        // Search through the linked list for key match
        for (dvisize i = 0; i < (dvisize)m_Data.m_LinkedList.GetData()->m_EntryCount; ++i)
        {
            MemoryObject* memoryObject = (MemoryObject*)m_Data.m_LinkedList.Find( i );
            dvmachword* pair = (dvmachword*)memoryObject->GetData()->m_Address;
            dvuchar* chars = (dvuchar*)pair[0];
            void* value = (void*)pair[1];

            if (String::CompareCharacters(key, chars) == DV_TRUE)
            {
                // Just update value for key
                return (void*)value;
            }
        }
    }

    return nullptr;
}