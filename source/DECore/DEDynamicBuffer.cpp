#include "../../include/DECore.hpp"

void DvigEngine::DYNAMIC_BUFFER_DATA::Init(const Engine* const engine, DynamicBuffer* const object)
{
    m_Address = nullptr;
    m_AddressOffset = nullptr;
    m_ByteWidth = 0;
    m_AheadByteWidth = 1024;
    m_AllocatedByteWidth = 0;
    m_MemoryPoolIndex = DV_NULL;
}

void DvigEngine::DynamicBuffer::CopyToBuffer(const void* dataAddress, const deusize byteWidth)
{
    const void* const expectedAddress = (const void* const)((demachword)dataAddress + byteWidth);
    const void* const maxRelativeBufferAddress = (const void* const)(((demachword)dataAddress + m_Data.m_AllocatedByteWidth) - byteWidth);
    const deuint32 memoryPoolIndex = 0;
    if (m_Data.m_Address == nullptr)
    {
        const deusize newByteWidth = m_Data.m_AllocatedByteWidth + byteWidth;
        const deusize newAllocatedByteWidth = m_Data.m_AheadByteWidth + byteWidth;
        m_Data.m_Address = DvigEngine::Engine::AllocateObject( memoryPoolIndex, newAllocatedByteWidth )->Unwrap<void>();
        m_Data.m_AddressOffset = m_Data.m_Address;
        Engine::CopyMemory( m_Data.m_AddressOffset, (void*)dataAddress, byteWidth );
        m_Data.m_AddressOffset = (void*)((demachword)m_Data.m_Address + newByteWidth);
        m_Data.m_ByteWidth = newByteWidth;
        m_Data.m_AllocatedByteWidth = newAllocatedByteWidth;
        m_Data.m_MemoryPoolIndex = memoryPoolIndex;
    }
    else if (expectedAddress > maxRelativeBufferAddress)
    {
        const deusize newByteWidth = m_Data.m_AllocatedByteWidth + byteWidth;
        const deusize newAllocatedByteWidth = m_Data.m_AllocatedByteWidth + m_Data.m_AheadByteWidth + byteWidth;
        void* const tempAddress = DvigEngine::Engine::AllocateObject( memoryPoolIndex, newAllocatedByteWidth )->Unwrap<void>();
        Engine::CopyMemory( m_Data.m_AddressOffset, (void*)dataAddress, byteWidth );
        m_Data.m_AddressOffset = (void*)((demachword)tempAddress + newAllocatedByteWidth);
        m_Data.m_ByteWidth = newByteWidth;
        m_Data.m_AllocatedByteWidth = newAllocatedByteWidth;
        m_Data.m_MemoryPoolIndex = memoryPoolIndex;
        Engine::CopyMemory( tempAddress, m_Data.m_Address, m_Data.m_AllocatedByteWidth );
    }
}

void DvigEngine::DynamicBuffer::RequestFromBuffer(const deusize requestOffset, void* const destAddress, const deusize byteWidth)
{
    const void* const requestAddress = (const void* const)((demachword)m_Data.m_Address + requestOffset);
    Engine::CopyMemory( destAddress, (void*)requestAddress, byteWidth );
}