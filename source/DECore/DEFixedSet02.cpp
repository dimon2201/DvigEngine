#include "../../include/DECore.hpp"

void DvigEngine2::FixedSet::Init(const DvigEngine2::deusize reservedCapacity, const DvigEngine2::deusize entryByteWidth)
{
    m_Prop.m_Capacity = 0;
    m_Prop.m_EntryByteWidth = entryByteWidth;
    m_Prop.m_DataObject = DvigEngine2::Engine::Allocate( 0, entryByteWidth + (entryByteWidth * reservedCapacity) );
    m_Prop.m_ReservedDataByteWidth = entryByteWidth * reservedCapacity;
    m_Prop.m_AllocatedDataByteWidth = entryByteWidth + m_Prop.m_ReservedDataByteWidth;
    m_Prop.m_DataByteWidth = 0;
}

DvigEngine2::deint32 DvigEngine2::FixedSet::Insert(void* entry)
{
    if (this->GetData()->m_DataByteWidth + this->GetData()->m_EntryByteWidth >= this->GetData()->m_AllocatedDataByteWidth)
    {
        // Allocate new block of memory
        // And copy previous data to newly created
        Engine* engine = this->GetEngine();

        const deusize allocByteWidth = this->GetData()->m_EntryByteWidth + (this->GetData()->m_Capacity * this->GetData()->m_EntryByteWidth) + this->GetData()->m_ReservedDataByteWidth;
        MemoryObject* newMemoryObject = DvigEngine2::Engine::Allocate( 0, allocByteWidth );

        Engine::CopyMemory( newMemoryObject->Unwrap<void*>(), this->GetData()->m_DataObject->Unwrap<void*>(), this->GetData()->m_DataByteWidth );
        engine->Delete( &this->GetData()->m_DataObject );

        this->GetData()->m_DataObject = newMemoryObject;
        this->GetData()->m_AllocatedDataByteWidth = allocByteWidth;
    }

    // Insert new entry
    const deuint32 capacity = this->GetData()->m_Capacity;
    deuchar* entryAddress = this->GetData()->m_DataObject->Unwrap<deuchar*>();
    entryAddress = DvigEngine2::Ptr<deuchar*>::Add( &entryAddress, capacity * this->GetData()->m_EntryByteWidth );
    Engine::CopyMemory( entryAddress, entry, this->GetData()->m_EntryByteWidth );
    this->GetData()->m_Capacity += 1;
    this->GetData()->m_DataByteWidth += this->GetData()->m_EntryByteWidth;

    return this->GetData()->m_Capacity - 1;
}

void DvigEngine2::FixedSet::Replace(const deint32 index, void* entry)
{
    if (index >= (deint32)this->GetData()->m_Capacity) { return; }
    void* entryAddress = this->GetData()->m_DataObject->Unwrap<void*>();
    entryAddress = DvigEngine2::Ptr<void*>::Add( &entryAddress, index * this->GetData()->m_EntryByteWidth );
    Engine::CopyMemory( entryAddress, entry, this->GetData()->m_EntryByteWidth );
}

void DvigEngine2::FixedSet::Remove(const deint32 index)
{
    if (index != (deint32)this->GetData()->m_Capacity)
    {
        // Not last index
        void* dataAddress = this->GetData()->m_DataObject->Unwrap<void*>();
        void* entryAddress = DvigEngine2::Ptr<void*>::Add( &dataAddress, index * this->GetData()->m_EntryByteWidth );
        void* nextEntryAddress = DvigEngine2::Ptr<void*>::Add( &entryAddress, this->GetData()->m_EntryByteWidth );
        const deusize moveByteWidth = this->GetData()->m_AllocatedDataByteWidth - ((index + 1) * this->GetData()->m_EntryByteWidth);
        Engine::MoveMemory( entryAddress, nextEntryAddress, moveByteWidth );
    }

    this->GetData()->m_Capacity -= 1;
    this->GetData()->m_DataByteWidth -= this->GetData()->m_EntryByteWidth;
}

DvigEngine2::deint32 DvigEngine2::FixedSet::FindValue(void* entry)
{
    for (deisize i = 0; i < (deisize)this->GetData()->m_Capacity; ++i)
    {
        void* dataAddress = this->GetData()->m_DataObject->Unwrap<void*>();
        void* entryAddress = DvigEngine2::Ptr<void*>::Add( &dataAddress, i * this->GetData()->m_EntryByteWidth );

        deuchar* compSetAddress = (deuchar*)entryAddress;
        deuchar* compEntryAddress = (deuchar*)entry;
        if (String::CompareCharacters( compSetAddress, compEntryAddress, this->GetData()->m_EntryByteWidth ) == DV_TRUE)
        {
            return i;
        }
    }

    return DV_NULL;
}