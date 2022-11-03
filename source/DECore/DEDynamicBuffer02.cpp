#include "../../include/DECore.hpp"

void DvigEngine2::DynamicBuffer::Init(const deusize reservedByteWidth, const deusize bufferByteWidth)
{
    m_Prop.m_DataObject = Engine::Allocate( 0, reservedByteWidth + bufferByteWidth );
    m_Prop.m_ReservedDataByteWidth = reservedByteWidth;
    m_Prop.m_AllocatedDataByteWidth = reservedByteWidth + bufferByteWidth;
    m_Prop.m_DataByteWidth = bufferByteWidth;
}

void DvigEngine2::DynamicBuffer::Insert(const deisize offset, const void* data, const deusize dataByteWidth)
{
    deisize actualOffset = offset;
    if (actualOffset == DV_NULL) { actualOffset = m_Prop.m_DataByteWidth; } // insert to end of buffer

    void* dataAddress = m_Prop.m_DataObject->Unwrap<void*>();
    void* insertToAddress = Ptr<void*>::Add( &dataAddress, actualOffset );
    const void* allocatedLastAddress = Ptr<void*>::Add( &dataAddress, m_Prop.m_AllocatedDataByteWidth );
    if (insertToAddress >= allocatedLastAddress)
    {
        // Overflow
        // Delete previous memory block
        Engine* engine = this->GetEngine();
        engine->Delete( &m_Prop.m_DataObject );

        // Allocate new
        m_Prop.m_DataObject = Engine::Allocate( 0, m_Prop.m_ReservedDataByteWidth + m_Prop.m_DataByteWidth + dataByteWidth );
        m_Prop.m_AllocatedDataByteWidth = m_Prop.m_ReservedDataByteWidth + m_Prop.m_DataByteWidth + dataByteWidth;
    }
    else
    {
        const void* dataLastAddress = Ptr<void*>::Add( &dataAddress, m_Prop.m_DataByteWidth );
        const deusize moveByteWidth = (demachword)dataLastAddress - (demachword)insertToAddress;
        void* moveToAddress = Ptr<void*>::Add( &insertToAddress, dataByteWidth );
        Engine::MoveMemory( moveToAddress, insertToAddress, moveByteWidth );
    }
    
    Engine::CopyMemory( insertToAddress, data, dataByteWidth );
    m_Prop.m_DataByteWidth += dataByteWidth;
}

void DvigEngine2::DynamicBuffer::Find(const deisize offset, void* output, const deusize copyByteWidth)
{
    void* dataAddress = m_Prop.m_DataObject->Unwrap<void*>();
    const void* copyFromAddress = Ptr<void*>::Add( &dataAddress, offset );
    Engine::CopyMemory( output, copyFromAddress, copyByteWidth );
}

void DvigEngine2::DynamicBuffer::Remove(const deisize offset, const deusize removeByteWidth)
{
    void* dataAddress = m_Prop.m_DataObject->Unwrap<void*>();
    void* removeAddress = Ptr<void*>::Add( &dataAddress, offset );
    const void* moveFromAddress = Ptr<void*>::Add( &removeAddress, removeByteWidth );
    const void* dataLastAddress = Ptr<void*>::Add( &dataAddress, m_Prop.m_DataByteWidth );
    const deusize moveByteWidth = (demachword)dataLastAddress - (demachword)moveFromAddress;
    Engine::MoveMemory( removeAddress, moveFromAddress, moveByteWidth );

    m_Prop.m_DataByteWidth -= removeByteWidth;
}