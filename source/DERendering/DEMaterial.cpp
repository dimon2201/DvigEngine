#include "../../include/DERendering.hpp"

void DvigEngine::MaterialComponent::Init()
{
    deuchar texels[32] = 
    {
        255, 255, 255, 255,
        0, 0, 0, 255,
        255, 255, 255, 255,
        0, 0, 0, 255,
    };

    deint32 textureIndex = TextureMergerSystem::AddTexture( 2, 2, (const void*)&texels[0] );
    this->m_DiffuseTexture = TextureMergerSystem::GetAtlasTexture( textureIndex );
}

void DvigEngine::MaterialComponent::Free()
{
    this->GetEngine()->Delete( this->GetMemoryObject() );
}