#include "../../include/DECore.hpp"

void DvigEngine::FixedSet::Init(const deint32 memoryPoolIndex, const DvigEngine::deusize reservedCapacity, const DvigEngine::deusize entryByteWidth)
{
    m_Capacity = 0;
    m_EntryByteWidth = entryByteWidth;
    m_DataObject = DvigEngine::Engine::Allocate( memoryPoolIndex, entryByteWidth + (entryByteWidth * reservedCapacity) );
    m_ReservedDataByteWidth = entryByteWidth * reservedCapacity;
    m_AllocatedDataByteWidth = entryByteWidth + m_ReservedDataByteWidth;
    m_DataByteWidth = 0;
    m_MemoryPoolIndex = memoryPoolIndex;
}

void DvigEngine::FixedSet::Free()
{
    this->GetEngine()->Delete( this->GetMemoryObject() );
}

DvigEngine::deint32 DvigEngine::FixedSet::Insert(void* entry)
{
    if (this->m_DataByteWidth + this->m_EntryByteWidth >= this->m_AllocatedDataByteWidth)
    {
        // Allocate new block of memory
        // And copy previous data to newly created
        Engine* engine = this->GetEngine();

        const deusize allocByteWidth = this->m_EntryByteWidth + (this->m_Capacity * this->m_EntryByteWidth) + this->m_ReservedDataByteWidth;
        MemoryObject* newMemoryObject = DvigEngine::Engine::Allocate( this->m_MemoryPoolIndex, allocByteWidth );

        DvigEngine::Engine::MemoryCopy( newMemoryObject->Unwrap<void*>(), this->m_DataObject->Unwrap<void*>(), this->m_DataByteWidth );
        engine->Delete( this->m_DataObject );

        this->m_DataObject = newMemoryObject;
        this->m_AllocatedDataByteWidth = allocByteWidth;
    }

    // Insert new entry
    const deuint32 capacity = this->m_Capacity;
    deuchar* entryAddress = this->m_DataObject->Unwrap<deuchar*>();
    entryAddress = DvigEngine::Ptr<deuchar*>::Add( &entryAddress, capacity * this->m_EntryByteWidth );
    DvigEngine::Engine::MemoryCopy( entryAddress, entry, this->m_EntryByteWidth );
    this->m_Capacity += 1;
    this->m_DataByteWidth += this->m_EntryByteWidth;

    return this->m_Capacity - 1;
}

void DvigEngine::FixedSet::Replace(const deint32 index, void* entry)
{
    if (index >= (deint32)this->m_Capacity) { return; }
    void* entryAddress = this->m_DataObject->Unwrap<void*>();
    entryAddress = DvigEngine::Ptr<void*>::Add( &entryAddress, index * this->m_EntryByteWidth );
    DvigEngine::Engine::MemoryCopy( entryAddress, entry, this->m_EntryByteWidth );
}

void DvigEngine::FixedSet::Remove(const deint32 index)
{
    if (index != (deint32)this->m_Capacity - 1)
    {
        // Not last index
        void* dataAddress = this->m_DataObject->Unwrap<void*>();
        void* entryAddress = DvigEngine::Ptr<void*>::Add( &dataAddress, index * this->m_EntryByteWidth );
        void* nextEntryAddress = DvigEngine::Ptr<void*>::Add( &entryAddress, this->m_EntryByteWidth );
        const deusize moveByteWidth = this->m_AllocatedDataByteWidth - ((index + 1) * this->m_EntryByteWidth);
        DvigEngine::Engine::MemoryMove( entryAddress, nextEntryAddress, moveByteWidth );
    }

    this->m_Capacity -= 1;
    this->m_DataByteWidth -= this->m_EntryByteWidth;
}

DvigEngine::deint32 DvigEngine::FixedSet::FindValue(void* entry)
{
    void* dataAddress = this->m_DataObject->Unwrap<void*>();
    for (deisize i = 0; i < (deisize)this->m_Capacity; ++i)
    {
        void* entryAddress = DvigEngine::Ptr<void*>::Add( &dataAddress, i * this->m_EntryByteWidth );
        const char* compSetAddress = (const char*)entryAddress;
        const char* compEntryAddress = (const char*)entry;

        if (String::CompareCharacters( compSetAddress, compEntryAddress, this->m_EntryByteWidth ) == DV_TRUE)
        {
            return i;
        }
    }

    return DV_NULL;
}

void DvigEngine::FixedSet::Clear()
{
    this->m_Capacity = 0;
    this->m_DataByteWidth = 0;
}