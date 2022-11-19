#include "../../include/DECore.hpp"
#include "../../include/DERendering.hpp"

void DvigEngine::TransformComponent::Init()
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

void DvigEngine::TransformComponent::Free()
{
    this->GetEngine()->Delete( this->GetMemoryObject() );
}

void DvigEngine::TransformComponent::AddPositionX(demfloat value)
{
    this->m_Position.x = value;
    this->m_WorldTranslationMatrix = glm::translate( this->m_Position );
    this->m_WorldSpaceMatrix = this->m_WorldTranslationMatrix * this->m_WorldRotationMatrix * this->m_WorldScaleMatrix;
}

void DvigEngine::TransformComponent::AddPositionY(demfloat value)
{
    this->m_Position.y = value;
    this->m_WorldTranslationMatrix = glm::translate( this->m_Position );
    this->m_WorldSpaceMatrix = this->m_WorldTranslationMatrix * this->m_WorldRotationMatrix * this->m_WorldScaleMatrix;
}

void DvigEngine::TransformComponent::AddPositionZ(demfloat value)
{
    this->m_Position.z = value;
    this->m_WorldTranslationMatrix = glm::translate( this->m_Position );
    this->m_WorldSpaceMatrix = this->m_WorldTranslationMatrix * this->m_WorldRotationMatrix * this->m_WorldScaleMatrix;
}

void DvigEngine::TransformComponent::AddRotationEulerX(demfloat value)
{
    this->m_RotationEuler.x += glm::radians(value);
    glm::quat tempQuatX = glm::angleAxis( this->m_RotationEuler.x, glm::vec3(1.0f, 0.0f, 0.0f) );
    glm::quat tempQuatY = glm::angleAxis( this->m_RotationEuler.y, glm::vec3(0.0f, 1.0f, 0.0f) );
    glm::quat tempQuatZ = glm::angleAxis( this->m_RotationEuler.z, glm::vec3(0.0f, 0.0f, 1.0f) );
    this->m_WorldRotationQuaternion = tempQuatX * tempQuatY * tempQuatZ;
    this->m_WorldRotationMatrix = glm::mat4_cast( this->m_WorldRotationQuaternion );
    this->m_WorldSpaceMatrix = this->m_WorldTranslationMatrix * this->m_WorldRotationMatrix * this->m_WorldScaleMatrix;
}

void DvigEngine::TransformComponent::AddRotationEulerY(demfloat value)
{
    this->m_RotationEuler.y += glm::radians(value);
    glm::quat tempQuatX = glm::angleAxis( this->m_RotationEuler.x, glm::vec3(1.0f, 0.0f, 0.0f) );
    glm::quat tempQuatY = glm::angleAxis( this->m_RotationEuler.y, glm::vec3(0.0f, 1.0f, 0.0f) );
    glm::quat tempQuatZ = glm::angleAxis( this->m_RotationEuler.z, glm::vec3(0.0f, 0.0f, 1.0f) );
    this->m_WorldRotationQuaternion = tempQuatX * tempQuatY * tempQuatZ;
    this->m_WorldRotationMatrix = glm::mat4_cast( this->m_WorldRotationQuaternion );
    this->m_WorldSpaceMatrix = this->m_WorldTranslationMatrix * this->m_WorldRotationMatrix * this->m_WorldScaleMatrix;
}

void DvigEngine::TransformComponent::AddRotationEulerZ(demfloat value)
{
    this->m_RotationEuler.z += glm::radians(value);
    glm::quat tempQuatX = glm::angleAxis( this->m_RotationEuler.x, glm::vec3(1.0f, 0.0f, 0.0f) );
    glm::quat tempQuatY = glm::angleAxis( this->m_RotationEuler.y, glm::vec3(0.0f, 1.0f, 0.0f) );
    glm::quat tempQuatZ = glm::angleAxis( this->m_RotationEuler.z, glm::vec3(0.0f, 0.0f, 1.0f) );
    this->m_WorldRotationQuaternion = tempQuatX * tempQuatY * tempQuatZ;
    this->m_WorldRotationMatrix = glm::mat4_cast( this->m_WorldRotationQuaternion );
    this->m_WorldSpaceMatrix = this->m_WorldTranslationMatrix * this->m_WorldRotationMatrix * this->m_WorldScaleMatrix;
}

void DvigEngine::TransformComponent::AddScaleX(demfloat value)
{
    this->m_Scale.x = value;
    this->m_WorldScaleMatrix = glm::scale( this->m_Scale );
    this->m_WorldSpaceMatrix = this->m_WorldTranslationMatrix * this->m_WorldRotationMatrix * this->m_WorldScaleMatrix;
}

void DvigEngine::TransformComponent::AddScaleY(demfloat value)
{
    this->m_Scale.y = value;
    this->m_WorldScaleMatrix = glm::scale( this->m_Scale );
    this->m_WorldSpaceMatrix = this->m_WorldTranslationMatrix * this->m_WorldRotationMatrix * this->m_WorldScaleMatrix;
}

void DvigEngine::TransformComponent::AddScaleZ(demfloat value)
{
    this->m_Scale.z = value;
    this->m_WorldScaleMatrix = glm::scale( this->m_Scale );
    this->m_WorldSpaceMatrix = this->m_WorldTranslationMatrix * this->m_WorldRotationMatrix * this->m_WorldScaleMatrix;
}

void DvigEngine::TransformComponent::SetPosition(demfloat x, demfloat y, demfloat z)
{
    this->m_Position.x = x;
    this->m_Position.y = y;
    this->m_Position.z = z;
    this->m_WorldTranslationMatrix = glm::translate( this->m_Position );
    this->m_WorldSpaceMatrix = this->m_WorldTranslationMatrix * this->m_WorldRotationMatrix * this->m_WorldScaleMatrix;
}

void DvigEngine::TransformComponent::SetRotationEuler(demfloat x, demfloat y, demfloat z)
{
    this->m_RotationEuler.x = glm::radians(x);
    this->m_RotationEuler.y = glm::radians(y);
    this->m_RotationEuler.z = glm::radians(z);
    glm::quat tempQuatX = glm::angleAxis( this->m_RotationEuler.x, glm::vec3(1.0f, 0.0f, 0.0f) );
    glm::quat tempQuatY = glm::angleAxis( this->m_RotationEuler.y, glm::vec3(0.0f, 1.0f, 0.0f) );
    glm::quat tempQuatZ = glm::angleAxis( this->m_RotationEuler.z, glm::vec3(0.0f, 0.0f, 1.0f) );
    this->m_WorldRotationQuaternion = tempQuatX * tempQuatY * tempQuatZ;
    this->m_WorldRotationMatrix = glm::mat4_cast( this->m_WorldRotationQuaternion );
    this->m_WorldSpaceMatrix = this->m_WorldTranslationMatrix * this->m_WorldRotationMatrix * this->m_WorldScaleMatrix;
}

void DvigEngine::TransformComponent::SetScale(demfloat x, demfloat y, demfloat z)
{
    this->m_Scale.x = x;
    this->m_Scale.y = y;
    this->m_Scale.z = z;
    this->m_WorldScaleMatrix = glm::scale( this->m_Scale );
    this->m_WorldSpaceMatrix = this->m_WorldTranslationMatrix * this->m_WorldRotationMatrix * this->m_WorldScaleMatrix;
}