#include "../../include/dvigengine/DECore.hpp"

void DvigEngine::Collection::Init(const usize entryByteWidth, const usize reservedCapacity)
{
    this->m_EntryByteWidth = entryByteWidth + DVIG_MAX_STRING_BYTE_WIDTH;
    this->m_ReservedCapacity = reservedCapacity;
    this->m_EntryCount = 0;
    this->m_MemoryObject = nullptr;
    this->m_CurrentByteWidth = 0;
    this->m_AllocatedByteWidth = 0;

    if (reservedCapacity == 0) {
        this->m_ReservedCapacity = 1024;
    }
}

void DvigEngine::Collection::Free()
{
    DVIG_ASSERT( (this->m_MemoryObject != nullptr), DebugResult::NULL_POINTER )

    Engine::GetClassInstance()->MemoryDelete( this->m_MemoryObject );
    Engine::GetClassInstance()->MemoryDelete( this->GetMemoryObject() );
}

DvigEngine::uint32 DvigEngine::Collection::Insert(const void* entryMemory, const char* label)
{
    Engine* pEngine = Engine::GetClassInstance();
    if (this->m_AllocatedByteWidth == 0 || this->m_CurrentByteWidth + this->m_EntryByteWidth > this->m_AllocatedByteWidth)
    {
        // Allocate more memory for Collection
        MemoryObject* newMemObject = pEngine->MemoryAllocate( 0, this->m_CurrentByteWidth +
            (this->m_ReservedCapacity * this->m_EntryByteWidth)
        );

        pEngine->MemoryCopy( newMemObject->GetAddress(), this->m_MemoryObject->GetAddress(), this->m_AllocatedByteWidth );
        this->m_AllocatedByteWidth += this->m_ReservedCapacity * this->m_EntryByteWidth;
    
        if (this->m_MemoryObject != nullptr) {
            pEngine->MemoryDelete( this->m_MemoryObject );
        }
        this->m_MemoryObject = newMemObject;
    }

    void* entryLabelAddress = Ptr<void*>::Add( this->m_MemoryObject->GetAddress(), this->m_EntryCount * this->m_EntryByteWidth );
    void* entryValueAddress = Ptr<void*>::Add( this->m_MemoryObject->GetAddress(), DVIG_MAX_STRING_BYTE_WIDTH + (this->m_EntryCount * this->m_EntryByteWidth) );
    pEngine->MemoryCopy( entryLabelAddress, &label[0], StringSystem::GetByteWidth(&label[0]) );
    pEngine->MemoryCopy( entryValueAddress, entryMemory, this->m_EntryByteWidth - DVIG_MAX_STRING_BYTE_WIDTH );

    this->m_CurrentByteWidth += this->m_EntryByteWidth;
    this->m_EntryCount += 1;

    return this->m_EntryCount - 1;
}

void* DvigEngine::Collection::Find(const char* label)
{
    void* entryLabelAddress;
    void* entryValueAddress;
    for (uint32 i = 0; i < this->m_EntryCount; ++i)
    {
        entryLabelAddress = Ptr<void*>::Add( this->m_MemoryObject->GetAddress(), i * this->m_EntryByteWidth );
        entryValueAddress = Ptr<void*>::Add( this->m_MemoryObject->GetAddress(), DVIG_MAX_STRING_BYTE_WIDTH + (i * this->m_EntryByteWidth) );

        if (StringSystem::GetStringsEqual((const char*)entryLabelAddress, &label[0]) == DVIG_TRUE) {
            return entryValueAddress;
        }
    }

    return nullptr;
}

void* DvigEngine::Collection::FindAt(const uint32 index)
{
    void* entryValueAddress = Ptr<void*>::Add( this->m_MemoryObject->GetAddress(), DVIG_MAX_STRING_BYTE_WIDTH + (index * this->m_EntryByteWidth) );
    return entryValueAddress;
}

void DvigEngine::Collection::Remove(const char* label)
{
    void* entryLabelAddress;
    void* entryValueAddress;
    for (uint32 i = 0; i < this->m_EntryCount; ++i)
    {
        entryLabelAddress = Ptr<void*>::Add( this->m_MemoryObject->GetAddress(), i * this->m_EntryByteWidth );
        entryValueAddress = Ptr<void*>::Add( this->m_MemoryObject->GetAddress(), DVIG_MAX_STRING_BYTE_WIDTH + (i * this->m_EntryByteWidth) );

        if (StringSystem::GetStringsEqual((const char*)entryLabelAddress, &label[0]) == DVIG_TRUE) {
            this->RemoveAt( i );
            break;
        }
    }
}

void DvigEngine::Collection::RemoveAt(const uint32 index)
{
    void* moveToAddress = Ptr<void*>::Add( this->m_MemoryObject->GetAddress(), index * this->m_EntryByteWidth );
    void* moveFromAddress = Ptr<void*>::Add( this->m_MemoryObject->GetAddress(), (index + 1) * this->m_EntryByteWidth );
    Engine::GetClassInstance()->MemoryMove( moveToAddress, moveFromAddress, this->m_AllocatedByteWidth - this->m_EntryByteWidth );
    this->m_EntryCount -= 1;
    this->m_CurrentByteWidth -= this->m_EntryByteWidth;
}

void DvigEngine::Collection::Clear()
{
    Engine::GetClassInstance()->MemoryDelete( this->m_MemoryObject );
    this->m_EntryCount = 0;
    this->m_MemoryObject = nullptr;
    this->m_CurrentByteWidth = 0;
    this->m_AllocatedByteWidth = 0;
}