#include "../../include/DECore.hpp"
#include "../../include/DERendering.hpp"

void DvigEngine::PostProcessorComponent::Init()
{

}

void DvigEngine::PostProcessorComponent::Free()
{
    this->GetEngine()->Delete( this->GetMemoryObject() );
}