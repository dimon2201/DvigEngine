#include "../../include/DECore.hpp"

DV_MACRO_DEFINE_SINGLETON(DvigEngine2::Engine)

void DvigEngine2::Engine::Init(DvigEngine2::EngineInputProperty* engineInputProperty)
{
    DvigEngine2::MemoryPoolProperty* memoryPoolsData = engineInputProperty->m_MemoryPoolsData;
    deisize memoryPoolsCount = engineInputProperty->m_MemoryPoolsCount;

    // Count total memory occupancy for pools
    deusize totalPoolByteWidth = 0;
    for (deisize i = 0; i < memoryPoolsCount; ++i) { totalPoolByteWidth += memoryPoolsData[i].m_ByteWidth; }
    totalPoolByteWidth += sizeof(DvigEngine2::MemoryPoolProperty); // for global pool info

    // Allocate extra global memory pool with malloc
    MemoryPoolProperty globalMemoryPoolProperty;
    const deint32 globalMemoryPoolIndex = 1 + memoryPoolsCount;
    globalMemoryPoolProperty.m_Address = malloc(totalPoolByteWidth);
    globalMemoryPoolProperty.m_AddressOffset = globalMemoryPoolProperty.m_Address;
    globalMemoryPoolProperty.m_ByteWidth = totalPoolByteWidth;
    memset(globalMemoryPoolProperty.m_Address, 0, globalMemoryPoolProperty.m_ByteWidth);
    // MemoryPool* globalMemoryPool = (MemoryPool*)globalMemoryPoolProperty.m_AddressOffset;
    // globalMemoryPoolProperty.m_AddressOffset = DvigEngine2::Ptr<void>::Add( globalMemoryPoolProperty.m_AddressOffset, sizeof(MemoryPool) );

    // Allocate extra intermediate memory pool with malloc
    MemoryPoolProperty intermediateMemoryPoolProperty;
    const deint32 intermediateMemoryPoolIndex = memoryPoolsCount;
    intermediateMemoryPoolProperty.m_Address = malloc(224 * 65536);
    intermediateMemoryPoolProperty.m_AddressOffset = intermediateMemoryPoolProperty.m_Address;
    intermediateMemoryPoolProperty.m_ByteWidth = 224 * 65536;
    memset(intermediateMemoryPoolProperty.m_Address, 0, intermediateMemoryPoolProperty.m_ByteWidth);
    // MemoryPool* intermediateMemoryPool = (MemoryPool*)intermediateMemoryPoolProperty.m_AddressOffset;
    // intermediateMemoryPoolProperty.m_AddressOffset = DvigEngine2::Ptr<void>::Add( intermediateMemoryPoolProperty.m_AddressOffset, sizeof(MemoryPool) );

    // Main memory pools constants
    const deint32 reservedMemoryPoolIndex = engineInputProperty->m_SystemMemoryPoolIndex;
    // const deint32 prototypeStorageMemoryPoolIndex = engineInputProperty->m_PrototypeStorageMemoryPoolIndex;
    const deint32 storageMemoryPoolIndex = engineInputProperty->m_StorageMemoryPoolIndex;

    // Update memory pools addresses based on global memory pool
    totalPoolByteWidth = 0;
    for (deisize i = 0; i < memoryPoolsCount; ++i)
    {
        DvigEngine2::MemoryPoolProperty* poolInfo = &memoryPoolsData[i];
        poolInfo->m_Address = (void*)((deusize)globalMemoryPoolProperty.m_AddressOffset + totalPoolByteWidth);
        poolInfo->m_AddressOffset = poolInfo->m_Address;
        totalPoolByteWidth += poolInfo->m_ByteWidth;
    }

    // Memory allocation for memory pools classes (2 Extra pools)
    deusize allocClassByteWidth = (2 + memoryPoolsCount) * sizeof(DvigEngine2::MemoryPool);
    DvigEngine2::MemoryPool* allocMemoryPools = (DvigEngine2::MemoryPool*)intermediateMemoryPoolProperty.m_AddressOffset;
    DvigEngine2::Ptr<void*>::Add( &intermediateMemoryPoolProperty.m_AddressOffset, allocClassByteWidth );

    // Memory allocation for Engine
    m_Instance = (DvigEngine2::Engine*)intermediateMemoryPoolProperty.m_AddressOffset;
    DvigEngine2::Ptr<void*>::Add( &intermediateMemoryPoolProperty.m_AddressOffset, sizeof(DvigEngine2::Engine) );
    m_Instance->m_Prop.m_MemoryPools = allocMemoryPools;

    // Copy all memory pools to Engine
    // Assign data for each memory pool in EngineData
    for (deisize i = 0; i < memoryPoolsCount; ++i)
    {
        m_Instance->m_Prop.m_MemoryPools[ i ].m_Prop.m_AddressOffset = memoryPoolsData[ i ].m_AddressOffset;
        m_Instance->m_Prop.m_MemoryPools[ i ].m_Prop.m_ByteWidth = memoryPoolsData[ i ].m_ByteWidth;
    }
    m_Instance->GetData()->m_MemoryPools[ intermediateMemoryPoolIndex ].m_Prop.m_Address = intermediateMemoryPoolProperty.m_Address;
    m_Instance->GetData()->m_MemoryPools[ intermediateMemoryPoolIndex ].m_Prop.m_AddressOffset = intermediateMemoryPoolProperty.m_AddressOffset;
    m_Instance->GetData()->m_MemoryPools[ intermediateMemoryPoolIndex ].m_Prop.m_ByteWidth = intermediateMemoryPoolProperty.m_ByteWidth;
    m_Instance->GetData()->m_MemoryPools[ globalMemoryPoolIndex ].m_Prop.m_Address = globalMemoryPoolProperty.m_Address;
    m_Instance->GetData()->m_MemoryPools[ globalMemoryPoolIndex ].m_Prop.m_AddressOffset = globalMemoryPoolProperty.m_AddressOffset;
    m_Instance->GetData()->m_MemoryPools[ globalMemoryPoolIndex ].m_Prop.m_ByteWidth = globalMemoryPoolProperty.m_ByteWidth;
    m_Instance->GetData()->m_MemoryPools[ reservedMemoryPoolIndex ].m_Prop.m_Address = memoryPoolsData[ reservedMemoryPoolIndex ].m_Address;
    m_Instance->GetData()->m_MemoryPools[ reservedMemoryPoolIndex ].m_Prop.m_AddressOffset = memoryPoolsData[ reservedMemoryPoolIndex ].m_AddressOffset;
    m_Instance->GetData()->m_MemoryPools[ reservedMemoryPoolIndex ].m_Prop.m_ByteWidth = memoryPoolsData[ reservedMemoryPoolIndex ].m_ByteWidth;
    m_Instance->GetData()->m_MemoryPools[ storageMemoryPoolIndex ].m_Prop.m_Address = memoryPoolsData[ storageMemoryPoolIndex ].m_Address;
    m_Instance->GetData()->m_MemoryPools[ storageMemoryPoolIndex ].m_Prop.m_AddressOffset = memoryPoolsData[ storageMemoryPoolIndex ].m_AddressOffset;
    m_Instance->GetData()->m_MemoryPools[ storageMemoryPoolIndex ].m_Prop.m_ByteWidth = memoryPoolsData[ storageMemoryPoolIndex ].m_ByteWidth;
    // DvigEngine2::Engine::CopyMemory( &m_Instance->GetData()->m_MemoryPools[ intermediateMemoryPoolIndex ].m_Prop, &intermediateMemoryPoolProperty, sizeof(DvigEngine2::MemoryPoolProperty) );
    // DvigEngine2::Engine::CopyMemory( &m_Instance->GetData()->m_MemoryPools[ globalMemoryPoolIndex ].m_Prop, &globalMemoryPoolIndex, sizeof(DvigEngine2::MemoryPoolProperty) );
    // DvigEngine2::Engine::CopyMemory( &m_Instance->GetData()->m_MemoryPools[ reservedMemoryPoolIndex ].m_Prop, &memoryPoolsData[reservedMemoryPoolIndex], sizeof(DvigEngine2::MemoryPoolProperty) );
    // DvigEngine2::Engine::CopyMemory( &m_Instance->GetData()->m_MemoryPools[ storageMemoryPoolIndex ].m_Prop, &memoryPoolsData[storageMemoryPoolIndex], sizeof(DvigEngine2::MemoryPoolProperty) );
    // DvigEngine2::Engine::CopyMemory( &m_Instance->GetData()->m_MemoryPools[ prototypeStorageMemoryPoolIndex ].m_Prop, &memoryPoolsData[prototypeStorageMemoryPoolIndex], sizeof(DvigEngine2::MemoryPoolProperty) );

    // Assign Engine data
    DvigEngine2::EngineProperty* engineData = (DvigEngine2::EngineProperty*)m_Instance->GetData();
    engineData->m_MaxThreadCount = std::thread::hardware_concurrency();
    if (engineInputProperty->m_RequestedThreadCount > 0 && engineInputProperty->m_RequestedThreadCount <= engineData->m_MaxThreadCount)
    {
        engineData->m_RequestedThreadCount = engineInputProperty->m_RequestedThreadCount;
    }
    else
    {
        engineData->m_RequestedThreadCount = 1;
    }

    // Memory allocation for Job Queues
    m_Instance->GetData()->m_JobQueues = (DvigEngine2::JobQueue*)DvigEngine2::Engine::Allocate(intermediateMemoryPoolIndex, engineData->m_RequestedThreadCount * sizeof(DvigEngine2::JobQueue));

    // Instantiate Job Queues
    m_Instance->GetData()->m_CurrentJobQueueCursor = 0;
    for (deisize i = 0; i < (deisize)engineData->m_RequestedThreadCount; ++i)
    {
        m_Instance->GetData()->m_JobQueues[i].m_Prop.m_JobCount = 0;
    }

    // Assign Storage memory pools (both Prototype and Component) for Engine
    // prototypeStorageMemoryPoolData->m_Index = prototypeStorageMemoryPoolID;
    // storageMemoryPoolData->m_Index = storageMemoryPoolID;
    // m_Instance->m_Data.m_MemoryPools[ prototypeStorageMemoryPoolID ].GetData()->m_Index = prototypeStorageMemoryPoolID;
    // m_Instance->m_Data.m_MemoryPools[ storageMemoryPoolID ].GetData()->m_Index = storageMemoryPoolID;

    // Copy user input data to Engine
    // DvigEngine2::Engine::CopyMemory(&m_Instance->m_InputProp, engineInputProperty, sizeof(DvigEngine2::EngineInputProperty));
    m_Instance->m_InputProp.m_MemoryPoolsCount += 2;
    
    // Initialize registry hashmap for Engine
    // m_Instance->m_RegistryData.m_StorageAddress = storageMemoryPoolData->m_AddressOffset;
    // m_Instance->m_RegistryData.m_UniqueComponentCount = 0;
    // m_Instance->m_RegistryData.m_UniqueSystemCount = 0;
    // m_Instance->m_RegistryData.m_TypeAllocationPoolID.Init();
    // m_Instance->m_RegistryData.m_Components.Init();
    // m_Instance->m_RegistryData.m_Systems.Init();

    // Initialize allocation pool ID for basic types
    // const deuchar* const typeName = (const deuchar* const)typeid(Prototype).name();
    // m_Instance->m_RegistryData.m_TypeAllocationPoolID.Insert( (deuchar*)typeName, (void*)(demachword)prototypeStorageMemoryPoolID );
}

