#include "../../include/DERendering.hpp"

void DvigEngine::MaterialComponent::Init()
{
    // DvigEngine::deuchar texels[32] = 
    // {
    //     0,   0,   0,   255,
    //     255, 0,   0,   255,
    //     0,   255, 0,   255,
    //     0,   0,   255, 255,
    // };

    // auto idx1 = DvigEngine::TextureMergerSystem::AddTexture( 2, 2, (const void*)&texels[0] );
}

void DvigEngine::MaterialComponent::Free()
{
    this->GetEngine()->Delete( this->GetMemoryObject() );
}