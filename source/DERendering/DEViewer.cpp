#include "../../include/DECore.hpp"
#include "../../include/DERendering.hpp"

void DvigEngine2::ViewerComponent::Init()
{
    this->m_RotationEuler = glm::vec3(0.0f);
    this->m_ViewRotationQuaternion = glm::quat( glm::vec3(0.0f) );
    this->m_ViewSpaceMatrix = glm::mat4();
    this->m_ProjectionSpaceMatrix = glm::mat4();
}

void DvigEngine2::ViewerComponent::Free()
{
    this->GetEngine()->Delete( this->GetMemoryObject() );
}

void DvigEngine2::ViewerComponent::SetRotationEuler(demfloat x, demfloat y, demfloat z)
{
    this->m_RotationEuler.x = x;
    this->m_RotationEuler.y = y;
    this->m_RotationEuler.z = z;
    glm::quat tempQuatX = glm::angleAxis( this->m_RotationEuler.x, glm::vec3(1.0f, 0.0f, 0.0f) );
    glm::quat tempQuatY = glm::angleAxis( this->m_RotationEuler.y, glm::vec3(0.0f, 1.0f, 0.0f) );
    glm::quat tempQuatZ = glm::angleAxis( this->m_RotationEuler.z, glm::vec3(0.0f, 0.0f, 1.0f) );
    this->m_ViewRotationQuaternion = tempQuatX * tempQuatY * tempQuatZ;
    this->m_ViewSpaceMatrix = glm::mat4_cast( this->m_ViewRotationQuaternion );
}

void DvigEngine2::ViewerComponent::SetOrthographicProjection(demfloat left, demfloat right, demfloat bottom, demfloat top)
{
    this->m_ProjectionSpaceMatrix = glm::ortho( left, right, bottom, top );
}

void DvigEngine2::ViewerComponent::SetPerspectiveProjection(demfloat fov, demfloat aspect, demfloat zNear, demfloat zFar)
{
    this->m_ProjectionSpaceMatrix = glm::perspective( fov, aspect, zNear, zFar );
}