DvigEngine2::MemoryObject* DvigEngine2::Engine::Allocate(deint32 memoryPoolIndex, deusize byteWidth)
{
    deusize allocByteWidth = sizeof(DvigEngine2::MemoryObject) + byteWidth;

    DvigEngine2::MemoryPoolProperty* memoryPoolProp = (DvigEngine2::MemoryPoolProperty*)m_Instance->GetMemoryPoolByIndex(memoryPoolIndex)->GetData();
    void* prevPoolOffset = memoryPoolProp->m_AddressOffset;
    memoryPoolProp->m_AddressOffset = (void*)((deusize)memoryPoolProp->m_AddressOffset + allocByteWidth);

    DvigEngine2::MemoryObject* memoryObject = (DvigEngine2::MemoryObject*)prevPoolOffset;
    memoryObject->GetData()->m_Address = (void*)((deusize)prevPoolOffset + sizeof(DvigEngine2::MemoryObject));
    memoryObject->GetData()->m_ByteWidth = byteWidth;
    memoryObject->GetData()->m_MemoryPoolIndex = memoryPoolIndex;

    return memoryObject;
}

void DvigEngine2::Engine::Delete(MemoryObject** ppMemoryObject)
{
    MemoryObject* curMemoryObject = *ppMemoryObject;
    const deusize deletedMemoryObjectByteWidth = sizeof(DvigEngine2::MemoryObject) + curMemoryObject->GetByteWidth();
    MemoryObject* nextMemoryObject = (MemoryObject*)DvigEngine2::Ptr<void*>::Add( (void**)&curMemoryObject, deletedMemoryObjectByteWidth );

    const deint32 memoryPoolIndex = curMemoryObject->GetMemoryPoolIndex();
    MemoryPool* const memoryPool = &m_Instance->GetData()->m_MemoryPools[memoryPoolIndex];

    IObject* curObject = curMemoryObject->Unwrap<IObject*>();
    if (curObject->m_Createe != nullptr) { *curObject->m_Createe = nullptr; }
    if (curObject->m_MemoryObject != nullptr) { *curObject->m_MemoryObject = nullptr; }

    const void* maxMemoryPoolAddress = memoryPool->GetData()->m_AddressOffset;
    const deusize moveByteWidth = (demachword)maxMemoryPoolAddress - (demachword)nextMemoryObject;
    DvigEngine2::Engine::MoveMemory( curMemoryObject, nextMemoryObject, moveByteWidth );

    maxMemoryPoolAddress = DvigEngine2::Ptr<const void*>::Subtract( &maxMemoryPoolAddress, deletedMemoryObjectByteWidth );
    while ((const void* const)curMemoryObject < maxMemoryPoolAddress)
    {
        if (curObject->m_Createe != nullptr) { *curMemoryObject->m_Createe = (IObject*)DvigEngine2::Ptr<void*>::Subtract( (void**)curMemoryObject->m_Createe, deletedMemoryObjectByteWidth ); }
        if (curObject->m_MemoryObject != nullptr) { *curMemoryObject->m_MemoryObject = (MemoryObject*)DvigEngine2::Ptr<void*>::Subtract( (void**)curMemoryObject->m_MemoryObject, deletedMemoryObjectByteWidth ); }

        curMemoryObject = DvigEngine2::Ptr<MemoryObject*>::Add( &curMemoryObject, sizeof(MemoryObject) + curMemoryObject->GetByteWidth() );
    }

    // MemoryObject* memoryObject = *ppMemoryObject;
    // const deint32 memoryPoolIndex = memoryObject->GetMemoryPoolIndex();
    // MemoryPool* const memoryPool = &m_Instance->GetData()->m_MemoryPools[memoryPoolIndex];

    // void* curAddress = (void*)memoryObject;
    // deusize deletedObjectByteWidth = sizeof(MemoryObject) + memoryObject->GetByteWidth();
    // void* nextAddress = (void*)((demachword)curAddress + deletedObjectByteWidth);

    // MemoryObject* curMemoryObject = (MemoryObject*)curAddress;
    // IObject* curObject = (IObject*)((demachword)curMemoryObject + sizeof(MemoryObject));
    // MemoryObject** pCurMemoryObjectPointer = curObject->GetMemoryObject();
    // IObject** pCurCreatee = curObject->GetCreatee();
    // *pCurMemoryObjectPointer = nullptr;
    // *pCurCreatee = nullptr;

    // void* lastAddress = (void*)memoryPool->GetData()->m_AddressOffset;
    // Engine::MoveMemory(curAddress, nextAddress, (deusize)lastAddress - (deusize)nextAddress);
    // // memoryPool->GetData()->m_AddressOffset = (void*)((demachword)memoryPool->GetData()->m_AddressOffset - deletedObjectByteWidth);
    // DvigEngine2::Ptr<void>::Subtract( &memoryPool->GetData()->m_AddressOffset, deletedObjectByteWidth );

    // while (curAddress < lastAddress)
    // {
    //     MemoryObject* curMemoryObject = (MemoryObject*)curAddress;
    //     IObject* curObject = (IObject*)((demachword)curMemoryObject + sizeof(MemoryObject));
    //     MemoryObject** pCurMemoryObjectPointer = curObject->GetMemoryObject();
    //     IObject** pCurCreatee = curObject->GetCreatee();
    //     std::cout << (demachword)curAddress << " " << (demachword)lastAddress << std::endl;

    //     *pCurMemoryObjectPointer = (MemoryObject*)((demachword)curAddress);
    //     *pCurCreatee = (IObject*)((demachword)curAddress + sizeof(MemoryObject));
        
    //     curAddress = (void*)((demachword)curAddress + sizeof(MemoryObject) + curMemoryObject->GetData()->m_ByteWidth);
    // }
}

void DvigEngine2::Engine::CopyMemory(void* dstAddress, const void* srcAddress, const deusize byteWidth)
{
    memcpy(dstAddress, srcAddress, byteWidth);
}

void DvigEngine2::Engine::MoveMemory(void* dstAddress, const void* srcAddress, const deusize byteWidth)
{
    memmove(dstAddress, srcAddress, byteWidth);
}

void DvigEngine2::Engine::SetMemory(void* dstAddress, const demachword value, const deusize byteWidth)
{
    memset(dstAddress, value, byteWidth);
}