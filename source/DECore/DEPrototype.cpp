#include "../../include/DECore.hpp"

DvigEngine::deusize DvigEngine::Prototype::m_GlobalInstanceCount = 0;

void DvigEngine::PROTOTYPE_DATA::Init(Engine* engine, Prototype* object)
{
    const deint32 componentStorageMemoryPoolIndex = engine->GetPrivateInputData()->m_StorageMemoryPoolID;
    m_InstanceCount = 0;
    m_InstanceSubStorageAddress = Engine::Allocate( componentStorageMemoryPoolIndex, sizeof(Instance) );
    m_InstanceSubStorageByteWidth = 0; //sizeof(Instance);
    m_InstanceLayoutByteWidth = sizeof(Instance);

    const deint32 prototypeStorageMemoryPoolIndex = engine->GetPrivateInputData()->m_PrototypeStorageMemoryPoolID;
    m_SharedComponentCount = 0;
    m_SharedComponentSubStorageAddress = Engine::Allocate( prototypeStorageMemoryPoolIndex, sizeof(IComponent) );
    m_SharedComponentSubStorageByteWidth = 0;
    Engine::SetMemory( &m_SharedComponentBits[0], 0, 4 * DV_COMPONENT_DWORD_COUNT_PER_COMPONENT_COUNT );
}

DvigEngine::Prototype::~Prototype()
{
    
}

void DvigEngine::Engine::PrototypeInstantiate(Instance** const result, const char* SID, Prototype* const prototype)
{
    const deint32 componentStorageMemoryPoolIndex = m_Instance->GetPrivateInputData()->m_StorageMemoryPoolID;
    const deusize instanceLayoutByteWidth = prototype->GetInstanceLayoutSize();
    Engine::Allocate( componentStorageMemoryPoolIndex, instanceLayoutByteWidth );

    void* curAddress = (void*)(m_Data.m_MemoryPools[componentStorageMemoryPoolIndex].GetAddress());
    // for (deisize i = 0; i < Prototype::m_GlobalInstanceCount; ++i)
    // {
    //     Instance* const curInstance = (Instance* const)curAddress;
    //     Prototype* const parentPrototype = curInstance->m_Data.m_ParentPrototype;
    //     Instance* copyToAddress = (Instance*)((demachword)curInstance + parentPrototype->GetInstanceLayoutSize());
    //     std::cout << (demachword)*curInstance->m_Createe << std::endl;
    //     *curInstance->m_Createe = (IObject*)(copyToAddress);
    //     curInstance->m_Data.m_ComponentSubStorageAddress = (void*)((demachword)copyToAddress + sizeof(Instance));
    //     Engine::CopyMemory( copyToAddress, curInstance, parentPrototype->GetInstanceLayoutSize() );
    // }
    for (deisize i = 0; i < Prototype::m_GlobalInstanceCount; ++i)
    {
        Instance* const curInstance = (Instance* const)curAddress;
        Prototype* const parentPrototype = curInstance->m_Data.m_ParentPrototype;
        const deusize instanceLayoutByteWidth = parentPrototype->GetInstanceLayoutSize();
        Instance* nextInstance = (Instance*)((demachword)curInstance + instanceLayoutByteWidth);
        Engine::CopyMemory( nextInstance, curInstance, instanceLayoutByteWidth );
        *(nextInstance->GetCreatee()) = curInstance;
        nextInstance->m_Data.m_ComponentSubStorageAddress = (void*)((demachword)curInstance + sizeof(Instance));
    
        curAddress = (void*)((demachword)curAddress + instanceLayoutByteWidth);
    }

    Instance* const instance = (Instance* const)curAddress;
    Engine::SetMemory( instance, 0, instanceLayoutByteWidth );
    Engine::CopyMemory( &instance->m_SID[0], &SID[0], String::CharactersCount( (const deuchar*)SID ) );
    instance->m_Createe = (IObject**)result;
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