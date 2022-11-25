#include "../../include/DEApplication.hpp"
#include "../../include/DECore.hpp"
#include "../../include/DERendering.hpp"

DvigEngine::Engine* DvigEngine::Engine::m_EngineInstance = nullptr;
DvigEngine::deint32 DvigEngine::Engine::m_GlobalComponentIndex = 0;

DvigEngine::Engine::Engine(DvigEngine::EngineInputInfo* engineInputInfo)
{
    // Get thread info
    deusize requestedThreadCount = engineInputInfo->m_RequestedThreadCount;
    const deusize maxThreadCount = std::thread::hardware_concurrency();
    if (requestedThreadCount > 0 && requestedThreadCount <= maxThreadCount) {
        requestedThreadCount = engineInputInfo->m_RequestedThreadCount;
    } else {
        requestedThreadCount = 1;
    }

    // Count all memory pools byte width
    deusize mallocByteWidth = 0;
    for (deisize i = 0; i < engineInputInfo->m_MemoryPoolsCount; ++i) { mallocByteWidth += engineInputInfo->m_MemoryPoolsData[i].m_ByteWidth; }

    // Single-allocated data block
    const void* mallocAddress = malloc( mallocByteWidth );

    // Assign proper pointers to memory pools
    void* movingAddress = (void*)mallocAddress;
    for (deisize i = 0; i < engineInputInfo->m_MemoryPoolsCount; ++i)
    {
        engineInputInfo->m_MemoryPoolsData[i].m_Address = movingAddress;
        engineInputInfo->m_MemoryPoolsData[i].m_AddressOffset = movingAddress;
        movingAddress = Ptr<void*>::Add( &movingAddress, engineInputInfo->m_MemoryPoolsData[i].m_ByteWidth );
    }

    // Allocate memory for Engine
    // const deusize allocEngineByteWidth = sizeof(DvigEngine::Engine);
    // m_Instance = (DvigEngine::Engine*)engineInputProperty->m_MemoryPoolsData[ 0 ].m_AddressOffset;
    // engineInputProperty->m_MemoryPoolsData[ 0 ].m_AddressOffset = DvigEngine::Ptr<void*>::Add( &engineInputProperty->m_MemoryPoolsData[ 0 ].m_AddressOffset, allocEngineByteWidth );

    m_EngineInstance = this;

    // Allocate memory for Engine Memory pools
    const deusize allocMemoryPoolsByteWidth = engineInputInfo->m_MemoryPoolsCount * sizeof(DvigEngine::MemoryPool);
    this->m_Info.m_MemoryPools = (DvigEngine::MemoryPool*)engineInputInfo->m_MemoryPoolsData[ 0 ].m_AddressOffset;
    engineInputInfo->m_MemoryPoolsData[ 0 ].m_AddressOffset = DvigEngine::Ptr<void*>::Add( &engineInputInfo->m_MemoryPoolsData[ 0 ].m_AddressOffset, allocMemoryPoolsByteWidth );

    // Allocate memory for Engine Job queues
    // const deusize allocJobQueuesByteWidth = requestedThreadCount * sizeof(DvigEngine::JobQueue);
    // this->m_Prop.m_JobQueues = (DvigEngine::JobQueue*)engineInputProperty->m_MemoryPoolsData[ 0 ].m_AddressOffset;
    // engineInputProperty->m_MemoryPoolsData[ 0 ].m_AddressOffset = DvigEngine::Ptr<void*>::Add( &engineInputProperty->m_MemoryPoolsData[ 0 ].m_AddressOffset, allocJobQueuesByteWidth );

    // Copy input data to Engine data
    for (deisize i = 0; i < engineInputInfo->m_MemoryPoolsCount; ++i)
    {
        this->GetData()->m_MemoryPools[i].m_AllocationCount = 1;
        this->GetData()->m_MemoryPools[i].m_Address = engineInputInfo->m_MemoryPoolsData[i].m_Address;
        this->GetData()->m_MemoryPools[i].m_AddressOffset = engineInputInfo->m_MemoryPoolsData[i].m_AddressOffset;
        this->GetData()->m_MemoryPools[i].m_ByteWidth = engineInputInfo->m_MemoryPoolsData[i].m_ByteWidth;
        this->GetData()->m_MemoryPools[i].m_OccupiedByteWidth = 0;

        // Put stub memory object
        this->GetData()->m_MemoryPools[i].m_OccupiedByteWidth += sizeof(MemoryObject) +
                                                            ((demachword)this->GetData()->m_MemoryPools[i].m_AddressOffset - (demachword)this->GetData()->m_MemoryPools[i].m_Address);
        MemoryObject* stubAfter = (MemoryObject*)this->GetData()->m_MemoryPools[i].m_AddressOffset;
        stubAfter->m_FreeFlag = DV_TRUE | DV_XMACRO_MEMORY_OBJECT_STUB;
        stubAfter->m_AllocatedByteWidth = this->GetData()->m_MemoryPools[i].m_ByteWidth -
                                          this->GetData()->m_MemoryPools[i].m_OccupiedByteWidth;
    }
    
    this->GetData()->m_Version = engineInputInfo->m_Version;
    this->GetData()->m_MemoryPoolsCount = engineInputInfo->m_MemoryPoolsCount;
    this->GetData()->m_SystemMemoryPoolIndex = engineInputInfo->m_SystemMemoryPoolIndex;
    this->GetData()->m_ComponentStorageMemoryPoolIndex = engineInputInfo->m_ComponentStorageMemoryPoolIndex;
    this->GetData()->m_MaxThreadCount = maxThreadCount;
    this->GetData()->m_RequestedThreadCount = requestedThreadCount;
    this->GetData()->m_CurrentJobQueueCursor = 0u;
    this->GetData()->m_UserData = nullptr;

    // Hardcoded creation of registry objects
    // Create registered components hash map
    MemoryObject* registeredComponentsHashMapMemoryObject = Engine::Allocate(0, sizeof(HashMap));
    HashMap registeredComponentsHashMapObjectOnStack;
    HashMap* registeredComponentsHashMapObject = registeredComponentsHashMapMemoryObject->Unwrap<HashMap*>();
    DvigEngine::Engine::MemoryCopy( registeredComponentsHashMapObject, &registeredComponentsHashMapObjectOnStack, sizeof(demachword) ); // copy vpointer
    registeredComponentsHashMapObject->SetUSIDAndUIIDAndMemoryObjectAndEngine( (deuchar*)"_RegistryComponentsHashMap", Engine::GetGlobalUIID(), registeredComponentsHashMapMemoryObject, this );
    Registry::m_Components = registeredComponentsHashMapObject;
    Registry::m_Components->Init(0, 128, sizeof(HashMapKeyValuePair), 1024);
    
    // Create createes hash map
    MemoryObject* createesHashMapMemoryObject = Engine::Allocate(0, sizeof(HashMap));
    HashMap createesHashMapObjectOnStack;
    HashMap* createesHashMapObject = createesHashMapMemoryObject->Unwrap<HashMap*>();
    DvigEngine::Engine::MemoryCopy( createesHashMapObject, &createesHashMapObjectOnStack, sizeof(demachword) ); // copy vpointer
    createesHashMapObject->SetUSIDAndUIIDAndMemoryObjectAndEngine( (deuchar*)"_RegistryCreateesHashMap", Engine::GetGlobalUIID(), createesHashMapMemoryObject, this );
    Registry::m_Instances = createesHashMapObject;
    Registry::m_Instances->Init(0, 128, sizeof(HashMapKeyValuePair), 1024);
    
    // Create allocation memory pool index hash map
    MemoryObject* allocPoolIndexHashMapMemoryObject = Engine::Allocate(0, sizeof(HashMap));
    HashMap allocPoolIndexObjectOnStack;
    HashMap* allocPoolIndexMapObject = allocPoolIndexHashMapMemoryObject->Unwrap<HashMap*>();
    DvigEngine::Engine::MemoryCopy( allocPoolIndexMapObject, &allocPoolIndexObjectOnStack, sizeof(demachword) ); // copy vpointer
    allocPoolIndexMapObject->SetUSIDAndUIIDAndMemoryObjectAndEngine( (deuchar*)"_RegistryAllocPoolIndexHashMap", Engine::GetGlobalUIID(), allocPoolIndexHashMapMemoryObject, this );
    Registry::m_AllocPoolIndices = allocPoolIndexMapObject;
    Registry::m_AllocPoolIndices->Init(0, 128, sizeof(HashMapKeyValuePair), 1024);

    // INode global root node
    this->Create<DvigEngine::INode>( &DvigEngine::INode::m_RootNode, "_RootNode" );
    DvigEngine::INode::m_RootNode->Init();

    // Memory pool index for built-in types
    Registry::GetAllocPoolIndices()->Insert( typeid(IComponent).name(), (void*)(demachword)engineInputInfo->m_ComponentStorageMemoryPoolIndex );

    // Init GLFW
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
}

