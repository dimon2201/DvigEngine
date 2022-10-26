#include "../../include/DECore.hpp"

DvigEngine::deusize DvigEngine::Prototype::m_GlobalInstanceCount = 0;

void DvigEngine::PROTOTYPE_DATA::Init(Engine* engine, Prototype* object)
{
    const deint32 componentStorageMemoryPoolIndex = engine->GetPrivateInputData()->m_StorageMemoryPoolID;
    m_InstanceCount = 0;
    m_InstanceSubStorageAddress = Engine::ObjectAllocate( componentStorageMemoryPoolIndex, sizeof(Instance) );
    m_InstanceSubStorageByteWidth = 0; //sizeof(Instance);
    m_InstanceLayoutByteWidth = sizeof(Instance);

    const deint32 prototypeStorageMemoryPoolIndex = engine->GetPrivateInputData()->m_PrototypeStorageMemoryPoolID;
    m_SharedComponentCount = 0;
    m_SharedComponentSubStorageAddress = Engine::ObjectAllocate( prototypeStorageMemoryPoolIndex, sizeof(IComponent) );
    m_SharedComponentSubStorageByteWidth = 0;
    Engine::SetMemory( &m_SharedComponentBits[0], 0, 4 * DV_COMPONENT_DWORD_COUNT_PER_COMPONENT_COUNT );
}

void DvigEngine::Engine::PrototypeInstantiate(Instance** const result, const char* SID, Prototype* const prototype)
{
    const deint32 componentStorageMemoryPoolIndex = m_Instance->GetPrivateInputData()->m_StorageMemoryPoolID;
    const deusize instanceLayoutByteWidth = prototype->GetInstanceLayoutSize();
    MemoryObject* memoryObject = Engine::ObjectAllocate( componentStorageMemoryPoolIndex, instanceLayoutByteWidth );

    void* curAddress = m_Instance->GetMemoryPoolByID(componentStorageMemoryPoolIndex)->m_Data.m_Address;
    for (deisize i = 0; i < (deisize)Prototype::m_GlobalInstanceCount; ++i)
    {
        Instance* const curInstance = (Instance* const)((demachword)curAddress + sizeof(MemoryObject));
        Prototype* const parentPrototype = curInstance->m_Data.m_ParentPrototype;
        std::cout << (demachword)parentPrototype << std::endl;
        const deusize instanceLayoutByteWidth = parentPrototype->GetInstanceLayoutSize();
        Instance* nextInstance = (Instance*)((demachword)curInstance + instanceLayoutByteWidth + sizeof(MemoryObject));
        Engine::CopyMemory( nextInstance, curInstance, instanceLayoutByteWidth );
        *(nextInstance->GetCreatee()) = curInstance;
        nextInstance->m_Data.m_ComponentSubStorageAddress = (void*)((demachword)curInstance + sizeof(Instance));
    
        curAddress = (void*)((demachword)curAddress + instanceLayoutByteWidth + sizeof(MemoryObject));
    }
    std::cout << (demachword)memoryObject << " " << (demachword)curAddress << std::endl;

    Instance instanceOnStack;
    Instance* const instance = (Instance* const)((demachword)curAddress + sizeof(MemoryObject));
    Engine::SetMemory( instance, 0, instanceLayoutByteWidth );
    Engine::CopyMemory( instance, (const void*)&instanceOnStack, sizeof(demachword) );
    Engine::CopyMemory( &instance->m_SID[0], &SID[0], String::CharactersCount( (const deuchar*)SID ) );
    instance->m_Createe = (IObject**)result;
    instance->m_Data.m_Engine = (void*)m_Instance;
    instance->m_Data.m_GlobalIndex = Prototype::m_GlobalInstanceCount;
    instance->m_Data.m_ParentPrototype = prototype;
    instance->m_Data.m_ComponentCount = 0;
    instance->m_Data.m_ComponentSubStorageAddress = (void*)((demachword)instance + sizeof(Instance));
    std::cout << "crt : " << (demachword)instance << " " << (demachword)instance->m_Data.m_ParentPrototype << std::endl;

    prototype->GetData()->m_InstanceCount += 1;
    prototype->GetData()->m_InstanceSubStorageByteWidth += instanceLayoutByteWidth;

    Prototype::m_GlobalInstanceCount += 1;

    *result = instance;
}