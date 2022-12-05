#include "../../include/dvigengine/DED3D11.hpp"
#include "../../include/dvigengine/DERendering.hpp"

void DvigEngine::TransformComponent::Init()
{
    this->m_Position = glm::vec3(0.0f);
    this->m_Rotation = glm::vec3(0.0f);
    this->m_Scale = glm::vec3(1.0f);
    this->m_WorldSpaceMatrix = glm::mat4(1.0f);
}

void DvigEngine::TransformComponent::Free()
{
    Engine::GetClassInstance()->MemoryDelete( this->GetMemoryObject() );
}

void DvigEngine::TransformComponent::SetPosition(mfloat x, mfloat y, mfloat z)
{
    this->m_Position = glm::vec3(x, y, z);
}

void DvigEngine::TransformComponent::SetRotation(mfloat x, mfloat y, mfloat z)
{
    this->m_Rotation = glm::vec3(x, y, z);
}

void DvigEngine::TransformComponent::SetScale(mfloat x, mfloat y, mfloat z)
{
    this->m_Scale = glm::vec3(x, y, z);
}