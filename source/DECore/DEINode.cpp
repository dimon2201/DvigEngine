#include "../../include/dvigengine/DECore.hpp"

void DvigEngine::INode::Init()
{
    this->m_Childs = Engine::GetClassInstance()->Create<Collection>("_NodeChildInstanceCollection");
    this->m_Childs->Init( sizeof(cpuword), 0 );
}

void DvigEngine::INode::Free()
{
    this->m_Childs->Free();
    Engine::GetClassInstance()->MemoryDelete( this->GetMemoryObject() );
}

void DvigEngine::INode::InsertChildInstance(ICommon* instance)
{
    this->m_Childs->Insert( (const void*)&instance, instance->GetUSID() );
}

DvigEngine::ICommon* DvigEngine::INode::FindChildInstance(const char* label)
{
    return (*((ICommon**)this->m_Childs->Find( &label[0] )));
}

void DvigEngine::INode::RemoveChildInstance(const char* label)
{
    this->m_Childs->Remove( &label[0] );
}