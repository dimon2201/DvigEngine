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

void DvigEngine2::ViewerComponent::SetRotation(glm::vec3 eye, demfloat eulerX, demfloat eulerY, demfloat eulerZ)
{
    this->m_RotationEuler.x = glm::radians(eulerX);
    this->m_RotationEuler.y = glm::radians(eulerY);
    this->m_RotationEuler.z = glm::radians(eulerZ);
    glm::quat tempQuatX = glm::angleAxis( this->m_RotationEuler.x, glm::vec3(1.0f, 0.0f, 0.0f) );
    glm::quat tempQuatY = glm::angleAxis( this->m_RotationEuler.y, glm::vec3(0.0f, 1.0f, 0.0f) );
    glm::quat tempQuatZ = glm::angleAxis( this->m_RotationEuler.z, glm::vec3(0.0f, 0.0f, 1.0f) );
    glm::quat tempQuat = tempQuatX * tempQuatY * tempQuatZ;
    glm::mat4 tempMat4 = glm::mat4_cast( tempQuat );
    glm::vec4 dirInit4 = glm::vec4( 0.0f, 0.0f, -1.0f, 1.0f ); // Right-handed (OpenGL)
    dirInit4 = tempMat4 * dirInit4;
    glm::vec3 dirInit3 = glm::vec3( dirInit4.x, dirInit4.y, dirInit4.z );
    this->m_ViewSpaceMatrix = glm::lookAtRH( eye, eye + dirInit3, glm::vec3( 0.0f, 1.0f, 0.0f ) );
}

void DvigEngine2::ViewerComponent::SetOrthographicProjection(demfloat left, demfloat right, demfloat bottom, demfloat top)
{
    this->m_ProjectionSpaceMatrix = glm::ortho( left, right, bottom, top );
}

void DvigEngine2::ViewerComponent::SetPerspectiveProjection(demfloat fov, demfloat aspect, demfloat zNear, demfloat zFar)
{
    this->m_ProjectionSpaceMatrix = glm::perspective( fov, aspect, zNear, zFar );
}