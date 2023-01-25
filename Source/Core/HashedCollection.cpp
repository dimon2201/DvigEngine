#include "../../Include/DvigEngine/Core.hpp"

DvigEngine::uint32 DvigEngine::HashedCollection::HashMurMur(const char* input, const cpuword bitMask)
{
    const ucpuint m = 0x5bd1e995;
    const ucpuint seed = 0;
    const scpuint r = 24;

    ucpuint len = StringSystem::GetByteWidth(input);
    ucpuint h = seed ^ len;

    const uint8* data = (const uint8*)input;
    ucpuint k = 0;

    while (len >= 4)
    {
        k  = data[0]; k |= data[1] << 8; k |= data[2] << 16; k |= data[3] << 24;
        k *= m; k ^= k >> r; k *= m;
        h *= m; h ^= k;
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
    }

    h ^= h >> 13;
    h *= m;
    h ^= h >> 15;

    return (uint32)h & (uint32)bitMask;
}

void DvigEngine::HashedCollection::Init(const usize entryByteWidth, const usize reservedCapacity, const usize hashTableEntryCount)
{
    usize newReservedCapacity = reservedCapacity;
    usize newHashTableEntryCount = hashTableEntryCount;
    if (reservedCapacity == 0) { newReservedCapacity = 1024; }
    if (hashTableEntryCount == 0) { newHashTableEntryCount = 1024; }

    this->m_Collection = Engine::GetClassInstance()->Create<Collection>("_EngineAllInstancesCollection");
    this->m_Collection->Init( entryByteWidth, newReservedCapacity );
    this->m_MemoryObject = Engine::GetClassInstance()->MemoryAllocate( 0, sizeof(ucpuint) * newHashTableEntryCount );
    Engine::GetClassInstance()->MemorySet( this->m_MemoryObject->GetAddress(), 0, sizeof(ucpuint) * newHashTableEntryCount );
    this->m_HashTable = (uint32*)this->m_MemoryObject->GetAddress();
    this->m_HashTableEntryCount = newHashTableEntryCount;
    for (uint32 i = 0; i < this->m_HashTableEntryCount; ++i) {
        this->m_HashTable[i] = DVIG_NULL;
    }
}

void DvigEngine::HashedCollection::Free()
{
    DVIG_ASSERT( (this->m_MemoryObject != nullptr), DebugResult::NULL_POINTER )

    this->m_Collection->Free();
    Engine::GetClassInstance()->MemoryDelete( this->m_MemoryObject );
}

DvigEngine::uint32 DvigEngine::HashedCollection::Insert(const void* entryMemory, const char* label)
{
    cpuword bitMask = this->m_HashTableEntryCount - 1;
    uint32 hash = HashedCollection::HashMurMur( &label[0], bitMask );

    uint32 index = this->m_HashTable[ hash ];
    if (index == DVIG_NULL) {
        index = this->m_Collection->Insert( entryMemory, &label[0] );
    } else {
        void* foundAddress = this->m_Collection->Find( &label[0] );
        if (foundAddress == nullptr) {
            index = this->m_Collection->Insert( entryMemory, &label[0] );
        }
    }

    this->m_HashTable[ hash ] = index;
    return index;
}

void* DvigEngine::HashedCollection::Find(const char* label)
{
    cpuword bitMask = this->m_HashTableEntryCount - 1;
    uint32 hash = HashedCollection::HashMurMur( &label[0], bitMask );

    uint32 index = this->m_HashTable[ hash ];
    if (index == DVIG_NULL)
    {
        return this->m_Collection->Find( &label[0] );
    }
    else
    {
        void* foundValueAddress = this->m_Collection->FindAt( index );
        if (foundValueAddress == nullptr) { return nullptr; }

        const char* foundLabelAddress = Ptr<const char*>::Subtract( foundValueAddress, DVIG_MAX_STRING_BYTE_WIDTH );
        if (StringSystem::GetStringsEqual( &foundLabelAddress[0], &label[0] ) == DVIG_TRUE) {
            return foundValueAddress;
        } else {
            return this->m_Collection->Find( &label[0] );
        }
    }

    return nullptr;
}

void* DvigEngine::HashedCollection::FindAt(const uint32 index)
{
    return this->m_Collection->FindAt(index);
}

void DvigEngine::HashedCollection::Remove(const char* label)
{
    void* foundAddress = this->m_Collection->Find( &label[0] );
    if (foundAddress != nullptr)
    {
        this->m_Collection->Remove( &label[0] );
        
        cpuword bitMask = this->m_HashTableEntryCount - 1;
        uint32 hash = HashedCollection::HashMurMur( &label[0], bitMask );
        this->m_HashTable[ hash ] = DVIG_NULL;
    }
}

void DvigEngine::HashedCollection::Clear()
{
    this->m_Collection->Clear();
    Engine::GetClassInstance()->MemorySet( this->m_MemoryObject->GetAddress(), 0, sizeof(ucpuint) * this->m_HashTableEntryCount );
}