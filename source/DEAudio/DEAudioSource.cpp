#include "../../include/DEAudio.hpp"
#include "../../include/DEOpenAL.hpp"
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
    ALenum bufferFormat = to_al_format( wavHeader.NumberOfChannels, wavHeader.BitsPerSample );
    AL::GenBuffers( 1, &this->m_ALBuffer );
    AL::BufferData( this->m_ALBuffer, bufferFormat, tempPCMData, wavHeader.DataSize, wavHeader.Freq );

    // Create OpenAL source
    AL::GenSources( 1, &this->m_ALSource );
    AL::Sourcei( this->m_ALSource, AL_BUFFER, this->m_ALBuffer );
    AL::Sourcef( this->m_ALSource, AL_PITCH, 1 );
    AL::Sourcef( this->m_ALSource, AL_GAIN, 1 );
    AL::Source3f( this->m_ALSource, AL_POSITION, 0, 0, 0 );
    AL::Source3f( this->m_ALSource, AL_VELOCITY, 0, 0, 0 );
    AL::Sourcei( this->m_ALSource, AL_LOOPING, AL_FALSE );

    // Delete temp data
    Engine* engine = Engine::GetClassInstance();
    engine->Delete(tempPCMDataMemoryObject);
}

void DvigEngine::AudioSourceComponent::Free()
{
    AL::DeleteBuffers( 1, &this->m_ALBuffer );
    AL::DeleteSources( 1, &this->m_ALSource );
}