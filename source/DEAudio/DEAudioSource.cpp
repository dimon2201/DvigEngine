#include "../../include/DEAudio.hpp"
#include <fstream>

void DvigEngine::AudioSourceComponent::Init(const char* wavFilePathOnDrive)
{
    std::ifstream fileStream( &wavFilePathOnDrive[0], std::ios::binary );

    // Read WAV header
    WAVFileHeader wavHeader;
    fileStream.read((char*)&wavHeader, sizeof(WAVFileHeader));
    
    // Create temporary PCM data buffer
    MemoryObject* tempPCMDataMemoryObject = Engine::Allocate( 0, wavHeader.DataSize );
    const void* tempPCMData = tempPCMDataMemoryObject->Unwrap<const void*>();
    fileStream.read((char*)tempPCMData, wavHeader.DataSize);

    // Close files
    fileStream.close();

    // Create OpenAL buffer

    // Create OpenAL source

    // Delete temp data
    Engine* engine = Engine::GetClassInstance();
    engine->Delete(tempPCMDataMemoryObject);
}

void DvigEngine::AudioSourceComponent::Free()
{
    this->GetEngine()->Delete( this->GetMemoryObject() );
}