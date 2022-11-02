#include "../../include/DECore.hpp"

void DvigEngine2::INode::Init()
{
    DvigEngine2::Engine* engine = this->GetEngine();

    this->m_ParentNode = nullptr;
    this->m_ChildNodes = nullptr;
    this->m_Components = nullptr;
    this->m_HelperObjects = nullptr;
    engine->Create<DvigEngine2::HashMap>( &this->m_ChildNodes, "_ChildNodesContainer", nullptr );
    this->m_ChildNodes->Init( 128, sizeof(DvigEngine2::HashMapKeyValuePair), 1024 );
    engine->Create<DvigEngine2::HashMap>( &this->m_Components, "_ComponentsContainer", nullptr );
    this->m_Components->Init( 128, sizeof(DvigEngine2::HashMapKeyValuePair), 1024 );
    engine->Create<DvigEngine2::HashMap>( &this->m_HelperObjects, "_HelperObjectsContainer", nullptr );
    this->m_HelperObjects->Init( 128, sizeof(DvigEngine2::HashMapKeyValuePair), 1024 );
}

void DvigEngine2::INode::AddChildNode(INode* const node)
{
    this->m_ChildNodes->Insert((const char*)node->GetSID(), node);
}

void DvigEngine2::INode::AddComponent(IComponent* const component)
{
    this->m_Components->Insert((const char*)component->GetSID(), component);
}

void DvigEngine2::INode::AddHelperObject(IHelperObject* const helperObject)
{
    this->m_HelperObjects->Insert((const char*)helperObject->GetSID(), helperObject);
}

void DvigEngine2::INode::RemoveChildNode(const char* SID)
{
    this->m_ChildNodes->Remove(&SID[0]);
}

DvigEngine2::INode* DvigEngine2::INode::GetChildNode(const char* SID)
{
    return (DvigEngine2::INode*)this->m_ChildNodes->Find( &SID[0] );
}

DvigEngine2::IComponent* DvigEngine2::INode::GetComponent(const char* SID)
{
    return (DvigEngine2::IComponent*)this->m_Components->Find( &SID[0] );
}

DvigEngine2::IHelperObject* DvigEngine2::INode::GetHelperObject(const char* SID)
{
    return (DvigEngine2::IHelperObject*)this->m_HelperObjects->Find( &SID[0] );
}