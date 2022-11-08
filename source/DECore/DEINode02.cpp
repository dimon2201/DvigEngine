#include "../../include/DECore.hpp"

DvigEngine2::INode* DvigEngine2::INode::m_RootNode = nullptr;

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
    this->m_Components->Init( engine->GetData()->m_ComponentStorageMemoryPoolIndex, 1024 );
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

void DvigEngine2::INode::RemoveChildNode(const char* USID)
{
    const deusize capacity = this->m_ChildNodes->GetCapacity();
    INode** dataAddress = (INode**)this->m_ChildNodes->GetDataAddress();
    for (deisize i = 0; i < (deisize)capacity; ++i)
    {
        INode* node = *dataAddress;

        if (node->m_ChildNodes->GetCapacity() > 0) {
            node->RemoveChildNode( &USID[0] );
        }
        else
        {
            if (String::CompareCharacters( &USID[0], (const char*)node->GetUSID(), String::CharactersCount((const deuchar*)&USID[0]) ) == DV_TRUE) {
                const deisize offset = (demachword)node - (demachword)this->m_ChildNodes->GetDataAddress();
                this->m_ChildNodes->Remove( offset, sizeof(INode) );
                return;
            }
        }

        dataAddress = Ptr<INode**>::Add( &dataAddress, sizeof(INode) );
    }
}

void DvigEngine2::INode::RemoveHelperObject(const char* USID)
{
    const deusize capacity = this->m_HelperObjects->GetCapacity();
    IHelperObject* dataAddress = (IHelperObject*)this->m_HelperObjects->GetDataAddress();
    for (deisize i = 0; i < (deisize)capacity; ++i)
    {
        if (String::CompareCharacters( &USID[0], (const char*)dataAddress->GetUSID(), String::CharactersCount((const deuchar*)&USID[0]) ) == DV_TRUE)
        {
            const deisize offset = (demachword)dataAddress - (demachword)this->m_HelperObjects->GetDataAddress();
            this->m_HelperObjects->Remove( offset, dataAddress->m_LayoutByteWidth );
            return;
        }

        dataAddress = Ptr<IHelperObject*>::Add( &dataAddress, dataAddress->m_LayoutByteWidth );
    }
}

DvigEngine2::INode* DvigEngine2::INode::GetChildNode(const char* USID)
{
    const deusize capacity = this->m_ChildNodes->GetCapacity();
    INode* dataAddress = (INode*)this->m_ChildNodes->GetDataAddress();
    for (deisize i = 0; i < (deisize)capacity; ++i)
    {   
        if (String::CompareCharacters( &USID[0], (const char*)dataAddress->GetUSID(), String::CharactersCount((const deuchar*)&USID[0]) ) == DV_TRUE) {
            return dataAddress;
        }

        if (dataAddress->m_ChildNodes->GetCapacity() > 0) {
            return dataAddress->GetChildNode( &USID[0] );
        }

        dataAddress = Ptr<INode*>::Add( &dataAddress, sizeof(INode) );
    }

    return nullptr;
}

DvigEngine2::IComponent* DvigEngine2::INode::GetComponent(const char* USID)
{
    const deusize capacity = m_Components->GetCapacity();
    IComponent** dataAddress = (IComponent**)m_Components->GetDataAddress();
    for (deisize i = 0; i < (deisize)capacity; ++i)
    {
        IComponent* component = *dataAddress;

        if (String::CompareCharacters( &USID[0], (const char*)component->GetUSID(), String::CharactersCount((const deuchar*)&USID[0]) ) == DV_TRUE) {
            return component;
        }

        dataAddress = Ptr<IComponent**>::Add( &dataAddress, sizeof(IComponent**) );
    }

    return nullptr;
}

DvigEngine2::IHelperObject* DvigEngine2::INode::GetHelperObject(const char* USID)
{
    const deusize capacity = this->m_HelperObjects->GetCapacity();
    IHelperObject* dataAddress = (IHelperObject*)this->m_HelperObjects->GetDataAddress();
    for (deisize i = 0; i < (deisize)capacity; ++i)
    {
        if (String::CompareCharacters( &USID[0], (const char*)dataAddress->GetUSID(), String::CharactersCount((const deuchar*)&USID[0]) ) == DV_TRUE)
        {
            return dataAddress;
        }

        dataAddress = Ptr<IHelperObject*>::Add( &dataAddress, dataAddress->m_LayoutByteWidth );
    }

    return nullptr;
}