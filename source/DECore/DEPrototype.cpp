#include "../../include/DECore.hpp"

DvigEngine::deusize DvigEngine::Prototype::m_GlobalInstanceCount = 0;

void DvigEngine::PROTOTYPE_DATA::Init(Engine* engine, Prototype* object)
{
    const deint32 componentStorageMemoryPoolIndex = engine->GetPrivateInputData()->m_StorageMemoryPoolID;
    m_InstanceCount = 0;
    m_InstanceSubStorageAddress = Engine::Allocate( componentStorageMemoryPoolIndex, sizeof(Instancex) );
    m_InstanceSubStorageByteWidth = 0; //sizeof(Instance);
    m_InstanceLayoutByteWidth = sizeof(Instancex);

    const deint32 prototypeStorageMemoryPoolIndex = engine->GetPrivateInputData()->m_PrototypeStorageMemoryPoolID;
    m_SharedComponentCount = 0;
    m_SharedComponentSubStorageAddress = Engine::Allocate( prototypeStorageMemoryPoolIndex, sizeof(IComponent) );
    m_SharedComponentSubStorageByteWidth = 0;
    Engine::SetMemory( &m_SharedComponentBits[0], 0, 4 * DV_COMPONENT_DWORD_COUNT_PER_COMPONENT_COUNT );
}

void DvigEngine::Engine::PrototypeInstantiate(Instancex** const result, const char* SID, Prototype* const prototype)
{
    const deint32 componentStorageMemoryPoolIndex = m_Instance->GetPrivateInputData()->m_StorageMemoryPoolID;
    const deusize instanceLayoutByteWidth = prototype->GetInstanceLayoutSize();
    // void* const moveFromAddress = (void* const)((demachword)prototype->GetInstanceSubStorageAddress());
    // void* const moveToAddress = (void* const)((demachword)moveFromAddress + instanceLayoutByteWidth);
    Engine::Allocate( componentStorageMemoryPoolIndex, instanceLayoutByteWidth );

    void* globaddr = (void*)(m_Data.m_MemoryPools[componentStorageMemoryPoolIndex].GetAddress());
    for (deisize i = 0; i < Prototype::m_GlobalInstanceCount; ++i)
    {
        Instancex* const inst = (Instancex* const)globaddr;
        Prototype* parprot = inst->m_Data.m_ParentPrototype;
        Instancex* copyto = (Instancex*)((demachword)inst + parprot->GetInstanceLayoutSize());
        std::cout << (demachword)*inst->m_Createe << std::endl;
        *inst->m_Createe = (IObject*)(copyto);
        inst->m_Data.m_ComponentSubStorageAddress = (void*)((demachword)copyto + sizeof(Instancex));
        Engine::CopyMemory( copyto, inst, parprot->GetInstanceLayoutSize() );
    }
    // Engine::MoveMemory( moveToAddress, moveFromAddress, instanceLayoutByteWidth );

    Instancex* const instance = (Instancex* const)globaddr;
    Engine::SetMemory( instance, 0, instanceLayoutByteWidth );
    Engine::CopyMemory( &instance->m_SID[0], &SID[0], String::CharactersCount( (const deuchar*)SID ) );
    instance->m_Createe = (IObject**)result;
    instance->m_Data.m_ParentPrototype = prototype;
    instance->m_Data.m_ComponentCount = 0;
    instance->m_Data.m_ComponentSubStorageAddress = (void*)((demachword)instance + sizeof(Instancex));
    std::cout << "crt : " << (demachword)instance << " " << (demachword)instance->m_Data.m_ParentPrototype << std::endl;

    prototype->GetData()->m_InstanceCount += 1;
    prototype->GetData()->m_InstanceSubStorageByteWidth += instanceLayoutByteWidth;

    Prototype::m_GlobalInstanceCount += 1;

    *result = instance;
}