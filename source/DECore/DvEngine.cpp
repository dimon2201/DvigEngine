#include "../../include/DvigEngine.hpp"

#include <iostream>
#include <typeinfo>
#include <thread>

DV_MACRO_DEFINE_SINGLETON(DvigEngine::Engine)

void DvigEngine::Engine::Init(DvigEngine::ENGINE_INPUT_DATA* engineInputData)
{
    DV_ASSERT((!m_Instance))
    DV_ASSERT((engineInputData->m_Version == DV_ENGINE_VERSION_NUMBER))

    DvigEngine::MEMORY_POOL_DATA* memoryPoolsData = engineInputData->m_MemoryPoolsData;
    dvisize memoryPoolsCount = engineInputData->m_MemoryPoolsCount;

    // Count total memory occupancy for pools
    dvusize totalPoolByteWidth = 0;
    for (dvisize i = 0; i < memoryPoolsCount; ++i) { totalPoolByteWidth += memoryPoolsData[i].m_ByteWidth; }
    totalPoolByteWidth += sizeof(DvigEngine::MEMORY_POOL_DATA); // for global pool info

    // Allocate global memory pool
    void* globalMemoryPoolAddress = malloc(totalPoolByteWidth);
    void* globalMemoryPoolOffset = globalMemoryPoolAddress;
    dvusize globalMemoryPoolByteWidth = totalPoolByteWidth;
    memset(globalMemoryPoolAddress, 0, globalMemoryPoolByteWidth);

    // Update memory pools addresses based on global memory pool
    totalPoolByteWidth = 0;
    for (dvisize i = 0; i < memoryPoolsCount; ++i)
    {
        DvigEngine::MEMORY_POOL_DATA* poolInfo = &memoryPoolsData[i];
        poolInfo->m_Address = (void*)((dvusize)globalMemoryPoolOffset + totalPoolByteWidth);
        poolInfo->m_AddressOffset = poolInfo->m_Address;
        totalPoolByteWidth += poolInfo->m_ByteWidth;
    }

    // Start System memory pools
    dvmachint reservedMemoryPoolID = engineInputData->m_SystemMemoryPoolID;
    dvmachint storageMemoryPoolID = engineInputData->m_StorageMemoryPoolID;
    DvigEngine::MEMORY_POOL_DATA* reservedMemoryPoolData = &memoryPoolsData[reservedMemoryPoolID];
    DvigEngine::MEMORY_POOL_DATA* storageMemoryPoolData = &memoryPoolsData[storageMemoryPoolID];

    // Memory allocation for Engine
    m_Instance = (DvigEngine::Engine*)DvigEngine::Engine::AllocateUsingData(reservedMemoryPoolData, sizeof(DvigEngine::Engine));

    // Assign Engine data
    DvigEngine::ENGINE_DATA* engineData = (DvigEngine::ENGINE_DATA*)m_Instance->GetData();
    engineData->m_MaxThreadCount = std::thread::hardware_concurrency();
    if (engineInputData->m_RequestedThreadCount > 0 && engineInputData->m_RequestedThreadCount <= engineData->m_MaxThreadCount)
    {
        engineData->m_RequestedThreadCount = engineInputData->m_RequestedThreadCount;
    }
    else
    {
        engineData->m_RequestedThreadCount = 1;
    }


    // Memory allocation for Job Queues
    m_Instance->m_Data.m_JobQueues = (DvigEngine::JobQueue*)DvigEngine::Engine::AllocateUsingData(reservedMemoryPoolData, engineData->m_RequestedThreadCount * sizeof(DvigEngine::JobQueue));
    
    // Instantiate Job Queues
    m_Instance->m_Data.m_CurrentJobQueueCursor = 0;
    for (dvisize i = 0; i < (dvisize)engineData->m_RequestedThreadCount; ++i)
    {
        m_Instance->m_Data.m_JobQueues[i].m_Data.m_JobCount = 0;
    }

    // Allocate memory for memory pools classes
    dvusize allocClassByteWidth = (1 + memoryPoolsCount) * sizeof(DvigEngine::MemoryPool);
    DvigEngine::MemoryPool* allocMemoryPools = (DvigEngine::MemoryPool*)DvigEngine::Engine::AllocateUsingData(reservedMemoryPoolData, allocClassByteWidth);

    // Assign data for each memory pool in EngineData
    m_Instance->m_Data.m_MemoryPools = allocMemoryPools;
    for (dvisize i = 0; i < memoryPoolsCount; ++i)
    {
        DvigEngine::MEMORY_POOL_DATA* curMemoryPoolInfo = (DvigEngine::MEMORY_POOL_DATA*)m_Instance->GetMemoryPoolByID(i)->GetData();
        DvigEngine::Engine::CopyMemory(curMemoryPoolInfo, &memoryPoolsData[i], sizeof(DvigEngine::MEMORY_POOL_DATA));
    }

    // Assign Global memory pool for Engine
    DvigEngine::MEMORY_POOL_DATA globalMemoryPoolData;
    globalMemoryPoolData.m_Index = memoryPoolsCount;
    globalMemoryPoolData.m_Address = globalMemoryPoolAddress;
    globalMemoryPoolData.m_AddressOffset = globalMemoryPoolData.m_Address;
    globalMemoryPoolData.m_ByteWidth = globalMemoryPoolByteWidth;
    DvigEngine::MemoryPool* globalMemoryPool = m_Instance->GetMemoryPoolByID(memoryPoolsCount);
    DvigEngine::Engine::CopyMemory(globalMemoryPool->GetData(), &globalMemoryPoolData, sizeof(DvigEngine::MEMORY_POOL_DATA));

    // Assign Storage memory pools (both Entity and Component) for Engine
    storageMemoryPoolData->m_Index = storageMemoryPoolID;
    m_Instance->m_Data.m_MemoryPools[ storageMemoryPoolID ].GetData()->m_Index = storageMemoryPoolID;

    // Copy user input data to Engine
    DvigEngine::Engine::CopyMemory(&m_Instance->m_InputData, engineInputData, sizeof(ENGINE_INPUT_DATA));
    m_Instance->m_InputData.m_MemoryPoolsCount += 1;
    
    // Initialize registry hashmap for Engine
    m_Instance->m_RegistryData.m_StorageAddress = storageMemoryPoolData->m_AddressOffset;
    m_Instance->m_RegistryData.m_UniqueComponentCount = 0;
    m_Instance->m_RegistryData.m_UniqueSystemCount = 0;
    m_Instance->m_RegistryData.m_TypeAllocationPoolID.Init();
    m_Instance->m_RegistryData.m_Components.Init();
    m_Instance->m_RegistryData.m_Systems.Init();

    // Initialize allocation pool ID for basic types
    const dvuchar* const typeName = (const dvuchar* const)typeid(Entity).name();
    m_Instance->m_RegistryData.m_TypeAllocationPoolID.Insert( (dvuchar*)typeName, (void*)storageMemoryPoolID );
}

