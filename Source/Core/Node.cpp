#include "../../Include/DvigEngine/Core.hpp"

void DvigEngine::Node::Init()
{
    this->m_Childs = Engine::GetClassInstance()->Create<Collection>("_NodeChildInstanceCollection");
    this->m_Childs->Init( sizeof(cpuword), 0 );
}

void DvigEngine::Node::Free()
{
    this->m_Childs->Free();
    Engine::GetClassInstance()->MemoryDelete( this->GetMemoryObject() );
}

void DvigEngine::Node::AddChild(Object* object)
{
    this->m_Childs->Insert( (const void*)&object, object->GetUSID() );
    if (dynamic_cast<Component*>(object) != nullptr)
    {
        Component* component = (Component*)object;
        component->m_Parent = this;
    }
}

DvigEngine::Object* DvigEngine::Node::FindChild(const char* label)
{
    return (*((Object**)this->m_Childs->Find( &label[0] )));
}

void DvigEngine::Node::RemoveChild(const char* label)
{
    this->m_Childs->Remove( &label[0] );
}