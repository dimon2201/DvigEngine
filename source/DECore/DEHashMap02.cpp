#include "../../include/DECore.hpp"

DvigEngine2::deuint32 DvigEngine2::HashMap::Hash(const destring input, const demachword bitMask)
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

DvigEngine2::deuint32 DvigEngine2::HashMap::HashMurMur(const destring input, const demachword bitMask)
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

void DvigEngine2::HashMap::Init(const deusize reservedCapacity, const deusize entryValueByteWidth, const deusize hashTableSize)
{
    m_Prop.m_Capacity = 0;
    m_Prop.m_EntryValueByteWidth = entryValueByteWidth;
    m_Prop.m_Entries.Init( reservedCapacity, entryValueByteWidth );
    m_Prop.m_HashTableSize = hashTableSize;
    m_Prop.m_HashTable = Engine::Allocate( 0, sizeof(demachword) * hashTableSize )->Unwrap<demachword*>();
    for (deisize i = 0; i < (deisize)m_Prop.m_HashTableSize; ++i)
    {
        m_Prop.m_HashTable[ i ] = 0;
    }
}

DvigEngine2::deint32 DvigEngine2::HashMap::Insert(const char* key, void* value)
{
    const deuint32 hash = HashMap::Hash( (deuchar*)key, (this->GetData()->m_HashTableSize - 1) );
    deint32 entryIndex = (deint32)this->GetData()->m_HashTable[ hash ];
    
    const deusize keyByteWidth = String::CharactersCount((deuchar*)&key[0]);
    HashMapKeyValuePair insertedPair;
    Engine::SetMemory( &insertedPair.m_Key[0], 0, DV_MEMORY_COMMON_STRING_BYTE_WIDTH );
    Engine::CopyMemory( &insertedPair.m_Key[0], &key[0], keyByteWidth );
    insertedPair.m_Key[keyByteWidth] = 0;
    insertedPair.m_Value = value;

    HashMapKeyValuePair* foundPair = this->GetData()->m_Entries.Find<HashMapKeyValuePair*>( entryIndex );
    if (foundPair == nullptr)
    {
        // Set does not have our value
        // Insert value to the set
        entryIndex = this->GetData()->m_Entries.Insert( (void*)&insertedPair );

        // Update hash table
        this->GetData()->m_HashTable[ hash ] = entryIndex;
        
        return entryIndex;
    }

    deuchar* compPairKeyAddress = (deuchar*)&foundPair->m_Key[0];
    deuchar* compKeyAddress = (deuchar*)key;
    if ( String::CompareCharacters( compPairKeyAddress, compKeyAddress, keyByteWidth ) == DV_FALSE)
    {
        // Possible collision
        // Check if set contains our value
        const deint32 foundEntryIndex = this->GetData()->m_Entries.FindValue( &insertedPair );
        if (foundEntryIndex == DV_NULL)
        {
            // Set does not contain value
            // Insert value to the set
            entryIndex = this->GetData()->m_Entries.Insert( (void*)&insertedPair );

            // Update hash table
            this->GetData()->m_HashTable[ hash ] = entryIndex;
        }
        else
        {
            entryIndex = foundEntryIndex;

            // Update hash table
            this->GetData()->m_HashTable[ hash ] = entryIndex;
        }

        return entryIndex;
    }

    // Replace
    this->GetData()->m_Entries.Replace(entryIndex, &insertedPair);

    // Update hash table
    this->GetData()->m_HashTable[ hash ] = entryIndex;

    return entryIndex;
}

void* DvigEngine2::HashMap::Find(const char* key)
{
    const deuint32 hash = HashMap::Hash( (deuchar*)key, (this->GetData()->m_HashTableSize - 1) );
    deint32 entryIndex = (deint32)this->GetData()->m_HashTable[ hash ];

    HashMapKeyValuePair* foundPair = this->GetData()->m_Entries.Find<HashMapKeyValuePair*>( entryIndex );
    if (foundPair == nullptr) { return nullptr; }

    const deusize keyByteWidth = String::CharactersCount((deuchar*)&key[0]);
    deuchar* compPairKeyAddress = (deuchar*)&foundPair->m_Key[0];
    deuchar* compKeyAddress = (deuchar*)key;
    std::cout << foundPair->m_Key << " " << (demachword)foundPair->m_Value << std::endl;
    if ( String::CompareCharacters( compPairKeyAddress, compKeyAddress, keyByteWidth ) == DV_FALSE)
    {
        // Possible collision
        // Check if set contains our value
        const deisize setCapacity = (const deisize)this->GetData()->m_Entries.GetCapacity();
        for (deisize i = 0; i < setCapacity; ++i)
        {
            HashMapKeyValuePair* newPair = this->GetData()->m_Entries.Find<HashMapKeyValuePair*>( i );
            const deusize newPairKeyByteWidth = String::CharactersCount( &newPair->m_Key[0] );
            if (newPairKeyByteWidth == keyByteWidth && (String::CompareCharacters( compKeyAddress, &newPair->m_Key[0], keyByteWidth ) == DV_TRUE))
            {
                return newPair->m_Value;
            }
        }
        
        return nullptr;
    }

    return foundPair->m_Value;
}

void DvigEngine2::HashMap::Remove(const char* key)
{
    const deuint32 hash = HashMap::Hash( (deuchar*)key, (this->GetData()->m_HashTableSize - 1) );
    deint32 entryIndex = (deint32)this->GetData()->m_HashTable[ hash ];

    HashMapKeyValuePair* foundPair = this->GetData()->m_Entries.Find<HashMapKeyValuePair*>( entryIndex );
    if (foundPair == nullptr) { return; }

    const deusize keyByteWidth = String::CharactersCount((deuchar*)&key[0]);
    deuchar* compPairKeyAddress = (deuchar*)&foundPair->m_Key[0];
    deuchar* compKeyAddress = (deuchar*)key;
    if ( String::CompareCharacters( compPairKeyAddress, compKeyAddress, keyByteWidth ) == DV_FALSE)
    {
        // Possible collision
        // Check if set contains our value
        const deisize setCapacity = (const deisize)this->GetData()->m_Entries.GetCapacity();
        for (deisize i = 0; i < setCapacity; ++i)
        {
            HashMapKeyValuePair* newPair = this->GetData()->m_Entries.Find<HashMapKeyValuePair*>( i );
            const deusize newPairKeyByteWidth = String::CharactersCount( &newPair->m_Key[0] );
            if (newPairKeyByteWidth == keyByteWidth && (String::CompareCharacters( compKeyAddress, &newPair->m_Key[0], keyByteWidth ) == DV_TRUE))
            {
                this->GetData()->m_Entries.Remove( i );
                return;
            }
        }
    }
    else
    {
        this->GetData()->m_Entries.Remove( entryIndex );
    }
}