void DvigEngine::Engine::Free()
{
    DV_ASSERT_PTR(m_Instance)
    DV_ASSERT_PTR(((DvigEngine::ENGINE_DATA*)m_Instance->GetData())->m_MemoryPools)

    MemoryPool* globalMemoryPool = (DvigEngine::MemoryPool*)&m_Instance->GetData()->m_MemoryPools[m_Instance->m_InputData.m_MemoryPoolsCount - 1];
    
    // All engine memory gets freed here
    free(globalMemoryPool->GetData()->m_Address);
}

void* DvigEngine::Engine::Allocate(dvusize memoryPoolID, dvusize byteWidth)
{
    DV_ASSERT_PTR(m_Instance)
    DV_ASSERT(byteWidth)

    const dvusize allocByteWidth = byteWidth;
    DvigEngine::MemoryPool* const memoryPool = m_Instance->GetMemoryPoolByID(memoryPoolID);
    void* prevPoolOffset = ((DvigEngine::MEMORY_POOL_DATA*)memoryPool->GetData())->m_AddressOffset;
    ((DvigEngine::MEMORY_POOL_DATA*)memoryPool->GetData())->m_AddressOffset = (void*)((dvusize)((DvigEngine::MEMORY_POOL_DATA*)memoryPool->GetData())->m_AddressOffset + allocByteWidth);

    return prevPoolOffset;
}

DvigEngine::MemoryObject* DvigEngine::Engine::AllocateObject(dvusize memoryPoolID, dvusize byteWidth)
{
    DV_ASSERT_PTR(m_Instance)
    DV_ASSERT(byteWidth)

    dvusize allocByteWidth = sizeof(DvigEngine::MemoryObject) + byteWidth;

    DvigEngine::MEMORY_POOL_DATA* memoryPoolInfo = (DvigEngine::MEMORY_POOL_DATA*)m_Instance->GetMemoryPoolByID(memoryPoolID)->GetData();
    void* prevPoolOffset = memoryPoolInfo->m_AddressOffset;
    memoryPoolInfo->m_AddressOffset = (void*)((dvusize)memoryPoolInfo->m_AddressOffset + allocByteWidth);

    DvigEngine::MemoryObject* memoryObject = (DvigEngine::MemoryObject*)prevPoolOffset;
    DvigEngine::MEMORY_OBJECT_DATA* memoryObjectData = (DvigEngine::MEMORY_OBJECT_DATA*)memoryObject->GetData();
    memoryObjectData->m_Address = (void*)((dvusize)prevPoolOffset + sizeof(DvigEngine::MemoryObject));
    memoryObjectData->m_ByteWidth = byteWidth;
    memoryObjectData->m_MemoryPoolIndex = memoryPoolID;

    return memoryObject;
}

