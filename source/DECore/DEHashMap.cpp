#include "../../include/DECore.hpp"

DvigEngine::deuint32 DvigEngine::HashMap::Hash(const destring input, const demachword bitMask)
{
    deuint32 hash = 0u;
    deuchar* string = (deuchar*)input;
    const deisize stringSize = String::CharactersCount((deuchar*)string);
    deuint32 bitsPerEntry = DV_TRAILING_ZEROS_COUNT(bitMask);
    for (deisize i = 0; i < stringSize; ++i)
    {
        deuint32 shift = i & (bitsPerEntry - 8u);
        hash = (hash ^ (string[i] << shift)) & (bitMask - 1u);
    }

    return hash;
}

DvigEngine::deuint32 DvigEngine::HashMap::HashMurMur(const destring input, const demachword bitMask)
{
    const demachuint m = 0x5bd1e995;
    const demachuint seed = 0;
    const demachint r = 24;

    demachuint len = String::CharactersCount(input);
    demachuint h = seed ^ len;

    const deuchar* data = (const deuchar*)input;
    demachuint k = 0;

    while (len >= 4)
    {
        k  = data[0];
        k |= data[1] << 8;
        k |= data[2] << 16;
        k |= data[3] << 24;

        k *= m;
        k ^= k >> r;
        k *= m;

        h *= m;
        h ^= k;

        data += 4;
        len -= 4;
    }

    switch (len)
    {
        case 3:
        h ^= data[2] << 16;
        case 2:
        h ^= data[1] << 8;
        case 1:
        h ^= data[0];
        h *= m;
    };

    h ^= h >> 13;
    h *= m;
    h ^= h >> 15;

    return h & bitMask;
}

void DvigEngine::HashMap::Init(const deint32 memoryPoolIndex, const deusize reservedCapacity, const deusize entryValueByteWidth, const deusize hashTableSize)
{
    this->m_Capacity = 0;
    this->m_EntryValueByteWidth = entryValueByteWidth;
    this->m_Entries.Init( memoryPoolIndex, reservedCapacity, sizeof(HashMapKeyValuePair) );
    this->m_HashTableSize = hashTableSize;
    this->m_HashTable = Engine::Allocate( memoryPoolIndex, sizeof(demachword) * hashTableSize )->Unwrap<demachword*>();
    for (deisize i = 0; i < (deisize)m_HashTableSize; ++i)
    {
        this->m_HashTable[ i ] = 0;
    }
    this->m_MemoryPoolIndex = memoryPoolIndex;
}

void DvigEngine::HashMap::Free()
{
    DvigEngine::Engine* engine = this->GetEngine();
    this->m_Entries.Free();
    DvigEngine::MemoryObject* tableMemoryObject = this->GetHashTableMemoryObject();
    engine->Delete( tableMemoryObject );
}

DvigEngine::deint32 DvigEngine::HashMap::Insert(const char* key, void* value)
{
    const deuint32 hash = HashMap::Hash( (deuchar*)key, (this->m_HashTableSize - 1) );
    deint32 entryIndex = (deint32)this->m_HashTable[ hash ];

    const deusize keyByteWidth = String::CharactersCount((deuchar*)&key[0]);
    HashMapKeyValuePair insertedPair;
    DvigEngine::Engine::MemorySet( &insertedPair.m_Key[0], 0, DV_MEMORY_COMMON_STRING_BYTE_WIDTH );
    DvigEngine::Engine::MemoryCopy( &insertedPair.m_Key[0], &key[0], keyByteWidth );
    insertedPair.m_Key[keyByteWidth] = 0;
    insertedPair.m_Value = value;

    HashMapKeyValuePair* foundPair = this->m_Entries.Find<HashMapKeyValuePair*>( entryIndex );
    if (foundPair == nullptr)
    {
        // Set does not have our value
        // Insert value to the set
        entryIndex = this->m_Entries.Insert( (void*)&insertedPair );

        // Update hash table
        this->m_HashTable[ hash ] = entryIndex;
        
        return entryIndex;
    }

    const char* compPairKeyAddress = (const char*)&foundPair->m_Key[0];
    const char* compKeyAddress = (const char*)key;
    if ( String::CompareCharacters( compPairKeyAddress, compKeyAddress, keyByteWidth ) == DV_FALSE)
    {
        // Possible collision
        // Check if set contains our value
        const deint32 foundEntryIndex = this->m_Entries.FindValue( &insertedPair );
        if (foundEntryIndex == (const deint32)DV_NULL)
        {
            // Set does not contain value
            // Insert value to the set
            entryIndex = this->m_Entries.Insert( (void*)&insertedPair );

            // Update hash table
            this->m_HashTable[ hash ] = entryIndex;
        }
        else
        {
            entryIndex = foundEntryIndex;

            // Update hash table
            this->m_HashTable[ hash ] = entryIndex;
        }

        return entryIndex;
    }

    // Replace
    this->m_Entries.Replace(entryIndex, &insertedPair);

    // Update hash table
    this->m_HashTable[ hash ] = entryIndex;

    return entryIndex;
}

