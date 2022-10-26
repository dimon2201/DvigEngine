#include "../../include/DECore.hpp"

DvigEngine2::deuint32 DvigEngine2::HashMap::Hash(const destring input)
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

void DvigEngine2::HashMap::Init(const deusize entryValueByteWidth)
{   
    m_Prop.m_Capacity = 0;
    m_Prop.m_EntryValueByteWidth = entryValueByteWidth;
    m_Prop.m_Entries.Init( m_Prop.m_EntryValueByteWidth );
    for (deisize i = 0; i < DV_MEMORY_COMMON_HASH_MAP_TABLE_SIZE; ++i)
    {
        m_Prop.m_HashTable[ i ] = 0;
    }
}

DvigEngine2::deint32 DvigEngine2::HashMap::Insert(const char* key, void* value)
{
    const deuint32 hash = HashMap::Hash( (deuchar*)key );
    deint32 entryIndex = (deint32)this->GetData()->m_HashTable[ hash ];

    HashMapKeyValuePair* foundPair = this->GetData()->m_Entries.Find<HashMapKeyValuePair*>( entryIndex );
    if (foundPair == nullptr)
    {
        // Set does not have our value
        // Insert value to the set
        const deusize keyByteWidth = String::CharactersCount((deuchar*)&key[0]);
        HashMapKeyValuePair* insertedPair = Engine::Allocate( 0, sizeof(HashMapKeyValuePair) )->Unwrap<HashMapKeyValuePair*>();
        Engine::CopyMemory( &insertedPair->m_Key[0], &key[0], keyByteWidth );
        insertedPair->m_Value = value;
        std::cout << "ins : " << (demachword)insertedPair << std::endl;
        entryIndex = this->GetData()->m_Entries.Insert( (void*)&insertedPair );
        HashMapKeyValuePair* p = this->GetData()->m_Entries.Find<HashMapKeyValuePair*>( entryIndex );
        std::cout << p->m_Key << std::endl;

        // Update hash table
        this->GetData()->m_HashTable[ hash ] = entryIndex;
        
        return entryIndex;
    }

    deuchar* compPairKeyAddress = (deuchar*)&foundPair->m_Key[0];
    deuchar* compKeyAddress = (deuchar*)key;
    if ( String::CompareCharacters( compPairKeyAddress, compKeyAddress, this->GetData()->m_EntryValueByteWidth ) == DV_FALSE)
    {
        // Possible collision
        // Check if set contains our value
        const deint32 foundEntryIndex = this->GetData()->m_Entries.FindValue( foundPair );
        if (foundEntryIndex == DV_NULL)
        {
            // Set does not contain value
            // Insert value to the set
            const deusize keyByteWidth = String::CharactersCount((deuchar*)key);
            HashMapKeyValuePair* insertedPair = Engine::Allocate( 0, sizeof(HashMapKeyValuePair) )->Unwrap<HashMapKeyValuePair*>();
            Engine::CopyMemory( &insertedPair->m_Key[0], &key[0], keyByteWidth );
            insertedPair->m_Value = value;
            entryIndex = this->GetData()->m_Entries.Insert( (void*)&insertedPair );
        }
        else
        {
            entryIndex = foundEntryIndex;
        }
    }

    // Update hash table
    this->GetData()->m_HashTable[ hash ] = entryIndex;

    return entryIndex;
}

void* DvigEngine2::HashMap::Find(const char* key)
{
    const deuint32 hash = HashMap::Hash( (deuchar*)key );
    deint32 entryIndex = (deint32)this->GetData()->m_HashTable[ hash ];
    std::cout << entryIndex << std::endl;
    HashMapKeyValuePair* foundPair = *this->GetData()->m_Entries.Find<HashMapKeyValuePair**>( entryIndex );
    if (foundPair == nullptr) { return nullptr; }
    std::cout << "get pair : " << (demachword)foundPair << std::endl;

    deuchar* compPairKeyAddress = (deuchar*)&foundPair->m_Key[0];
    deuchar* compKeyAddress = (deuchar*)key;
    std::cout << foundPair->m_Value << " " << compKeyAddress << std::endl;
    if ( String::CompareCharacters( compPairKeyAddress, compKeyAddress, this->GetData()->m_EntryValueByteWidth ) == DV_FALSE)
    {
        // Possible collision
        // Check if set contains our value
        const deint32 foundEntryIndex = this->GetData()->m_Entries.FindValue( foundPair );
        if (foundEntryIndex == DV_NULL)
        {
            return nullptr;
        }
        else
        {
            HashMapKeyValuePair* pair = this->GetData()->m_Entries.Find<HashMapKeyValuePair*>( foundEntryIndex );
            return pair->m_Value;
        }
    }
    std::cout << "y" << std::endl;

    return foundPair->m_Value;
}