void* DvigEngine::Engine::AllocateUsingData(MEMORY_POOL_DATA* memoryPool, dvusize byteWidth)
{
    DV_ASSERT(byteWidth)
    DV_ASSERT_PTR(memoryPool)

    void* prevPoolOffset = memoryPool->m_AddressOffset;
    memoryPool->m_AddressOffset = (void*)((dvusize)memoryPool->m_AddressOffset + byteWidth);

    return prevPoolOffset;
}

void DvigEngine::Engine::DeleteObject(MemoryObject** ppMemoryObject)
{
    MemoryObject* memoryObject = *ppMemoryObject;
    const dvint32 memoryPoolIndex = memoryObject->GetMemoryPoolIndex();
    MemoryPool* const memoryPool = &m_Instance->m_Data.m_MemoryPools[memoryPoolIndex];

    void* curAddress = (void*)memoryObject;
    dvusize deletedObjectByteWidth = sizeof(MemoryObject) + memoryObject->GetByteWidth();
    void* nextAddress = (void*)((dvmachword)curAddress + deletedObjectByteWidth);
    
    MemoryObject* curMemoryObject = (MemoryObject*)curAddress;
    IObject* curObject = (IObject*)((dvmachword)curMemoryObject + sizeof(MemoryObject));
    MemoryObject** pCurMemoryObjectPointer = curObject->GetMemoryObject();
    IObject** pCurCreatee = curObject->GetCreatee();
    *pCurMemoryObjectPointer = nullptr;
    *pCurCreatee = nullptr;

    void* lastAddress = (void*)memoryPool->m_Data.m_AddressOffset;
    Engine::MoveMemory(curAddress, nextAddress, (dvusize)lastAddress - (dvusize)nextAddress);
    memoryPool->GetData()->m_AddressOffset = (void*)((dvmachword)memoryPool->GetData()->m_AddressOffset - deletedObjectByteWidth);
    lastAddress = (void*)memoryPool->m_Data.m_AddressOffset;

    while (curAddress < lastAddress)
    {
        MemoryObject* curMemoryObject = (MemoryObject*)curAddress;
        IObject* curObject = (IObject*)((dvmachword)curMemoryObject + sizeof(MemoryObject));
        MemoryObject** pCurMemoryObjectPointer = curObject->GetMemoryObject();
        IObject** pCurCreatee = curObject->GetCreatee();

        *pCurMemoryObjectPointer = (MemoryObject*)((dvmachword)curAddress);
        *pCurCreatee = (IObject*)((dvmachword)curAddress + sizeof(MemoryObject));
        
        curAddress = (void*)((dvmachword)curAddress + sizeof(MemoryObject) + curMemoryObject->m_Data.m_ByteWidth);
    }
}

void DvigEngine::Engine::CopyMemory(void* dstAddress, void* srcAddress, dvusize byteWidth)
{
    memcpy(dstAddress, srcAddress, byteWidth);
}

void DvigEngine::Engine::MoveMemory(void* dstAddress, void* srcAddress, dvusize byteWidth)
{
    memmove(dstAddress, srcAddress, byteWidth);
}

void DvigEngine::Engine::SetMemory(void* dstAddress, dvmachword value, dvusize byteWidth)
{
    memset(dstAddress, value, byteWidth);
}

void DvigEngine::Engine::StartThreads()
{
    for (dvisize i = 0; i < (dvisize)m_Data.m_RequestedThreadCount; ++i)
    {
        m_Data.m_JobQueues[i].m_Data.m_Thread = std::thread(
            &DvigEngine::JobQueue::Start, &m_Data.m_JobQueues[i]
        );
    }
}

void DvigEngine::Engine::StopThreads()
{
    for (dvisize i = 0; i < (dvisize)m_Data.m_RequestedThreadCount; ++i)
    {
        m_Data.m_JobQueues[i].Stop();
        m_Data.m_JobQueues[i].m_Data.m_Thread.join();
    }
}

void DvigEngine::Engine::UpdateSystems()
{
    const dvisize uniqueSystemCount = m_RegistryData.m_UniqueSystemCount;
    HashMap* const systemList = (HashMap* const)&m_RegistryData.m_Systems;
    MemoryObject* storageAddress = (MemoryObject*)m_RegistryData.m_StorageAddress; // array of all Entities means Entity Storage
    const dvisize entityCount = Entity::m_EntityCount;

    // Run through each System
    for (dvisize i = 0; i < uniqueSystemCount; ++i)
    {
        ISystem* system = (ISystem*)systemList->FindIndex( i );
        void* entityCursor = (void*)storageAddress->GetAddress();
        
        // Run through each Entity
        for (dvisize i = 0; i < entityCount; ++i)
        {
            Entity* entity = (Entity*)entityCursor;

            // Update derived system
            system->Update( m_Instance, entity );
            
            entityCursor = (void*)((dvmachword)entityCursor + entity->m_Data.m_SubStorageByteWidth);
        }
    }
}