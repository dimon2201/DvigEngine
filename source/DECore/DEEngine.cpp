#include "../../include/dvigengine/DECore.hpp"
#include <cstring>
#include <cstdlib>

DvigEngine::Engine* DvigEngine::Engine::m_Instance = nullptr;

DvigEngine::Engine::Engine(const DvigEngine::EngineInfo* engineInfo)
{
    DVIG_ASSERT( (Engine::m_Instance == nullptr), DebugResult::SECOND_INSTANCE_ENGINE )

    #if defined(DVIG_DEBUG)
        Debug::m_State = (DebugState*)malloc( sizeof(DebugState) );
        Engine::MemorySet( Debug::m_State, 0, sizeof(DebugState) );
    #endif
    
    Engine::m_Instance = (Engine*)malloc( sizeof(Engine) );
    Engine::m_Instance->m_Info = (EngineInfo*)malloc( sizeof(EngineInfo) );
    Engine::MemoryCopy( Engine::m_Instance->m_Info, engineInfo, sizeof(EngineInfo) );

    // Initialize system Memory Pool
    const sint32 systemMemoryPoolIndex = 0;
    Engine::m_Instance->m_MemoryPools[systemMemoryPoolIndex].m_Address = malloc( Engine::m_Instance->m_Info->SystemMemoryPoolByteWidth );
    Engine::m_Instance->m_MemoryPools[systemMemoryPoolIndex].m_ByteWidth = Engine::m_Instance->m_Info->SystemMemoryPoolByteWidth;
    Engine::m_Instance->m_MemoryPools[systemMemoryPoolIndex].m_OccupiedByteWidth = 0;
    Engine::m_Instance->m_MemoryPools[systemMemoryPoolIndex].m_AllocationCount = 1;
    Engine::MemorySet( Engine::m_Instance->m_MemoryPools[systemMemoryPoolIndex].m_Address, 0, Engine::m_Instance->m_MemoryPools[systemMemoryPoolIndex].m_ByteWidth );
    Engine::m_Instance->MemoryInsertObjectToMemoryPool(systemMemoryPoolIndex, 0, Engine::m_Instance->m_MemoryPools[systemMemoryPoolIndex].m_ByteWidth - sizeof(MemoryObject));
    Engine::m_Instance->m_MemoryPools[systemMemoryPoolIndex].m_OccupiedByteWidth += sizeof(MemoryObject);

    // Initialize component storage Memory Pool
    const sint32 componentStorageMemoryPoolIndex = 1;
    Engine::m_Instance->m_MemoryPools[componentStorageMemoryPoolIndex].m_Address = malloc( Engine::m_Instance->m_Info->ComponentStorageMemoryPoolByteWidth );
    Engine::m_Instance->m_MemoryPools[componentStorageMemoryPoolIndex].m_ByteWidth = Engine::m_Instance->m_Info->ComponentStorageMemoryPoolByteWidth;
    Engine::m_Instance->m_MemoryPools[componentStorageMemoryPoolIndex].m_OccupiedByteWidth = 0;
    Engine::m_Instance->m_MemoryPools[componentStorageMemoryPoolIndex].m_AllocationCount = 1;
    Engine::MemorySet( Engine::m_Instance->m_MemoryPools[componentStorageMemoryPoolIndex].m_Address, 0, Engine::m_Instance->m_MemoryPools[componentStorageMemoryPoolIndex].m_ByteWidth );
    Engine::m_Instance->MemoryInsertObjectToMemoryPool(componentStorageMemoryPoolIndex, 0, Engine::m_Instance->m_MemoryPools[componentStorageMemoryPoolIndex].m_ByteWidth - sizeof(MemoryObject));
    Engine::m_Instance->m_MemoryPools[componentStorageMemoryPoolIndex].m_OccupiedByteWidth += sizeof(MemoryObject);

    // Initialize all instances Hashed Collection
    Engine::m_Instance->m_AllInstances = Engine::m_Instance->Create<HashedCollection>("_EngineAllInstancesHashedCollection");
    Engine::m_Instance->m_AllInstances->Init( sizeof(cpuword), 0, 0 );
}