// void DvigEngine::HashMap::Insert(destring key, void* value)
// {
//     deuint32 hash = HashMap::Hash(key);
//     if (m_Data.m_HashTable[hash] == (demachword)nullptr)
//     {
//         // Empty slot
//         this->InsertToMemoryBlock( hash, key, value );
//     }
//     else
//     {
//         // Possible collision
//         // Search through the memory blocks for key match
//         const deusize memoryBlockEntryCount = m_Data.m_MemoryBlocks.GetEntryCount();
//         for (deisize i = 0; i < (deisize)memoryBlockEntryCount; ++i)
//         {
//             // Run through each memory block entry
//             MemoryObject* memoryObject = (MemoryObject*)m_Data.m_MemoryBlocks.Find( (deint32)i );
//             HASH_MAP_MEMORY_BLOCK* memoryBlock = memoryObject->Unwrap<HASH_MAP_MEMORY_BLOCK>();
//             const deusize entriesPerBlock = memoryBlock->m_EntryCount;

//             HASH_MAP_DATA_ENTRY* entries = (HASH_MAP_DATA_ENTRY*)&memoryBlock->m_Memory[0];

//             for (deisize j = 0; j < (deisize)entriesPerBlock; ++j)
//             {
//                 deuchar** chars = (deuchar**)&entries[j].m_Key;
//                 demachword* value = (demachword*)&entries[j].m_Value;
//                 if (String::CompareCharacters(key, *chars) == DV_TRUE)
//                 {
//                     // Just update value for key
//                     MemoryObject* mo = Engine::ObjectAllocate(1, String::CharactersCount((deuchar*)key));
//                     Engine::CopyMemory( mo->GetAddress(), &key[0], String::CharactersCount((deuchar*)key) );
//                     *chars = (deuchar*)mo->GetAddress();
//                     *value = (demachword)value;
//                     m_Data.m_HashTable[hash] = (demachword)&entries[j];
//                     return;
//                 }
//             }
//         }

//         // Nothing found
//         // Insert new entry
//         this->InsertToMemoryBlock( hash, key, value );
//     }
// }

// void* DvigEngine::HashMap::Find(const destring key)
// {
//     deuint32 hash = HashMap::Hash(key);
//     struct Entry { demachword chars; demachword value; };
//     HASH_MAP_DATA_ENTRY* entry = (HASH_MAP_DATA_ENTRY*)m_Data.m_HashTable[hash];

//     if (entry == nullptr) { return nullptr; }

//     // MemoryObject* memoryObject = (MemoryObject*)m_Data.m_LinkedList.Find( index );
//     const deuchar* chars = (const deuchar*)entry->m_Key;
//     void* value = (void*)entry->m_Value;

//     // std::cout << "chars : " << chars << " " << key << std::endl;

//     if (String::CompareCharacters(key, chars) == DV_TRUE)
//     {
//         // First hit exact match
//         return (void*)value;
//     }
//     else
//     {
//         // Possible collision
//         // Search through the memory blocks for key match
//         const deusize memoryBlockCount = m_Data.m_MemoryBlocks.GetEntryCount();
//         for (deisize i = 0; i < (deisize)memoryBlockCount; ++i)
//         {
//             // Run through each memory block entry
//             MemoryObject* memoryObject = (MemoryObject*)m_Data.m_MemoryBlocks.Find( (deint32)i );
//             HASH_MAP_MEMORY_BLOCK* memoryBlock = memoryObject->Unwrap<HASH_MAP_MEMORY_BLOCK>();
//             const deusize entriesPerBlock = memoryBlock->m_EntryCount;

//             HASH_MAP_DATA_ENTRY* entries = (HASH_MAP_DATA_ENTRY*)&memoryBlock->m_Memory[0];

//             for (deisize j = 0; j < (deisize)entriesPerBlock; ++j)
//             {
//                 // demachword* pair = (demachword*)memoryBlock;
//                 // deuchar* chars = (deuchar*)pair[0];
//                 deuchar** chars = (deuchar**)&entries[j].m_Key;
//                 void** value = (void**)&entries[j].m_Value;