DvigEngine::MemoryObject* DvigEngine::Engine::Allocate(deint32 memoryPoolIndex, deusize byteWidth)
{
    /* Old fragmention-protected model */
    // deusize allocByteWidth = sizeof(DvigEngine::MemoryObject) + byteWidth;

    // DvigEngine::MemoryPool* memoryPool = (DvigEngine::MemoryPool*)m_Instance->GetMemoryPoolByIndex(memoryPoolIndex);
    // void* prevPoolOffset = memoryPool->m_AddressOffset;
    // memoryPool->m_AddressOffset = (void*)((deusize)memoryPool->m_AddressOffset + allocByteWidth);

    // DvigEngine::MemoryObject* memoryObject = (DvigEngine::MemoryObject*)prevPoolOffset;
    // memoryObject->m_Address = (void*)((deusize)prevPoolOffset + sizeof(DvigEngine::MemoryObject));
    // memoryObject->m_ByteWidth = byteWidth;
    // memoryObject->m_MemoryPoolIndex = memoryPoolIndex;

    // const deusize allocByteWidth = sizeof(DvigEngine::MemoryObject) + byteWidth;

    // DvigEngine::MemoryPool* memoryPool = (DvigEngine::MemoryPool*)m_EngineInstance->GetMemoryPoolByIndex(memoryPoolIndex);
    // void* prevPoolOffset = memoryPool->m_AddressOffset;

    // DvigEngine::deint32 allocationIndex = 0;
    // DvigEngine::MemoryObject* memoryObject = (DvigEngine::MemoryObject*)prevPoolOffset;
    // for (allocationIndex = 0; allocationIndex < memoryPool->m_AllocationCount; ++allocationIndex) {
    //     if (memoryObject->m_FreeFlag == DV_TRUE && memoryObject->m_ByteWidth >= byteWidth) { break; }
    //     memoryObject = Ptr<DvigEngine::MemoryObject*>::Add( &memoryObject, sizeof(DvigEngine::MemoryObject) + memoryObject->m_ByteWidth );
    // }

    // // Populate one
    // memoryObject->m_MemoryObject = memoryObject;
    // memoryObject->m_Address = (void*)((deusize)memoryObject + sizeof(DvigEngine::MemoryObject));
    // memoryObject->m_ByteWidth = byteWidth;
    // memoryObject->m_FreeFlag = DV_FALSE;
    // memoryObject->m_MemoryPoolIndex = memoryPoolIndex;
    
    // m_EngineInstance->GetData()->m_MemoryPools[memoryPoolIndex].m_OccupiedByteWidth += allocByteWidth;

    // // Create stub allocation if chosen
    // // allocation index is last
    // DvigEngine::MemoryObject* stubMemoryObject = Ptr<DvigEngine::MemoryObject*>::Add( &memoryObject, allocByteWidth );
    // stubMemoryObject->m_Address = nullptr;
    // stubMemoryObject->m_ByteWidth =     (demachword)m_EngineInstance->GetData()->m_MemoryPools[memoryPoolIndex].m_ByteWidth -
    //                                     (demachword)m_EngineInstance->GetData()->m_MemoryPools[memoryPoolIndex].m_OccupiedByteWidth;
    // stubMemoryObject->m_FreeFlag = DV_TRUE;
    // stubMemoryObject->m_MemoryPoolIndex = memoryPoolIndex;

    // return memoryObject;
    
    MemoryPool* memoryPool = (MemoryPool*)m_EngineInstance->GetMemoryPoolByIndex(memoryPoolIndex);
	void* memoryPoolOffset = memoryPool->m_AddressOffset;
	const deusize memoryPoolAllocationCount = memoryPool->m_AllocationCount;

	MemoryObject* memoryObject = (MemoryObject*)memoryPoolOffset;
	deint32 cursor;
	for (cursor = 0; cursor < memoryPool->m_AllocationCount; ++cursor)
	{
		if ((memoryObject->m_FreeFlag & DV_TRUE) == DV_TRUE && memoryObject->m_AllocatedByteWidth > sizeof(MemoryObject) + byteWidth) {
			break;
		}

		memoryObject = Ptr<MemoryObject*>::Add(&memoryObject, sizeof(MemoryObject) + memoryObject->m_AllocatedByteWidth);
	}

	memoryPool->m_OccupiedByteWidth += byteWidth;

	memoryObject->m_ByteWidth = byteWidth;
	if ((memoryObject->m_FreeFlag & DV_XMACRO_MEMORY_OBJECT_STUB) == DV_XMACRO_MEMORY_OBJECT_STUB) {
		memoryObject->m_AllocatedByteWidth = memoryObject->m_ByteWidth;
	}
	memoryObject->m_FreeFlag = DV_FALSE;

	if (cursor == memoryPool->m_AllocationCount - 1) {
		memoryPool->m_AllocationCount += 1;
		memoryPool->m_OccupiedByteWidth += sizeof(MemoryObject);
		MemoryObject* stubOffset = Ptr<MemoryObject*>::Add(&memoryObject, sizeof(MemoryObject) + memoryObject->m_ByteWidth);
		stubOffset->m_FreeFlag = DV_TRUE | DV_XMACRO_MEMORY_OBJECT_STUB;
		stubOffset->m_AllocatedByteWidth = memoryPool->m_ByteWidth - memoryPool->m_OccupiedByteWidth;
	}

	return memoryObject;
}

