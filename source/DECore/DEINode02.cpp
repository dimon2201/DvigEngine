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
    for (deisize i = 0; i < DV_COMPONENT_DWORD_COUNT_PER_COMPONENT_COUNT; ++i) {
        this->m_ComponentBitSet[i] = 0;
    }
}

void DvigEngine2::INode::Free()
{
    this->m_ChildNodes->Free();
    this->m_Components->Free();
    this->m_HelperObjects->Free();
}

void DvigEngine2::INode::AddChildNode(INode* const node)
{
    this->m_ChildNodes->Insert( DV_NULL, node, sizeof(INode) );
}

void DvigEngine2::INode::AddHelperObject(IHelperObject* const helperObject)
{
    this->m_HelperObjects->Insert( DV_NULL, helperObject, sizeof(helperObject) );
}

DvigEngine2::INode* DvigEngine2::INode::GetChildNode(const char* USID)
{
    const deusize capacity = this->m_ChildNodes->GetCapacity();
    INode* dataAddress = (INode*)this->m_ChildNodes->GetDataAddress();
    for (deisize i = 0; i < (deisize)capacity; ++i)
    {
        if (String::CompareCharacters( (const deuchar*)&USID[0], dataAddress->GetUSID(), String::CharactersCount((const deuchar*)&USID[0]) ) == DV_TRUE)
        {
            return dataAddress;
        }

        dataAddress = Ptr<INode*>::Add( &dataAddress, sizeof(INode) );
    }

    return nullptr;
}

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

DvigEngine2::IHelperObject* DvigEngine2::INode::GetHelperObject(const char* USID)
{
    const deusize capacity = this->m_HelperObjects->GetCapacity();
    IHelperObject* dataAddress = (IHelperObject*)this->m_HelperObjects->GetDataAddress();
    for (deisize i = 0; i < (deisize)capacity; ++i)
    {
        if (String::CompareCharacters( (const deuchar*)&USID[0], dataAddress->GetUSID(), String::CharactersCount((const deuchar*)&USID[0]) ) == DV_TRUE)
        {
            return dataAddress;
        }

        dataAddress = Ptr<IHelperObject*>::Add( &dataAddress, dataAddress->m_LayoutByteWidth );
    }

    return nullptr;
}