void DvigEngine::Engine::Free()
{
    #if defined(DVIG_DEBUG)
        free( Debug::m_State );
    #endif
    free( Engine::m_Instance->m_Info );
    free( Engine::m_Instance->m_MemoryPools[0].m_Address );
    free( Engine::m_Instance->m_MemoryPools[1].m_Address );
    free( Engine::m_Instance );
}

DvigEngine::MemoryObject* DvigEngine::Engine::MemoryAllocate(const sint32 index, const usize byteWidth)
{
    DVIG_ASSERT( (Engine::m_Instance->m_MemoryPools[index].m_Address != nullptr), DebugResult::NULL_POINTER )
    DVIG_ASSERT( (Engine::m_Instance->m_MemoryPools[index].m_ByteWidth > (Engine::m_Instance->m_MemoryPools[index].m_OccupiedByteWidth + byteWidth + sizeof(MemoryObject))), DebugResult::NOT_ENOUGH_MEMORY_POOL )

    usize i;
    MemoryObject* memObject = (MemoryObject*)Engine::m_Instance->m_MemoryPools[index].m_Address;
    for (i = 0; i < Engine::m_Instance->m_MemoryPools[index].m_AllocationCount; ++i)
    {
        if (memObject->m_ByteWidth > byteWidth + sizeof(MemoryObject) &&
            ((cpuword)memObject->m_Flags & (cpuword)MemoryObjectFlag::FREE) == (cpuword)MemoryObjectFlag::FREE) {
            break;
        }

        memObject = Ptr<MemoryObject*>::Add( memObject, sizeof(MemoryObject) + memObject->m_ByteWidth );
    }

    // memObject->m_ByteWidth = byteWidth;
    memObject->m_Flags = MemoryObjectFlag::OCCUPIED;

    if (i == Engine::m_Instance->m_MemoryPools[index].m_AllocationCount - 1)
    {
        // If last entry then memory pool gets more occupied
        Engine::m_Instance->m_MemoryPools[index].m_OccupiedByteWidth += sizeof(MemoryObject) + byteWidth;

        MemoryObject* stub = Ptr<MemoryObject*>::Add( memObject, sizeof(MemoryObject) + byteWidth );
        stub->m_ByteWidth = memObject->m_ByteWidth - byteWidth - sizeof(MemoryObject);
        stub->m_Flags = MemoryObjectFlag::FREE;

        memObject->m_ByteWidth = byteWidth;
        Engine::m_Instance->m_MemoryPools[index].m_AllocationCount += 1;
    }
    
    return memObject;
}

void DvigEngine::Engine::MemoryDelete(MemoryObject* object)
{
    DVIG_ASSERT( (object != nullptr), DebugResult::NULL_POINTER )

    object->m_Flags = MemoryObjectFlag::FREE;
}

void DvigEngine::Engine::MemoryCopy(void* destAddress, const void* srcAddress, const DvigEngine::usize byteWidth)
{
    memcpy( destAddress, srcAddress, byteWidth );
}

void DvigEngine::Engine::MemoryMove(void* destAddress, const void* srcAddress, const DvigEngine::usize byteWidth)
{
    memmove( destAddress, srcAddress, byteWidth );
}

void DvigEngine::Engine::MemorySet(void* destAddress, const DvigEngine::cpuword value, const DvigEngine::usize byteWidth)
{
    memset( destAddress, value, byteWidth );
}

void DvigEngine::Engine::MemoryInsertObjectToMemoryPool(const sint32 index, const usize offset, const usize byteWidth)
{
    DVIG_ASSERT( (Engine::m_Instance->m_MemoryPools[index].m_Address != nullptr), DebugResult::NULL_POINTER )
    DVIG_ASSERT( (Engine::m_Instance->m_MemoryPools[index].m_ByteWidth > (Engine::m_Instance->m_MemoryPools[index].m_OccupiedByteWidth + byteWidth + sizeof(MemoryObject))), DebugResult::NOT_ENOUGH_MEMORY_POOL )

    MemoryObject memObject;
    memObject.m_ByteWidth = byteWidth;
    memObject.m_Flags = MemoryObjectFlag::FREE;

    void* memoryPoolOffsetAddress = Ptr<void*>::Add( Engine::m_Instance->m_MemoryPools[index].m_Address, offset );
    Engine::m_Instance->MemoryCopy( memoryPoolOffsetAddress, &memObject, sizeof(MemoryObject) );
}