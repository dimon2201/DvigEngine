#include "../../Include/DvigEngine/D3D11.hpp"
#include "../../Include/DvigEngine/Rendering.hpp"

void DvigEngine::TransformComponent::Init()
{
    this->m_Position = glm::vec3(0.0f);
    this->m_Rotation = glm::vec3(0.0f);
    this->m_Scale = glm::vec3(1.0f);
    this->m_WorldSpaceMatrix = glm::mat4(1.0f);
}

void DvigEngine::TransformComponent::Free()
{
}

void DvigEngine::TransformComponent::SetPosition(mfloat x, mfloat y, mfloat z)
{
    this->m_Position = glm::vec3(x, y, z);
    this->m_WorldSpaceMatrix = glm::translate(this->m_Position) * glm::scale(this->m_Scale);
}

void DvigEngine::TransformComponent::SetRotation(mfloat x, mfloat y, mfloat z)
{
    this->m_Rotation = glm::vec3(x, y, z);
}

void DvigEngine::TransformComponent::SetScale(mfloat x, mfloat y, mfloat z)
{
    this->m_Scale = glm::vec3(x, y, z);
}