//                 if (String::CompareCharacters(key, *chars) == DV_TRUE)
//                 {
//                     // Just return a value
//                     return *value;
//                 }
//             }
//         }
//     }

//     return nullptr;
// }

// void* DvigEngine::HashMap::FindIndex(const deint32 index)
// {
//     deint32 searchIndex = index;
//     if (searchIndex < 0) { return nullptr; }

//     // Search through the memory blocks for index match
//     deint32 cycle = 0;
//     const deusize memoryBlockCount = m_Data.m_MemoryBlocks.GetEntryCount();
//     for (deisize i = 0; i < (deisize)memoryBlockCount; ++i)
//     {
//         // Run through each memory block entry
//         MemoryObject* memoryObject = (MemoryObject*)m_Data.m_MemoryBlocks.Find( (deint32)i );
//         HASH_MAP_MEMORY_BLOCK* memoryBlock = memoryObject->Unwrap<HASH_MAP_MEMORY_BLOCK>();
//         const deusize entriesPerBlock = memoryBlock->m_EntryCount;

//         HASH_MAP_DATA_ENTRY* entries = (HASH_MAP_DATA_ENTRY*)&memoryBlock->m_Memory[0];

//         for (deisize j = 0; j < (deisize)entriesPerBlock; ++j, ++cycle)
//         {
//             void** value = (void**)&entries[j].m_Value;

//             if (searchIndex == cycle)
//             {
//                 // Just return a value
//                 return *value;
//             }
//         }
//     }

//     return nullptr;
// }

// DvigEngine::MemoryObject* DvigEngine::HashMap::AllocateBlock()
// {
//     MemoryObject* memoryObject = Engine::ObjectAllocate( 1, sizeof(HASH_MAP_MEMORY_BLOCK) );
//     HASH_MAP_MEMORY_BLOCK* hashMapMemoryBlock = memoryObject->Unwrap<HASH_MAP_MEMORY_BLOCK>();
//     m_Data.m_MemoryBlocks.Insert( (void*)memoryObject );
//     hashMapMemoryBlock->m_EntryCount = 0;
    
//     return memoryObject;
// }

// void DvigEngine::HashMap::InsertToMemoryBlock(deuint32 hash, destring key, void* value)
// {
//     MemoryObject* mo = Engine::ObjectAllocate(1, String::CharactersCount((deuchar*)key));
//     Engine::CopyMemory( mo->GetAddress(), &key[0], String::CharactersCount((deuchar*)key) );

//     deint32 memoryBlockCount = (const deint32)m_Data.m_MemoryBlocks.GetEntryCount();
//     MemoryObject* memoryObject = (MemoryObject*)m_Data.m_MemoryBlocks.Find( memoryBlockCount - 1 );
//     HASH_MAP_MEMORY_BLOCK* memoryBlock = memoryObject->Unwrap<HASH_MAP_MEMORY_BLOCK>();
//     deuint32 memoryBlockEntryCount = memoryBlock->m_EntryCount;
    
//     // Check if memory block is full
//     // Then create new memory block
//     struct Entry { demachword chars; void* value; };
//     HASH_MAP_DATA_ENTRY* entries = (HASH_MAP_DATA_ENTRY*)&memoryBlock->m_Memory[0];
//     HASH_MAP_DATA_ENTRY* startAddress = &entries[0];
//     HASH_MAP_DATA_ENTRY* writeToAddress = &entries[ memoryBlockEntryCount ];
//     HASH_MAP_DATA_ENTRY* aheadAddress = &entries[ memoryBlockEntryCount + 1 ];
//     const demachword occupancy = (demachword)aheadAddress - (demachword)startAddress;
//     // std::cout << occupancy << std::endl;
//     if (occupancy >= DV_MAX_HASH_MAP_MEMORY_BLOCK_BYTE_WIDTH)
//     {
//         memoryObject = this->AllocateBlock();
//         memoryBlock = memoryObject->Unwrap<HASH_MAP_MEMORY_BLOCK>();
//         memoryBlock->m_EntryCount = 0;
//         memoryBlockEntryCount = 0;
//         writeToAddress = (HASH_MAP_DATA_ENTRY*)&memoryBlock->m_Memory[0];
//     }

//     // Write pair to proper memory block
//     writeToAddress->m_Key = mo->GetAddress();
//     writeToAddress->m_Value = value;
//     memoryBlock->m_EntryCount += 1;
    
//     m_Data.m_HashTable[hash] = (demachword)writeToAddress;
// }