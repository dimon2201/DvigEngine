#include "../../include/DECore.hpp"

DvigEngine::INode* DvigEngine::INode::m_RootNode = nullptr;

void DvigEngine::INode::Init()
{
    DvigEngine::Engine* engine = this->GetEngine();

    this->m_ParentNode = nullptr;
    this->m_ChildNodes = nullptr;
    this->m_Components = nullptr;
    this->m_HelperObjects = nullptr;
    engine->Create<DvigEngine::DynamicBuffer>( &this->m_ChildNodes, "_ChildNodesContainer" );
    this->m_ChildNodes->Init( 0, 1024 );
    engine->Create<DvigEngine::DynamicBuffer>( &this->m_Components, "_ChildNodesContainer" );
    this->m_Components->Init( engine->GetData()->m_ComponentStorageMemoryPoolIndex, 1024 );
    engine->Create<DvigEngine::DynamicBuffer>( &this->m_HelperObjects, "_ChildNodesContainer" );
    this->m_HelperObjects->Init( 0, 1024 );
    for (deisize i = 0; i < DV_COMPONENT_DWORD_COUNT_PER_COMPONENT_COUNT; ++i) {
        this->m_ComponentBitSet[i] = 0;
    }
}

void DvigEngine::INode::Free()
{
    this->m_ChildNodes->Free();
    this->m_Components->Free();
    this->m_HelperObjects->Free();
}

void DvigEngine::INode::AddChildNode(INode* const node)
{
    this->m_ChildNodes->Insert( DV_NULL, node, sizeof(INode) );
}

void DvigEngine::INode::AddHelperObject(IHelperObject* const helperObject)
{
    this->m_HelperObjects->Insert( DV_NULL, helperObject, sizeof(helperObject) );
}

void DvigEngine::INode::RemoveChildNode(const char* USID)
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

void DvigEngine::INode::RemoveHelperObject(const char* USID)
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

DvigEngine::INode* DvigEngine::INode::GetChildNode(const char* USID)
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

DvigEngine::IHelperObject* DvigEngine::INode::GetHelperObject(const char* USID)
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