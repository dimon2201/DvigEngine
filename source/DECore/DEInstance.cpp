#include "../../include/DECore.hpp"

void DvigEngine::INSTANCE_DATA::Init(DvigEngine::Engine* engine, DvigEngine::Instance* instance)
{
    
}

DvigEngine::Instance::~Instance()
{
    void* curAddress = (void*)this;
    Instance* const curInstance = (Instance* const)curAddress;
    Prototype* const parentPrototype = curInstance->m_Data.m_ParentPrototype;
    const deusize curInstanceLayoutByteWidth = parentPrototype->GetInstanceLayoutSize();

    const deint32 curInstanceGlobalIndex = curInstance->m_Data.m_GlobalIndex;
    const deisize globalInstanceCount = m_Data.m_ParentPrototype->GetGlobalInstanceCount();
    
    for (deisize i = curInstanceGlobalIndex + 1; i < globalInstanceCount; ++i)
    {
        const deusize instanceLayoutByteWidth = parentPrototype->GetInstanceLayoutSize();
        curAddress = (void*)((demachword)curAddress + instanceLayoutByteWidth);
        Instance* const curInstance = (Instance* const)curAddress;
        Instance* nextInstance = (Instance*)((demachword)curInstance + instanceLayoutByteWidth);
        // Engine::CopyMemory( curInstance, nextInstance, instanceLayoutByteWidth );
        *(nextInstance->GetCreatee()) = (IObject*)((demachword)curInstance - curInstanceLayoutByteWidth);
        nextInstance->m_Data.m_ComponentSubStorageAddress = (void*)(((demachword)curInstance - curInstanceLayoutByteWidth) + sizeof(Instance));
    }

    parentPrototype->GetData()->m_InstanceCount -= 1;
    parentPrototype->GetData()->m_InstanceSubStorageByteWidth -= curInstanceLayoutByteWidth;
}