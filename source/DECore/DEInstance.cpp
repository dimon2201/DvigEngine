#include "../../include/DECore.hpp"

void DvigEngine::INSTANCE_DATA::Init(DvigEngine::Engine* engine, DvigEngine::Instance* instance)
{
    
}

DvigEngine::Instance::~Instance()
{
    /*
        Change destructor to DELETE Function
    */
    // void* curAddress = (void*)this;
    // Instance* const curInstance = (Instance* const)curAddress;
    // Prototype* const parentPrototype = curInstance->m_Data.m_ParentPrototype;
    // const deusize curInstanceLayoutByteWidth = parentPrototype->GetInstanceLayoutSize();

    // const deint32 curInstanceGlobalIndex = curInstance->m_Data.m_GlobalIndex;
    // const deisize globalInstanceCount = m_Data.m_ParentPrototype->GetGlobalInstanceCount();

    // if (curInstanceGlobalIndex == globalInstanceCount)
    // {
    //     // Last instance
    //     // Delete memory object
    //     Engine* engine = GetEngine();
    //     MemoryObject* instanceMemoryObject = (MemoryObject*)((demachword)this - sizeof(MemoryObject));
    //     engine->ObjectDelete( &instanceMemoryObject );
    // }
    // else
    // {
        
    // }

    // parentPrototype->GetData()->m_InstanceCount -= 1;
    // parentPrototype->GetData()->m_InstanceSubStorageByteWidth -= curInstanceLayoutByteWidth;

    Prototype::m_GlobalInstanceCount -= 1;
}