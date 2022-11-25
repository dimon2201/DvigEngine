#include "../../include/DERendering.hpp"
#include <fstream>

void DvigEngine::MaterialComponent::Init(const char* rxFilePathOnDrive)
{
    std::ifstream fileStream( &rxFilePathOnDrive[0], std::ios::binary );

    // Read RX header
    RXFileHeader rxHeader;
    fileStream.read((char*)&rxHeader, sizeof(RXFileHeader));
    
    // Create temporary pixel data buffer
    MemoryObject* tempPixelDataMemoryObject = Engine::Allocate( 0, 4 * rxHeader.ImageWidth * rxHeader.ImageHeight );
    const void* tempPixelData = tempPixelDataMemoryObject->Unwrap<const void*>();
    fileStream.read((char*)tempPixelData, 4 * rxHeader.ImageWidth * rxHeader.ImageHeight);

    // Close files
    fileStream.close();

    deint32 textureIndex = TextureMergerSystem::AddTexture( rxHeader.ImageWidth, rxHeader.ImageHeight, tempPixelData );
    this->m_DiffuseTexture = TextureMergerSystem::GetAtlasTexture( textureIndex );

    // Delete temp memory
    Engine* engine = Engine::GetClassInstance();
    engine->Delete(tempPixelDataMemoryObject);
}

void DvigEngine::MaterialComponent::Free()
{
    this->GetEngine()->Delete( this->GetMemoryObject() );
}