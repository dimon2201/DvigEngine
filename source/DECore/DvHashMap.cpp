#include <cstring>

#include "../../include/DvigEngine.hpp"

DvigEngine::HASH_MAP_DATA_ENTRY::HASH_MAP_DATA_ENTRY(DvigEngine::String* key, void* value)
{
    m_Key = key->GetString();
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
    this->AllocateBlock();

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
        const dvusize memoryBlockEntryCount = m_Data.m_MemoryBlocks.GetEntryCount();
        for (dvisize i = 0; i < (dvisize)memoryBlockEntryCount; ++i)
        {
            // Run through each memory block entry
            MemoryObject* memoryObject = (MemoryObject*)m_Data.m_MemoryBlocks.Find( (dvint32)i );
            HASH_MAP_MEMORY_BLOCK* memoryBlock = memoryObject->Unwrap<HASH_MAP_MEMORY_BLOCK>();
            const dvusize entriesPerBlock = memoryBlock->m_EntryCount;

            HASH_MAP_DATA_ENTRY* entries = (HASH_MAP_DATA_ENTRY*)&memoryBlock->m_Memory[0];

            for (dvisize j = 0; j < (dvisize)entriesPerBlock; ++j)
            {
                dvuchar** chars = (dvuchar**)&entries[j].m_Key;
                dvmachword* value = (dvmachword*)&entries[j].m_Value;
                if (String::CompareCharacters(key, *chars) == DV_TRUE)
                {
                    // Just update value for key
                    MemoryObject* mo = Engine::AllocateObject(1, String::CharactersCount((dvuchar*)key));
                    Engine::CopyMemory( mo->GetAddress(), &key[0], String::CharactersCount((dvuchar*)key) );
                    *chars = (dvuchar*)mo->GetAddress();
                    *value = (dvmachword)value;
                    m_Data.m_HashTable[hash] = (dvmachword)&entries[j];
                    return;
                }
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
    struct Entry { dvmachword chars; dvmachword value; };
    HASH_MAP_DATA_ENTRY* entry = (HASH_MAP_DATA_ENTRY*)m_Data.m_HashTable[hash];

    if (entry == nullptr) { return nullptr; }

    // MemoryObject* memoryObject = (MemoryObject*)m_Data.m_LinkedList.Find( index );
    dvuchar* chars = (dvuchar*)entry->m_Key;
    void* value = (void*)entry->m_Value;

    // std::cout << "chars : " << chars << " " << key << std::endl;

    if (String::CompareCharacters(key, chars) == DV_TRUE)
    {
        // First hit exact match
        return (void*)value;
    }
    else
    {
        // Possible collision
        // Search through the memory blocks for key match
        const dvusize memoryBlockCount = m_Data.m_MemoryBlocks.GetEntryCount();
        for (dvisize i = 0; i < (dvisize)memoryBlockCount; ++i)
        {
            // Run through each memory block entry
            MemoryObject* memoryObject = (MemoryObject*)m_Data.m_MemoryBlocks.Find( (dvint32)i );
            HASH_MAP_MEMORY_BLOCK* memoryBlock = memoryObject->Unwrap<HASH_MAP_MEMORY_BLOCK>();
            const dvusize entriesPerBlock = memoryBlock->m_EntryCount;

            HASH_MAP_DATA_ENTRY* entries = (HASH_MAP_DATA_ENTRY*)&memoryBlock->m_Memory[0];

            for (dvisize j = 0; j < (dvisize)entriesPerBlock; ++j)
            {
                // dvmachword* pair = (dvmachword*)memoryBlock;
                // dvuchar* chars = (dvuchar*)pair[0];
                dvuchar** chars = (dvuchar**)&entries[j].m_Key;
                void** value = (void**)&entries[j].m_Value;

                if (String::CompareCharacters(key, *chars) == DV_TRUE)
                {
                    // Just return a value
                    return *value;
                }
            }
        }
    }

    return nullptr;
}

void* DvigEngine::HashMap::FindIndex(const dvint32 index)
{
    dvint32 searchIndex = index;
    if (searchIndex < 0) { return nullptr; }

    // Search through the memory blocks for index match
    dvint32 cycle = 0;
    const dvusize memoryBlockCount = m_Data.m_MemoryBlocks.GetEntryCount();
    for (dvisize i = 0; i < (dvisize)memoryBlockCount; ++i)
    {
        // Run through each memory block entry
        MemoryObject* memoryObject = (MemoryObject*)m_Data.m_MemoryBlocks.Find( (dvint32)i );
        HASH_MAP_MEMORY_BLOCK* memoryBlock = memoryObject->Unwrap<HASH_MAP_MEMORY_BLOCK>();
        const dvusize entriesPerBlock = memoryBlock->m_EntryCount;

        HASH_MAP_DATA_ENTRY* entries = (HASH_MAP_DATA_ENTRY*)&memoryBlock->m_Memory[0];

        for (dvisize j = 0; j < (dvisize)entriesPerBlock; ++j, ++cycle)
        {
            void** value = (void**)&entries[j].m_Value;

            if (searchIndex == cycle)
            {
                // Just return a value
                return *value;
            }
        }
    }

    return nullptr;
}

DvigEngine::MemoryObject* DvigEngine::HashMap::AllocateBlock()
{
    MemoryObject* memoryObject = Engine::AllocateObject( 1, sizeof(HASH_MAP_MEMORY_BLOCK) );
    HASH_MAP_MEMORY_BLOCK* hashMapMemoryBlock = memoryObject->Unwrap<HASH_MAP_MEMORY_BLOCK>();
    m_Data.m_MemoryBlocks.Insert( (void*)memoryObject );
    hashMapMemoryBlock->m_EntryCount = 0;
    
    return memoryObject;
}

void DvigEngine::HashMap::InsertToMemoryBlock(dvuint32 hash, dvstring key, void* value)
{
    MemoryObject* mo = Engine::AllocateObject(1, String::CharactersCount((dvuchar*)key));
    Engine::CopyMemory( mo->GetAddress(), &key[0], String::CharactersCount((dvuchar*)key) );

    dvint32 memoryBlockCount = (const dvint32)m_Data.m_MemoryBlocks.GetEntryCount();
    MemoryObject* memoryObject = (MemoryObject*)m_Data.m_MemoryBlocks.Find( memoryBlockCount - 1 );
    HASH_MAP_MEMORY_BLOCK* memoryBlock = memoryObject->Unwrap<HASH_MAP_MEMORY_BLOCK>();
    dvuint32 memoryBlockEntryCount = memoryBlock->m_EntryCount;
    
    // Check if memory block is full
    // Then create new memory block
    struct Entry { dvmachword chars; void* value; };
    HASH_MAP_DATA_ENTRY* entries = (HASH_MAP_DATA_ENTRY*)&memoryBlock->m_Memory[0];
    HASH_MAP_DATA_ENTRY* startAddress = &entries[0];
    HASH_MAP_DATA_ENTRY* writeToAddress = &entries[ memoryBlockEntryCount ];
    HASH_MAP_DATA_ENTRY* aheadAddress = &entries[ memoryBlockEntryCount + 1 ];
    const dvmachword occupancy = (dvmachword)aheadAddress - (dvmachword)startAddress;
    // std::cout << occupancy << std::endl;
    if (occupancy >= DV_MAX_HASH_MAP_MEMORY_BLOCK_BYTE_WIDTH)
    {
        memoryObject = this->AllocateBlock();
        memoryBlock = memoryObject->Unwrap<HASH_MAP_MEMORY_BLOCK>();
        memoryBlock->m_EntryCount = 0;
        memoryBlockEntryCount = 0;
        writeToAddress = (HASH_MAP_DATA_ENTRY*)&memoryBlock->m_Memory[0];
    }

    // Write pair to proper memory block
    writeToAddress->m_Key = mo->GetAddress();
    writeToAddress->m_Value = value;
    memoryBlock->m_EntryCount += 1;
    
    m_Data.m_HashTable[hash] = (dvmachword)writeToAddress;
}