void DvigEngine::Engine::Delete(MemoryObject* memoryObject)
{
    /* Old fragmention-protected model */
    // MemoryObject* curMemoryObject = *ppMemoryObject;
    // const deint32 memoryPoolIndex = curMemoryObject->GetMemoryPoolIndex();
    // MemoryPool* const memoryPool = &m_Instance->GetData()->m_MemoryPools[memoryPoolIndex];

    // const deusize deletedMemoryObjectByteWidth = sizeof(DvigEngine::MemoryObject) + curMemoryObject->GetByteWidth();

    // void* maxMemoryPoolAddress = memoryPool->m_AddressOffset;
    // MemoryObject* nextMemoryObject = (MemoryObject*)DvigEngine::Ptr<void*>::Add( (void**)&curMemoryObject, deletedMemoryObjectByteWidth );
    // const deusize moveByteWidth = (demachword)maxMemoryPoolAddress - (demachword)nextMemoryObject;
    // DvigEngine::Engine::MoveMemory( curMemoryObject, nextMemoryObject, moveByteWidth );
    
    // memoryPool->m_AddressOffset = Ptr<void*>::Subtract( &memoryPool->m_AddressOffset, deletedMemoryObjectByteWidth );
    // maxMemoryPoolAddress = memoryPool->m_AddressOffset;

    // while (curMemoryObject < maxMemoryPoolAddress)
    // {
    //     ICommon* curObject = curMemoryObject->Unwrap<ICommon*>();

    //     // Decrease all access pointers
    //     deint32 cycle = 0;
    //     while (curObject->m_AccessPointers[cycle] != nullptr && ++cycle < DV_MAX_ACCESS_POINTERS) {
    //         *curObject->m_AccessPointers[cycle] = (ICommon*)DvigEngine::Ptr<void*>::Subtract( (void**)curObject->m_AccessPointers[cycle], deletedMemoryObjectByteWidth );
    //     }

    //     // Decrease memory object pointer
    //     if (curMemoryObject->m_MemoryObject != nullptr) { *curMemoryObject->m_MemoryObject = (MemoryObject*)DvigEngine::Ptr<void*>::Subtract( (void**)curMemoryObject->m_MemoryObject, deletedMemoryObjectByteWidth ); }

    //     curMemoryObject = DvigEngine::Ptr<MemoryObject*>::Add( &curMemoryObject, sizeof(MemoryObject) + curMemoryObject->GetByteWidth() );
    // }

    memoryObject->m_FreeFlag = DV_TRUE;
}

void DvigEngine::Engine::MemoryCopy(void* dstAddress, const void* srcAddress, const deusize byteWidth)
{
    deuchar* const pDst = (deuchar* const)dstAddress;
    deuchar* const pSrc = (deuchar* const)srcAddress;
    for (deint32 i = 0; i < byteWidth; ++i) {
        pDst[i] = pSrc[i];
    }
}

void DvigEngine::Engine::MemoryMove(void* dstAddress, const void* srcAddress, const deusize byteWidth)
{
    deuchar* const pDst = (deuchar* const)dstAddress;
    deuchar* const pSrc = (deuchar* const)srcAddress;
    for (deint32 i = 0; i < byteWidth; ++i) {
        pDst[i] = pSrc[i];
    }
}

void DvigEngine::Engine::MemorySet(void* dstAddress, const demachword value, const deusize byteWidth)
{
    deuchar* const pDst = (deuchar* const)dstAddress;
    for (deint32 i = 0; i < byteWidth; ++i) {
        pDst[i] = (deuchar)value;
    }
}