void* DvigEngine::HashMap::Find(const char* key)
{
    const deuint32 hash = HashMap::Hash( (deuchar*)key, (this->m_HashTableSize - 1) );
    deint32 entryIndex = (deint32)this->m_HashTable[ hash ];

    HashMapKeyValuePair* foundPair = this->m_Entries.Find<HashMapKeyValuePair*>( entryIndex );
    if (foundPair == nullptr) { return nullptr; }

    const deusize keyByteWidth = String::CharactersCount((deuchar*)&key[0]);
    const char* compPairKeyAddress = (const char*)&foundPair->m_Key[0];
    const char* compKeyAddress = (const char*)key;
    if ( String::CompareCharacters( &compPairKeyAddress[0], &compKeyAddress[0], keyByteWidth ) == DV_FALSE)
    {
        // Possible collision
        // Check if set contains our value
        const deisize setCapacity = (const deisize)this->m_Entries.GetCapacity();
        for (deisize i = 0; i < setCapacity; ++i)
        {
            HashMapKeyValuePair* newPair = this->m_Entries.Find<HashMapKeyValuePair*>( i );
            const deusize newPairKeyByteWidth = String::CharactersCount( &newPair->m_Key[0] );
            if (newPairKeyByteWidth == keyByteWidth && (String::CompareCharacters( &compKeyAddress[0], (const char*)&newPair->m_Key[0], keyByteWidth ) == DV_TRUE))
            {
                return newPair->m_Value;
            }
        }
        
        return nullptr;
    }

    return foundPair->m_Value;
}

void DvigEngine::HashMap::Remove(const char* key)
{
    const deuint32 hash = HashMap::Hash( (deuchar*)key, (this->m_HashTableSize - 1) );
    deint32 entryIndex = (deint32)this->m_HashTable[ hash ];

    HashMapKeyValuePair* foundPair = this->m_Entries.Find<HashMapKeyValuePair*>( entryIndex );
    if (foundPair == nullptr) { return; }

    const deusize keyByteWidth = String::CharactersCount((deuchar*)&key[0]);
    const char* compPairKeyAddress = (const char*)&foundPair->m_Key[0];
    const char* compKeyAddress = (const char*)key;
    if ( String::CompareCharacters( compPairKeyAddress, compKeyAddress, keyByteWidth ) == DV_FALSE)
    {
        // Possible collision
        // Check if set contains our value
        const deisize setCapacity = (const deisize)this->m_Entries.GetCapacity();
        for (deisize i = 0; i < setCapacity; ++i)
        {
            HashMapKeyValuePair* newPair = this->m_Entries.Find<HashMapKeyValuePair*>( i );
            const deusize newPairKeyByteWidth = String::CharactersCount( &newPair->m_Key[0] );
            if (newPairKeyByteWidth == keyByteWidth && (String::CompareCharacters( compKeyAddress, (const char*)&newPair->m_Key[0], keyByteWidth ) == DV_TRUE))
            {
                this->m_Entries.Remove( i );
                return;
            }
        }
    }
    else
    {
        this->m_Entries.Remove( entryIndex );
    }
}