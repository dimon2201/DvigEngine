#include "../../include/DECore.hpp"

void DvigEngine::DynamicBuffer::Init(const deint32 memoryPoolIndex, const deusize bufferByteWidth)
{
    this->m_DataObject = Engine::Allocate( memoryPoolIndex, bufferByteWidth );
    this->m_AllocatedDataByteWidth = bufferByteWidth;
    this->m_DataByteWidth = 0;
    this->m_MemoryPoolIndex = memoryPoolIndex;
}

void DvigEngine::DynamicBuffer::Free()
{
    this->GetEngine()->Delete( this->m_DataObject );
    this->GetEngine()->Delete( this->GetMemoryObject() );
}

DvigEngine::deint32 DvigEngine::DynamicBuffer::Insert(const deisize offset, const void* data, const deusize dataByteWidth)
{
    deisize actualOffset = offset;
    if (actualOffset == DV_NULL) { actualOffset = this->m_DataByteWidth; } // insert to end of buffer

    void* dataAddress = this->m_DataObject->Unwrap<void*>();
    void* insertToAddress = Ptr<void*>::Add( &dataAddress, actualOffset );
    void* moveToAddress = Ptr<void*>::Add( &insertToAddress, dataByteWidth );
    const void* allocatedLastAddress = Ptr<void*>::Add( &dataAddress, this->m_AllocatedDataByteWidth );
    if (moveToAddress >= allocatedLastAddress)
    {
        // Overflow
        // Delete previous memory block
        Engine* engine = this->GetEngine();

        // Allocate new
        MemoryObject* newDataObject = Engine::Allocate( this->m_MemoryPoolIndex, this->m_AllocatedDataByteWidth + this->m_DataByteWidth + dataByteWidth );
        this->m_AllocatedDataByteWidth = this->m_AllocatedDataByteWidth + this->m_DataByteWidth + dataByteWidth;

        // Copy previous to new
        // And delete
        DvigEngine::Engine::MemoryCopy( newDataObject->Unwrap<void*>(), this->m_DataObject->Unwrap<void*>(), this->m_DataByteWidth );
        engine->Delete( this->m_DataObject );
        this->m_DataObject = newDataObject;

        // Update pointers
        dataAddress = this->m_DataObject->Unwrap<void*>();
        insertToAddress = Ptr<void*>::Add( &dataAddress, actualOffset );
        moveToAddress = Ptr<void*>::Add( &insertToAddress, dataByteWidth );
    }
    else
    {
        const void* dataLastAddress = Ptr<void*>::Add( &dataAddress, this->m_DataByteWidth );
        const deusize moveByteWidth = (demachword)dataLastAddress - (demachword)insertToAddress;
        DvigEngine::Engine::MemoryCopy( moveToAddress, insertToAddress, moveByteWidth );
    }
    
    DvigEngine::Engine::MemoryCopy( insertToAddress, data, dataByteWidth );
    this->m_Capacity += 1;
    this->m_DataByteWidth += dataByteWidth;

    return actualOffset;
}

void DvigEngine::DynamicBuffer::Find(const deisize offset, void* output, const deusize copyByteWidth)
{
    void* dataAddress = this->m_DataObject->Unwrap<void*>();
    const void* copyFromAddress = Ptr<void*>::Add( &dataAddress, offset );
    DvigEngine::Engine::MemoryCopy( output, copyFromAddress, copyByteWidth );
}

void DvigEngine::DynamicBuffer::Remove(const deisize offset, const deusize removeByteWidth)
{
    void* dataAddress = this->m_DataObject->Unwrap<void*>();
    void* removeAddress = Ptr<void*>::Add( &dataAddress, offset );
    const void* moveFromAddress = Ptr<void*>::Add( &removeAddress, removeByteWidth );
    const void* dataLastAddress = Ptr<void*>::Add( &dataAddress, this->m_DataByteWidth );
    const deusize moveByteWidth = (demachword)dataLastAddress - (demachword)moveFromAddress;

    if (moveFromAddress < dataLastAddress) {
        DvigEngine::Engine::MemoryMove( removeAddress, moveFromAddress, moveByteWidth );
    }

    this->m_Capacity -= 1;
    this->m_DataByteWidth -= removeByteWidth;
}

void DvigEngine::DynamicBuffer::Clear()
{
    const deint32 offset = 0;
    const deusize removeByteWidth = this->m_DataByteWidth;
    void* dataAddress = this->m_DataObject->Unwrap<void*>();
    void* removeAddress = Ptr<void*>::Add( &dataAddress, offset );
    const void* moveFromAddress = Ptr<void*>::Add( &removeAddress, removeByteWidth );
    const void* dataLastAddress = Ptr<void*>::Add( &dataAddress, this->m_DataByteWidth );
    const deusize moveByteWidth = (demachword)dataLastAddress - (demachword)moveFromAddress;

    if (moveFromAddress < dataLastAddress) {
        DvigEngine::Engine::MemoryCopy( removeAddress, moveFromAddress, moveByteWidth );
    }

    this->m_Capacity = 0;
    this->m_DataByteWidth -= removeByteWidth;
}