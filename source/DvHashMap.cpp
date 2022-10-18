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
    // Allocate first memory block
    m_Data.m_MemoryBlocks.Init(2 * sizeof(dvmachword)); // pair (2 address pointers)
    MemoryObject* memoryBlock = Engine::AllocateObject( 0, DV_MAX_HASH_MAP_MEMORY_BLOCK_BYTE_WIDTH );
    m_Data.m_MemoryBlocks.Insert( (void*)memoryBlock );
    void* usefulData = memoryBlock->GetData()->m_Address;
    *((dvuint32*)usefulData) = 0;

    for (dvisize i = 0; i < DV_MEMORY_COMMON_HASH_MAP_TABLE_BYTE_WIDTH; ++i) {
        m_Data.m_HashTable[i] = (dvmachword)nullptr;
    }
}

void DvigEngine::HashMap::Insert(dvstring key, void* value)
{
    dvuint32 hash = HashMap::Hash(key);
    if (m_Data.m_HashTable[hash] == (dvmachword)nullptr)
    {
        // Empty slot
        this->InsertToMemoryBlock( hash, key, value );
    }
    else
    {
        // Possible collision
        // Search through the memory blocks for key match
        const dvusize memoryBlockEntryCount = m_Data.m_MemoryBlocks.GetData()->m_EntryCount;
        for (dvisize i = 0; i < (dvisize)memoryBlockEntryCount; ++i)
        {
            // Run through each memory block entry
            void* memoryBlock = ((MemoryObject*)m_Data.m_MemoryBlocks.Find( (dvint32)i ))->GetData()->m_Address;
            const dvusize entriesPerBlock = *((dvuint32*)memoryBlock);
            memoryBlock = (void*)((dvmachword)memoryBlock + 4);

            for (dvisize j = 0; j < (dvisize)entriesPerBlock; ++j)
            {
                dvmachword* pair = (dvmachword*)memoryBlock;
                dvuchar* chars = (dvuchar*)pair[0];

                if (String::CompareCharacters(key, chars) == DV_TRUE)
                {
                    // Just update value for key
                    dvmachword newPair[2];
                    newPair[0] = (dvmachword)key;
                    newPair[1] = (dvmachword)value;
                    pair[0] = newPair[0];
                    pair[1] = newPair[1];
                    m_Data.m_HashTable[hash] = i;
                    return;
                }

                memoryBlock = (void*)((dvmachword)memoryBlock + m_Data.m_MemoryBlocks.GetData()->m_EntryValueByteWidth);
            }
        }

        // Nothing found
        // Insert new entry
        this->InsertToMemoryBlock( hash, key, value );
    }
}

void* DvigEngine::HashMap::Find(dvstring key)
{
    dvuint32 hash = HashMap::Hash(key);
    void* readFromAddress = (void*)m_Data.m_HashTable[hash];

    if (readFromAddress == nullptr) { return nullptr; }

    // MemoryObject* memoryObject = (MemoryObject*)m_Data.m_LinkedList.Find( index );
    dvmachword* pair = (dvmachword*)readFromAddress;
    dvuchar* chars = (dvuchar*)pair[0];
    void* value = (void*)pair[1];

    if (String::CompareCharacters(key, chars) == DV_TRUE)
    {
        // First hit exact match
        return (void*)value;
    }
    else
    {
        // Possible collision
        // Search through the memory blocks for key match
        const dvusize memoryBlockEntryCount = m_Data.m_MemoryBlocks.GetData()->m_EntryCount;
        for (dvisize i = 0; i < (dvisize)memoryBlockEntryCount; ++i)
        {
            // Run through each memory block entry
            void* memoryBlock = ((MemoryObject*)m_Data.m_MemoryBlocks.Find( (dvint32)i ))->GetData()->m_Address;
            const dvusize entriesPerBlock = *((dvuint32*)memoryBlock);
            memoryBlock = (void*)((dvmachword)memoryBlock + 4);

            for (dvisize j = 0; j < (dvisize)entriesPerBlock; ++j)
            {
                dvmachword* pair = (dvmachword*)memoryBlock;
                dvuchar* chars = (dvuchar*)pair[0];
                void* value = (void*)pair[1];

                if (String::CompareCharacters(key, chars) == DV_TRUE)
                {
                    // Return proper value
                    return value;
                }

                memoryBlock = (void*)((dvmachword)memoryBlock + m_Data.m_MemoryBlocks.GetData()->m_EntryValueByteWidth);
            }
        }
    }

    return nullptr;
}

void DvigEngine::HashMap::InsertToMemoryBlock(dvuint32 hash, dvstring key, void* value)
{
    dvmachword pair[2];
    pair[0] = (dvmachword)key;
    pair[1] = (dvmachword)value;

    dvint32 memoryBlockCount = (const dvint32)m_Data.m_MemoryBlocks.GetData()->m_EntryCount;
    dvusize memoryBlockEntryByteWidth = m_Data.m_MemoryBlocks.GetData()->m_EntryValueByteWidth;
    MemoryObject* memoryBlock = (MemoryObject*)m_Data.m_MemoryBlocks.Find( memoryBlockCount - 1 );
    void* usefulData = memoryBlock->GetData()->m_Address;
    dvuint32 memoryBlockEntryCount = *((dvuint32*)usefulData);
    
    // Check if memory block is full
    // Then create new memory block
    if (memoryBlockEntryCount * memoryBlockEntryByteWidth >= DV_MAX_HASH_MAP_MEMORY_BLOCK_BYTE_WIDTH)
    {
        memoryBlock = Engine::AllocateObject( 0, DV_MAX_HASH_MAP_MEMORY_BLOCK_BYTE_WIDTH );
        m_Data.m_MemoryBlocks.Insert( memoryBlock );
        usefulData = memoryBlock->GetData()->m_Address;
        *((dvuint32*)usefulData) = 0;
    }

    // Write pair to proper memory block
    dvmachword* writeToAddress = (dvmachword*)((dvmachword)usefulData + 4 + (memoryBlockEntryCount * memoryBlockEntryByteWidth));
    writeToAddress[0] = pair[0];
    writeToAddress[1] = pair[1];
    *((dvuint32*)usefulData) += 1;

    m_Data.m_HashTable[hash] = (dvmachword)writeToAddress;
}