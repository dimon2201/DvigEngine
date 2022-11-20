#include "../../include/DECore.hpp"
#include "../../include/DERendering.hpp"

void DvigEngine::ViewerComponent::Init()
{
    this->m_Position = glm::vec3(0.0f);
    this->m_Direction = glm::vec4( 0.0f, 0.0f, -1.0f, 1.0f ); // Right-handed (OpenGL)
    this->m_RotationEuler = glm::vec3(0.0f);
    this->m_RotationQuaternion = glm::quat( glm::vec3(0.0f) );
    this->m_ViewSpaceMatrix = glm::mat4();
    this->m_ProjectionSpaceMatrix = glm::mat4();
}

void DvigEngine::ViewerComponent::Free()
{
    this->GetEngine()->Delete( this->GetMemoryObject() );
}

void DvigEngine::ViewerComponent::AddPositionX(demfloat value)
{
    this->m_Position.x = value;
}

void DvigEngine::ViewerComponent::AddPositionY(demfloat value)
{
    this->m_Position.y = value;
}

void DvigEngine::ViewerComponent::AddPositionZ(demfloat value)
{
    this->m_Position.z = value;
}

void DvigEngine::ViewerComponent::AddRotationEulerX(demfloat value)
{
    this->m_RotationEuler.x += glm::radians(value);
    glm::quat quatX = glm::angleAxis( this->m_RotationEuler.x, glm::vec3(1.0f, 0.0f, 0.0f) );
    glm::quat quatY = glm::angleAxis( this->m_RotationEuler.y, glm::vec3(0.0f, 1.0f, 0.0f) );
    glm::quat quatZ = glm::angleAxis( this->m_RotationEuler.z, glm::vec3(0.0f, 0.0f, 1.0f) );
    this->m_Direction = quatZ * quatY * quatX * glm::vec3(0.0f, 0.0f, -1.0f);
    this->m_ViewSpaceMatrix = glm::lookAtRH( this->m_Position, this->m_Position + this->m_Direction, glm::vec3( 0.0f, 1.0f, 0.0f ) );
}

void DvigEngine::ViewerComponent::AddRotationEulerY(demfloat value)
{
    this->m_RotationEuler.y += glm::radians(value);
    glm::quat quatX = glm::angleAxis( this->m_RotationEuler.x, glm::vec3(1.0f, 0.0f, 0.0f) );
    glm::quat quatY = glm::angleAxis( this->m_RotationEuler.y, glm::vec3(0.0f, 1.0f, 0.0f) );
    glm::quat quatZ = glm::angleAxis( this->m_RotationEuler.z, glm::vec3(0.0f, 0.0f, 1.0f) );
    this->m_Direction = quatZ * quatY * quatX * glm::vec3(0.0f, 0.0f, -1.0f);
    this->m_ViewSpaceMatrix = glm::lookAtRH( this->m_Position, this->m_Position + this->m_Direction, glm::vec3( 0.0f, 1.0f, 0.0f ) );
}

void DvigEngine::ViewerComponent::AddRotationEulerZ(demfloat value)
{
    this->m_RotationEuler.z += glm::radians(value);
    glm::quat quatX = glm::angleAxis( this->m_RotationEuler.x, glm::vec3(1.0f, 0.0f, 0.0f) );
    glm::quat quatY = glm::angleAxis( this->m_RotationEuler.y, glm::vec3(0.0f, 1.0f, 0.0f) );
    glm::quat quatZ = glm::angleAxis( this->m_RotationEuler.z, glm::vec3(0.0f, 0.0f, 1.0f) );
    this->m_Direction = quatZ * quatY * quatX * glm::vec3(0.0f, 0.0f, -1.0f);
    this->m_ViewSpaceMatrix = glm::lookAtRH( this->m_Position, this->m_Position + this->m_Direction, glm::vec3( 0.0f, 1.0f, 0.0f ) );
}

void DvigEngine::ViewerComponent::Move(demfloat value)
{
    this->m_Position += value * this->m_Direction;
    this->m_ViewSpaceMatrix = glm::lookAtRH( this->m_Position, this->m_Position + this->m_Direction, glm::vec3( 0.0f, 1.0f, 0.0f ) );
}

void DvigEngine::ViewerComponent::Strafe(demfloat value)
{
    glm::vec3 right = glm::cross( glm::vec3(0.0f, 1.0f, 0.0f), this->m_Direction );
    this->m_Position += value * right;
    this->m_ViewSpaceMatrix = glm::lookAtRH( this->m_Position, this->m_Position + this->m_Direction, glm::vec3( 0.0f, 1.0f, 0.0f ) );
}

void DvigEngine::ViewerComponent::Lift(demfloat value)
{
    this->m_Position.y += value;
    this->m_ViewSpaceMatrix = glm::lookAtRH( this->m_Position, this->m_Position + this->m_Direction, glm::vec3( 0.0f, 1.0f, 0.0f ) );
}

