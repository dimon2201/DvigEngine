#include "../../include/DECore.hpp"

DvigEngine::HASH_MAP_DATA_ENTRY::HASH_MAP_DATA_ENTRY(DvigEngine::String* key, void* value)
{
    m_Key = key->GetString();
    m_Value = value;
}

void DvigEngine::HASH_MAP_DATA::Init(Engine* engine, HashMap* object)
{

}

DvigEngine::deuint32 DvigEngine::HashMap::Hash(destring input)
{
    deuint32 hash = 0u;
    deuchar* string = (deuchar*)input;
    const deisize stringSize = String::CharactersCount((deuchar*)string);
    deuint32 mask = DV_MEMORY_COMMON_HASH_MAP_TABLE_BYTE_WIDTH / DV_MACRO_ARCH_WORD_BYTE_WIDTH;
    deuint32 bitsPerEntry = DV_TRAILING_ZEROS_COUNT(mask);
    for (deisize i = 0; i < stringSize; ++i)
    {
        deuint32 shift = i & (bitsPerEntry - 8u);
        hash = (hash ^ (string[i] << shift)) & (mask - 1u);
    }

    return hash;
}

void DvigEngine::HashMap::Init()
{   
    // Allocate first memory block
    m_Data.m_MemoryBlocks.Init(2 * sizeof(demachword)); // pair (2 address pointers)
    this->AllocateBlock();

    for (deisize i = 0; i < DV_MEMORY_COMMON_HASH_MAP_TABLE_BYTE_WIDTH; ++i) {
        m_Data.m_HashTable[i] = (demachword)nullptr;
    }
}

void DvigEngine::HashMap::Insert(destring key, void* value)
{
    deuint32 hash = HashMap::Hash(key);
    if (m_Data.m_HashTable[hash] == (demachword)nullptr)
    {
        // Empty slot
        this->InsertToMemoryBlock( hash, key, value );
    }
    else
    {
        // Possible collision
        // Search through the memory blocks for key match
        const deusize memoryBlockEntryCount = m_Data.m_MemoryBlocks.GetEntryCount();
        for (deisize i = 0; i < (deisize)memoryBlockEntryCount; ++i)
        {
            // Run through each memory block entry
            MemoryObject* memoryObject = (MemoryObject*)m_Data.m_MemoryBlocks.Find( (deint32)i );
            HASH_MAP_MEMORY_BLOCK* memoryBlock = memoryObject->Unwrap<HASH_MAP_MEMORY_BLOCK>();
            const deusize entriesPerBlock = memoryBlock->m_EntryCount;

            HASH_MAP_DATA_ENTRY* entries = (HASH_MAP_DATA_ENTRY*)&memoryBlock->m_Memory[0];

            for (deisize j = 0; j < (deisize)entriesPerBlock; ++j)
            {
                deuchar** chars = (deuchar**)&entries[j].m_Key;
                demachword* value = (demachword*)&entries[j].m_Value;
                if (String::CompareCharacters(key, *chars) == DV_TRUE)
                {
                    // Just update value for key
                    MemoryObject* mo = Engine::AllocateObject(1, String::CharactersCount((deuchar*)key));
                    Engine::CopyMemory( mo->GetAddress(), &key[0], String::CharactersCount((deuchar*)key) );
                    *chars = (deuchar*)mo->GetAddress();
                    *value = (demachword)value;
                    m_Data.m_HashTable[hash] = (demachword)&entries[j];
                    return;
                }
            }
        }

        // Nothing found
        // Insert new entry
        this->InsertToMemoryBlock( hash, key, value );
    }
}

