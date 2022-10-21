#include "../../include/DECore.hpp"

void DvigEngine::PROTOTYPE_DATA::Init(Engine* engine, Prototype* object)
{
    m_InstanceCount = 0;
    const deint32 storageMemoryPoolIndex = engine->GetPrivateInputData()->m_StorageMemoryPoolID;
    m_InstanceSubStorageAddress = Engine::Allocate( storageMemoryPoolIndex, sizeof(INSTANCE_LAYOUT) );
    m_InstanceSubStorageByteWidth = sizeof(INSTANCE_LAYOUT);
    m_InstanceLayoutByteWidth = sizeof(INSTANCE_LAYOUT);
    m_ComponentCount = 0;
    Engine::SetMemory( &m_ComponentBits[0], 0, 4 * DV_COMPONENT_DWORD_COUNT_PER_COMPONENT_COUNT );

    INSTANCE_LAYOUT* const firstInstance = (INSTANCE_LAYOUT* const)m_InstanceSubStorageAddress;
    Engine::SetMemory( firstInstance, 0, m_InstanceLayoutByteWidth );
    firstInstance->m_ParentPrototype = object;
}

DvigEngine::INSTANCE_LAYOUT* DvigEngine::Engine::PrototypeInstantiate(const char* SID, Prototype* const prototype)
{
    const deusize instanceLayoutByteWidth = prototype->GetInstanceLayoutSize();
    void* const nextAddress = (void* const)((demachword)prototype->GetInstanceSubStorageAddress() + prototype->GetInstanceSubStorageSize());
    void* const moveToAddress = (void* const)((demachword)nextAddress + instanceLayoutByteWidth);
    Engine::MoveMemory( moveToAddress, nextAddress, instanceLayoutByteWidth );

    INSTANCE_LAYOUT* const instance = (INSTANCE_LAYOUT* const)nextAddress;
    Engine::CopyMemory( &instance->m_SID[0], &SID[0], String::CharactersCount( (const deuchar*)SID ) );
    Engine::SetMemory( instance, 0, instanceLayoutByteWidth );
    instance->m_ParentPrototype = prototype;

    prototype->GetData()->m_InstanceCount += 1;
    prototype->GetData()->m_InstanceSubStorageByteWidth += instanceLayoutByteWidth;

    return instance;
}