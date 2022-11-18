#include "../../include/DECore.hpp"
#include "../../include/DERendering.hpp"

void DvigEngine2::TransformComponent::Init()
{
    this->m_Position = glm::vec3(0.0f);
    this->m_RotationEuler = glm::vec3(0.0f);
    this->m_Scale = glm::vec3(0.0f);
    this->m_WorldRotationQuaternion = glm::quat( glm::vec3(0.0f) );
    this->m_WorldTranslationMatrix = glm::mat4();
    this->m_WorldRotationMatrix = glm::mat4();
    this->m_WorldScaleMatrix = glm::mat4();
    this->m_WorldSpaceMatrix = glm::mat4();
}

void DvigEngine2::TransformComponent::Free()
{
    this->GetEngine()->Delete( this->GetMemoryObject() );
}

void DvigEngine2::TransformComponent::SetPosition(demfloat x, demfloat y, demfloat z)
{
    this->m_Position.x = x;
    this->m_Position.y = y;
    this->m_Position.z = z;
    this->m_WorldTranslationMatrix = glm::translate( this->m_Position );
    this->m_WorldSpaceMatrix = this->m_WorldTranslationMatrix * this->m_WorldRotationMatrix * this->m_WorldScaleMatrix;
}

void DvigEngine2::TransformComponent::SetRotationEuler(demfloat x, demfloat y, demfloat z)
{
    this->m_RotationEuler.x = x;
    this->m_RotationEuler.y = y;
    this->m_RotationEuler.z = z;
    this->m_WorldRotationQuaternion = glm::quat( this->m_RotationEuler );
    this->m_WorldSpaceMatrix = this->m_WorldTranslationMatrix * this->m_WorldRotationMatrix * this->m_WorldScaleMatrix;
}

void DvigEngine2::TransformComponent::SetScale(demfloat x, demfloat y, demfloat z)
{
    this->m_Scale.x = x;
    this->m_Scale.y = y;
    this->m_Scale.z = z;
    this->m_WorldScaleMatrix = glm::scale( this->m_Scale );
    this->m_WorldSpaceMatrix = this->m_WorldTranslationMatrix * this->m_WorldRotationMatrix * this->m_WorldScaleMatrix;
}