void* DvigEngine::HashMap::Find(destring key)
{
    deuint32 hash = HashMap::Hash(key);
    struct Entry { demachword chars; demachword value; };
    HASH_MAP_DATA_ENTRY* entry = (HASH_MAP_DATA_ENTRY*)m_Data.m_HashTable[hash];

    if (entry == nullptr) { return nullptr; }

    // MemoryObject* memoryObject = (MemoryObject*)m_Data.m_LinkedList.Find( index );
    deuchar* chars = (deuchar*)entry->m_Key;
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
        const deusize memoryBlockCount = m_Data.m_MemoryBlocks.GetEntryCount();
        for (deisize i = 0; i < (deisize)memoryBlockCount; ++i)
        {
            // Run through each memory block entry
            MemoryObject* memoryObject = (MemoryObject*)m_Data.m_MemoryBlocks.Find( (deint32)i );
            HASH_MAP_MEMORY_BLOCK* memoryBlock = memoryObject->Unwrap<HASH_MAP_MEMORY_BLOCK>();
            const deusize entriesPerBlock = memoryBlock->m_EntryCount;

            HASH_MAP_DATA_ENTRY* entries = (HASH_MAP_DATA_ENTRY*)&memoryBlock->m_Memory[0];

            for (deisize j = 0; j < (deisize)entriesPerBlock; ++j)
            {
                // demachword* pair = (demachword*)memoryBlock;
                // deuchar* chars = (deuchar*)pair[0];
                deuchar** chars = (deuchar**)&entries[j].m_Key;
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

void* DvigEngine::HashMap::FindIndex(const deint32 index)
{
    deint32 searchIndex = index;
    if (searchIndex < 0) { return nullptr; }

    // Search through the memory blocks for index match
    deint32 cycle = 0;
    const deusize memoryBlockCount = m_Data.m_MemoryBlocks.GetEntryCount();
    for (deisize i = 0; i < (deisize)memoryBlockCount; ++i)
    {
        // Run through each memory block entry
        MemoryObject* memoryObject = (MemoryObject*)m_Data.m_MemoryBlocks.Find( (deint32)i );
        HASH_MAP_MEMORY_BLOCK* memoryBlock = memoryObject->Unwrap<HASH_MAP_MEMORY_BLOCK>();
        const deusize entriesPerBlock = memoryBlock->m_EntryCount;

        HASH_MAP_DATA_ENTRY* entries = (HASH_MAP_DATA_ENTRY*)&memoryBlock->m_Memory[0];

        for (deisize j = 0; j < (deisize)entriesPerBlock; ++j, ++cycle)
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

void DvigEngine::HashMap::InsertToMemoryBlock(deuint32 hash, destring key, void* value)
{
    MemoryObject* mo = Engine::AllocateObject(1, String::CharactersCount((deuchar*)key));
    Engine::CopyMemory( mo->GetAddress(), &key[0], String::CharactersCount((deuchar*)key) );

    deint32 memoryBlockCount = (const deint32)m_Data.m_MemoryBlocks.GetEntryCount();
    MemoryObject* memoryObject = (MemoryObject*)m_Data.m_MemoryBlocks.Find( memoryBlockCount - 1 );
    HASH_MAP_MEMORY_BLOCK* memoryBlock = memoryObject->Unwrap<HASH_MAP_MEMORY_BLOCK>();
    deuint32 memoryBlockEntryCount = memoryBlock->m_EntryCount;
    
    // Check if memory block is full
    // Then create new memory block
    struct Entry { demachword chars; void* value; };
    HASH_MAP_DATA_ENTRY* entries = (HASH_MAP_DATA_ENTRY*)&memoryBlock->m_Memory[0];
    HASH_MAP_DATA_ENTRY* startAddress = &entries[0];
    HASH_MAP_DATA_ENTRY* writeToAddress = &entries[ memoryBlockEntryCount ];
    HASH_MAP_DATA_ENTRY* aheadAddress = &entries[ memoryBlockEntryCount + 1 ];
    const demachword occupancy = (demachword)aheadAddress - (demachword)startAddress;
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
    
    m_Data.m_HashTable[hash] = (demachword)writeToAddress;
}