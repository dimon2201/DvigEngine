#include "../../include/DEAudio.hpp"
#include "../../include/DEOpenAL.hpp"
#include "../../include/DERendering.hpp"
#include <windows.h>

ALCdevice* DvigEngine::AudioSystem::m_Device = nullptr;
ALCcontext* DvigEngine::AudioSystem::m_Context = nullptr;

void DvigEngine::AudioSystem::Init()
{
    AudioSystem::m_Device = AL::COpenDevice(NULL);
    if (!AudioSystem::m_Device) { return; }

    AudioSystem::m_Context = AL::CCreateContext(AudioSystem::m_Device, NULL);
    if (!AL::CMakeContextCurrent(AudioSystem::m_Context)) { return; }
}

void DvigEngine::AudioSystem::Free()
{
    AL::CDestroyContext(AudioSystem::m_Context);
}

void DvigEngine::AudioSystem::Play(INode* const node)
{
    TransformComponent* nodeTransform = node->GetComponent<TransformComponent>(nullptr);
    AudioSourceComponent* nodeAudioSource = node->GetComponent<AudioSourceComponent>(nullptr);
    if (nodeTransform == nullptr || nodeAudioSource == nullptr) { return; }
    
    glm::vec3 position = nodeTransform->m_Position;
    ALuint source = nodeAudioSource->m_ALSource;
    
    AL::Source3f( source, AL_POSITION, position.x, position.y, position.z );
    
    AL::SourcePlay(source);
}