void DvigEngine::ViewerComponent::SetPosition(demfloat x, demfloat y, demfloat z)
{
    this->m_Position.x = x;
    this->m_Position.y = y;
    this->m_Position.z = z;
    this->m_ViewSpaceMatrix = glm::lookAtRH( this->m_Position, this->m_Position + this->m_Direction, glm::vec3( 0.0f, 1.0f, 0.0f ) );
}

void DvigEngine::ViewerComponent::SetRotationX(demfloat value)
{
    this->m_RotationEuler.x = glm::radians(value);
    glm::quat quatX = glm::angleAxis( this->m_RotationEuler.x, glm::vec3(1.0f, 0.0f, 0.0f) );
    glm::quat quatY = glm::angleAxis( this->m_RotationEuler.y, glm::vec3(0.0f, 1.0f, 0.0f) );
    glm::quat quatZ = glm::angleAxis( this->m_RotationEuler.z, glm::vec3(0.0f, 0.0f, 1.0f) );
    this->m_Direction = quatZ * quatY * quatX * glm::vec3(0.0f, 0.0f, -1.0f);
    this->m_ViewSpaceMatrix = glm::lookAtRH( this->m_Position, this->m_Position + this->m_Direction, glm::vec3( 0.0f, 1.0f, 0.0f ) );
}

void DvigEngine::ViewerComponent::SetRotationY(demfloat value)
{
    this->m_RotationEuler.y = glm::radians(value);
    glm::quat quatX = glm::angleAxis( this->m_RotationEuler.x, glm::vec3(1.0f, 0.0f, 0.0f) );
    glm::quat quatY = glm::angleAxis( this->m_RotationEuler.y, glm::vec3(0.0f, 1.0f, 0.0f) );
    glm::quat quatZ = glm::angleAxis( this->m_RotationEuler.z, glm::vec3(0.0f, 0.0f, 1.0f) );
    this->m_Direction = quatZ * quatY * quatX * glm::vec3(0.0f, 0.0f, -1.0f);
    this->m_ViewSpaceMatrix = glm::lookAtRH( this->m_Position, this->m_Position + this->m_Direction, glm::vec3( 0.0f, 1.0f, 0.0f ) );
}

void DvigEngine::ViewerComponent::SetRotationZ(demfloat value)
{
    this->m_RotationEuler.z = glm::radians(value);
    glm::quat quatX = glm::angleAxis( this->m_RotationEuler.x, glm::vec3(1.0f, 0.0f, 0.0f) );
    glm::quat quatY = glm::angleAxis( this->m_RotationEuler.y, glm::vec3(0.0f, 1.0f, 0.0f) );
    glm::quat quatZ = glm::angleAxis( this->m_RotationEuler.z, glm::vec3(0.0f, 0.0f, 1.0f) );
    this->m_Direction = quatZ * quatY * quatX * glm::vec3(0.0f, 0.0f, -1.0f);
    this->m_ViewSpaceMatrix = glm::lookAtRH( this->m_Position, this->m_Position + this->m_Direction, glm::vec3( 0.0f, 1.0f, 0.0f ) );
}

void DvigEngine::ViewerComponent::SetRotationEuler(demfloat eulerX, demfloat eulerY, demfloat eulerZ)
{
    this->m_RotationEuler.x = glm::radians(eulerX);
    this->m_RotationEuler.y = glm::radians(eulerY);
    this->m_RotationEuler.z = glm::radians(eulerZ);
    glm::quat quatX = glm::angleAxis( this->m_RotationEuler.x, glm::vec3(1.0f, 0.0f, 0.0f) );
    glm::quat quatY = glm::angleAxis( this->m_RotationEuler.y, glm::vec3(0.0f, 1.0f, 0.0f) );
    glm::quat quatZ = glm::angleAxis( this->m_RotationEuler.z, glm::vec3(0.0f, 0.0f, 1.0f) );
    this->m_Direction = quatZ * quatY * quatX * glm::vec3(0.0f, 0.0f, -1.0f);
    this->m_ViewSpaceMatrix = glm::lookAtRH( this->m_Position, this->m_Position + this->m_Direction, glm::vec3( 0.0f, 1.0f, 0.0f ) );
}

void DvigEngine::ViewerComponent::SetOrthographicProjection(demfloat left, demfloat right, demfloat bottom, demfloat top)
{
    this->m_ProjectionSpaceMatrix = glm::ortho( left, right, bottom, top );
}

void DvigEngine::ViewerComponent::SetPerspectiveProjection(demfloat fov, demfloat aspect, demfloat zNear, demfloat zFar)
{
    this->m_ProjectionSpaceMatrix = glm::perspectiveRH( glm::radians(fov), aspect, zNear, zFar );
}