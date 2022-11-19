#include "../../include/DECore.hpp"
#include "../../include/DERendering.hpp"

void DvigEngine2::ViewerComponent::Init()
{
    this->m_Position = glm::vec3(0.0f);
    this->m_Direction = glm::vec4( 0.0f, 0.0f, -1.0f, 1.0f ); // Right-handed (OpenGL)
    this->m_RotationEuler = glm::vec3(0.0f);
    this->m_ViewRotationQuaternion = glm::quat( glm::vec3(0.0f) );
    this->m_ViewSpaceMatrix = glm::mat4();
    this->m_ProjectionSpaceMatrix = glm::mat4();
}

void DvigEngine2::ViewerComponent::Free()
{
    this->GetEngine()->Delete( this->GetMemoryObject() );
}

void DvigEngine2::ViewerComponent::AddPositionX(demfloat value)
{
    this->m_Position.x = value;
}

void DvigEngine2::ViewerComponent::AddPositionY(demfloat value)
{
    this->m_Position.y = value;
}

void DvigEngine2::ViewerComponent::AddPositionZ(demfloat value)
{
    this->m_Position.z = value;
}

void DvigEngine2::ViewerComponent::AddRotationEulerX(demfloat value)
{
    this->m_Direction.x += glm::radians(value);
    glm::quat tempQuatX = glm::angleAxis( this->m_RotationEuler.x, glm::vec3(1.0f, 0.0f, 0.0f) );
    glm::quat tempQuatY = glm::angleAxis( this->m_RotationEuler.y, glm::vec3(0.0f, 1.0f, 0.0f) );
    glm::quat tempQuatZ = glm::angleAxis( this->m_RotationEuler.z, glm::vec3(0.0f, 0.0f, 1.0f) );
    glm::quat tempQuat = tempQuatX * tempQuatY * tempQuatZ;
    glm::mat4 tempMat4 = glm::mat4_cast( tempQuat );
    this->m_Direction = tempMat4 * this->m_Direction;
    glm::vec3 dirInit3 = glm::vec3( this->m_Direction.x, this->m_Direction.y, this->m_Direction.z );
    this->m_ViewSpaceMatrix = glm::lookAtRH( this->m_Position, this->m_Position + dirInit3, glm::vec3( 0.0f, 1.0f, 0.0f ) );
}

void DvigEngine2::ViewerComponent::AddRotationEulerY(demfloat value)
{
    this->m_Direction.y += glm::radians(value);
    glm::quat tempQuatX = glm::angleAxis( this->m_RotationEuler.x, glm::vec3(1.0f, 0.0f, 0.0f) );
    glm::quat tempQuatY = glm::angleAxis( this->m_RotationEuler.y, glm::vec3(0.0f, 1.0f, 0.0f) );
    glm::quat tempQuatZ = glm::angleAxis( this->m_RotationEuler.z, glm::vec3(0.0f, 0.0f, 1.0f) );
    glm::quat tempQuat = tempQuatX * tempQuatY * tempQuatZ;
    glm::mat4 tempMat4 = glm::mat4_cast( tempQuat );
    this->m_Direction = tempMat4 * this->m_Direction;
    glm::vec3 dirInit3 = glm::vec3( this->m_Direction.x, this->m_Direction.y, this->m_Direction.z );
    this->m_ViewSpaceMatrix = glm::lookAtRH( this->m_Position, this->m_Position + dirInit3, glm::vec3( 0.0f, 1.0f, 0.0f ) );
}

void DvigEngine2::ViewerComponent::AddRotationEulerZ(demfloat value)
{
    this->m_Direction.z += glm::radians(value);
    glm::quat tempQuatX = glm::angleAxis( this->m_RotationEuler.x, glm::vec3(1.0f, 0.0f, 0.0f) );
    glm::quat tempQuatY = glm::angleAxis( this->m_RotationEuler.y, glm::vec3(0.0f, 1.0f, 0.0f) );
    glm::quat tempQuatZ = glm::angleAxis( this->m_RotationEuler.z, glm::vec3(0.0f, 0.0f, 1.0f) );
    glm::quat tempQuat = tempQuatX * tempQuatY * tempQuatZ;
    glm::mat4 tempMat4 = glm::mat4_cast( tempQuat );
    this->m_Direction = tempMat4 * this->m_Direction;
    glm::vec3 dirInit3 = glm::vec3( this->m_Direction.x, this->m_Direction.y, this->m_Direction.z );
    this->m_ViewSpaceMatrix = glm::lookAtRH( this->m_Position, this->m_Position + dirInit3, glm::vec3( 0.0f, 1.0f, 0.0f ) );
}

void DvigEngine2::ViewerComponent::SetPosition(demfloat x, demfloat y, demfloat z)
{
    this->m_Position.x = x;
    this->m_Position.y = y;
    this->m_Position.z = z;
}

void DvigEngine2::ViewerComponent::SetRotationEuler(demfloat eulerX, demfloat eulerY, demfloat eulerZ)
{
    this->m_RotationEuler.x = glm::radians(eulerX);
    this->m_RotationEuler.y = glm::radians(eulerY);
    this->m_RotationEuler.z = glm::radians(eulerZ);
    glm::quat tempQuatX = glm::angleAxis( this->m_RotationEuler.x, glm::vec3(1.0f, 0.0f, 0.0f) );
    glm::quat tempQuatY = glm::angleAxis( this->m_RotationEuler.y, glm::vec3(0.0f, 1.0f, 0.0f) );
    glm::quat tempQuatZ = glm::angleAxis( this->m_RotationEuler.z, glm::vec3(0.0f, 0.0f, 1.0f) );
    glm::quat tempQuat = tempQuatX * tempQuatY * tempQuatZ;
    glm::mat4 tempMat4 = glm::mat4_cast( tempQuat );
    this->m_Direction = tempMat4 * this->m_Direction;
    glm::vec3 dirInit3 = glm::vec3( this->m_Direction.x, this->m_Direction.y, this->m_Direction.z );
    this->m_ViewSpaceMatrix = glm::lookAtRH( this->m_Position, this->m_Position + dirInit3, glm::vec3( 0.0f, 1.0f, 0.0f ) );
}

void DvigEngine2::ViewerComponent::SetOrthographicProjection(demfloat left, demfloat right, demfloat bottom, demfloat top)
{
    this->m_ProjectionSpaceMatrix = glm::ortho( left, right, bottom, top );
}

void DvigEngine2::ViewerComponent::SetPerspectiveProjection(demfloat fov, demfloat aspect, demfloat zNear, demfloat zFar)
{
    this->m_ProjectionSpaceMatrix = glm::perspectiveRH( glm::radians(fov), aspect, zNear, zFar );
}