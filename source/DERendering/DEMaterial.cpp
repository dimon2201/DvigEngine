#include "../../include/DERendering.hpp"

void DvigEngine::MaterialComponent::Init()
{
    
}

void DvigEngine::MaterialComponent::Free()
{
    this->GetEngine()->Delete( this->GetMemoryObject() );
}