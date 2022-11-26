#include "../../include/DEAudio.hpp"
#include "../../include/DERendering.hpp"
#include <windows.h>

void DvigEngine::AudioSystem::Init()
{
    
}

void DvigEngine::AudioSystem::Free()
{
    
}

void DvigEngine::AudioSystem::Play(INode* const node)
{
    TransformComponent* nodeTransform = node->GetComponent<TransformComponent>();
    AudioSourceComponent* nodeAudioSource = node->GetComponent<AudioSourceComponent>();
    if (nodeTransform == nullptr || nodeAudioSource == nullptr) { return; }
}