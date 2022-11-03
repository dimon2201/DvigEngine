#include "../../include/DECore.hpp"

void DvigEngine2::INode::Init()
{
    DvigEngine2::Engine* engine = this->GetEngine();

    this->m_ParentNode = nullptr;
    this->m_ChildNodes = nullptr;
    this->m_Components = nullptr;
    this->m_HelperObjects = nullptr;
    engine->Create<DvigEngine2::DynamicBuffer>( &this->m_ChildNodes, "_ChildNodesContainer", nullptr );
    this->m_ChildNodes->Init( 0, 1024 );
    engine->Create<DvigEngine2::DynamicBuffer>( &this->m_Components, "_ChildNodesContainer", nullptr );
    this->m_Components->Init( engine->GetInputData()->m_ComponentStorageMemoryPoolIndex, 1024 );
    engine->Create<DvigEngine2::DynamicBuffer>( &this->m_HelperObjects, "_ChildNodesContainer", nullptr );
    this->m_HelperObjects->Init( 0, 1024 );
}

void DvigEngine2::INode::AddChildNode(INode* const node)
{
    this->m_ChildNodes->Insert( DV_NULL, node, sizeof(INode) );
}

void DvigEngine2::INode::AddComponent(IComponent* const component)
{
    IComponent* const curComponent = this->GetComponent( (const char*)component->GetUSID() );
    if ( curComponent != nullptr) { return; }
    this->m_Components->Insert( DV_NULL, component, component->m_LayoutByteWidth );
    *component->GetCreatee() = this->GetComponent( (const char*)component->GetUSID() );
}

void DvigEngine2::INode::AddHelperObject(IHelperObject* const helperObject)
{
    this->m_HelperObjects->Insert( DV_NULL, helperObject, sizeof(helperObject) );
}

void DvigEngine2::INode::RemoveComponent(const char* USID)
{
    IComponent* dataAddress = (IComponent*)m_Components->GetDataAddress();
    IComponent* removedComponent = this->GetComponent( &USID[0] );
    const deusize offset = (demachword)removedComponent - (demachword)dataAddress;
    m_Components->Remove( offset, removedComponent->m_LayoutByteWidth );

    // Update createe pointers
    IComponent* const lastAddress = Ptr<IComponent*>::Add( &dataAddress, m_Components->GetSize() );
    while (removedComponent < lastAddress)
    {
        void* curCreateeAddress = *removedComponent->GetCreatee();
        *removedComponent->GetCreatee() = (ICommon*)Ptr<void*>::Subtract( &curCreateeAddress, removedComponent->m_LayoutByteWidth );
        removedComponent = Ptr<IComponent*>::Add( &removedComponent, removedComponent->m_LayoutByteWidth );
    }
}

// DvigEngine2::INode* DvigEngine2::INode::GetChildNode(const char* USID)
// {
//     return (DvigEngine2::INode*)this->m_ChildNodes->Find( &USID[0] );
// }

DvigEngine2::IComponent* DvigEngine2::INode::GetComponent(const char* USID)
{
    const deusize capacity = m_Components->GetCapacity();
    IComponent* dataAddress = (IComponent*)m_Components->GetDataAddress();
    for (deisize i = 0; i < (deisize)capacity; ++i)
    {
        if (String::CompareCharacters( (const deuchar*)&USID[0], dataAddress->GetUSID(), String::CharactersCount((const deuchar*)&USID[0]) ) == DV_TRUE)
        {
            return dataAddress;
        }

        dataAddress = Ptr<IComponent*>::Add( &dataAddress, dataAddress->m_LayoutByteWidth );
    }

    return nullptr;
}

// DvigEngine2::IHelperObject* DvigEngine2::INode::GetHelperObject(const char* USID)
// {
//     return (DvigEngine2::IHelperObject*)this->m_HelperObjects->Find( &USID[0] );
// }