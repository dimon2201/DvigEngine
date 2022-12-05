#include "../../include/dvigengine/DECore.hpp"
#include "../../include/dvigengine/DEGUI.hpp"
#include "../../include/dvigengine/DERendering.hpp"

void DvigEngine::ViewerComponent::Init()
{
    this->m_Position = glm::vec3(0.0f);
    this->m_Direction = glm::vec4( 0.0f, 0.0f, 1.0f, 1.0f ); // Left-handed (DirectX)
    this->m_RotationEuler = glm::vec3(0.0f);
    this->m_RotationQuaternion = glm::quat( glm::vec3(0.0f) );
    this->m_ViewSpaceMatrix = glm::mat4(1.0f);
    this->m_ProjectionSpaceMatrix = glm::mat4(1.0f);
}

void DvigEngine::ViewerComponent::Free()
{
    Engine::GetClassInstance()->MemoryDelete( this->GetMemoryObject() );
}

void DvigEngine::ViewerComponent::AddPositionX(mfloat value)
{
    this->m_Position.x = value;
}

void DvigEngine::ViewerComponent::AddPositionY(mfloat value)
{
    this->m_Position.y = value;
}

void DvigEngine::ViewerComponent::AddPositionZ(mfloat value)
{
    this->m_Position.z = value;
}

void DvigEngine::ViewerComponent::AddRotationEulerX(mfloat value)
{
    this->m_RotationEuler.x += glm::radians(value);
    glm::quat quatX = glm::angleAxis( this->m_RotationEuler.x, glm::vec3(1.0f, 0.0f, 0.0f) );
    glm::quat quatY = glm::angleAxis( this->m_RotationEuler.y, glm::vec3(0.0f, 1.0f, 0.0f) );
    glm::quat quatZ = glm::angleAxis( this->m_RotationEuler.z, glm::vec3(0.0f, 0.0f, 1.0f) );
    this->m_Direction = quatZ * quatY * quatX * glm::vec3(0.0f, 0.0f, 1.0f);
    this->m_ViewSpaceMatrix = glm::lookAtLH( this->m_Position, this->m_Position + this->m_Direction, glm::vec3( 0.0f, 1.0f, 0.0f ) );
}

void DvigEngine::ViewerComponent::AddRotationEulerY(mfloat value)
{
    this->m_RotationEuler.y += glm::radians(value);
    glm::quat quatX = glm::angleAxis( this->m_RotationEuler.x, glm::vec3(1.0f, 0.0f, 0.0f) );
    glm::quat quatY = glm::angleAxis( this->m_RotationEuler.y, glm::vec3(0.0f, 1.0f, 0.0f) );
    glm::quat quatZ = glm::angleAxis( this->m_RotationEuler.z, glm::vec3(0.0f, 0.0f, 1.0f) );
    this->m_Direction = quatZ * quatY * quatX * glm::vec3(0.0f, 0.0f, 1.0f);
    this->m_ViewSpaceMatrix = glm::lookAtLH( this->m_Position, this->m_Position + this->m_Direction, glm::vec3( 0.0f, 1.0f, 0.0f ) );
}

void DvigEngine::ViewerComponent::AddRotationEulerZ(mfloat value)
{
    this->m_RotationEuler.z += glm::radians(value);
    glm::quat quatX = glm::angleAxis( this->m_RotationEuler.x, glm::vec3(1.0f, 0.0f, 0.0f) );
    glm::quat quatY = glm::angleAxis( this->m_RotationEuler.y, glm::vec3(0.0f, 1.0f, 0.0f) );
    glm::quat quatZ = glm::angleAxis( this->m_RotationEuler.z, glm::vec3(0.0f, 0.0f, 1.0f) );
    this->m_Direction = quatZ * quatY * quatX * glm::vec3(0.0f, 0.0f, 1.0f);
    this->m_ViewSpaceMatrix = glm::lookAtLH( this->m_Position, this->m_Position + this->m_Direction, glm::vec3( 0.0f, 1.0f, 0.0f ) );
}

void DvigEngine::ViewerComponent::Move(mfloat value)
{
    this->m_Position += value * this->m_Direction;
    this->m_ViewSpaceMatrix = glm::lookAtLH( this->m_Position, this->m_Position + this->m_Direction, glm::vec3( 0.0f, 1.0f, 0.0f ) );
}

void DvigEngine::ViewerComponent::Strafe(mfloat value)
{
    glm::vec3 right = glm::cross( glm::vec3(0.0f, 1.0f, 0.0f), this->m_Direction );
    this->m_Position += value * right;
    this->m_ViewSpaceMatrix = glm::lookAtLH( this->m_Position, this->m_Position + this->m_Direction, glm::vec3( 0.0f, 1.0f, 0.0f ) );
}

void DvigEngine::ViewerComponent::Lift(mfloat value)
{
    this->m_Position.y += value;
    this->m_ViewSpaceMatrix = glm::lookAtLH( this->m_Position, this->m_Position + this->m_Direction, glm::vec3( 0.0f, 1.0f, 0.0f ) );
}

void DvigEngine::ViewerComponent::SetPosition(mfloat x, mfloat y, mfloat z)
{
    this->m_Position.x = x;
    this->m_Position.y = y;
    this->m_Position.z = z;
    this->m_ViewSpaceMatrix = glm::lookAtLH( this->m_Position, this->m_Position + this->m_Direction, glm::vec3( 0.0f, 1.0f, 0.0f ) );
}

void DvigEngine::ViewerComponent::SetRotationX(mfloat value)
{
    this->m_RotationEuler.x = glm::radians(value);
    glm::quat quatX = glm::angleAxis( this->m_RotationEuler.x, glm::vec3(1.0f, 0.0f, 0.0f) );
    glm::quat quatY = glm::angleAxis( this->m_RotationEuler.y, glm::vec3(0.0f, 1.0f, 0.0f) );
    glm::quat quatZ = glm::angleAxis( this->m_RotationEuler.z, glm::vec3(0.0f, 0.0f, 1.0f) );
    this->m_Direction = quatZ * quatY * quatX * glm::vec3(0.0f, 0.0f, 1.0f);
    this->m_ViewSpaceMatrix = glm::lookAtLH( this->m_Position, this->m_Position + this->m_Direction, glm::vec3( 0.0f, 1.0f, 0.0f ) );
}

void DvigEngine::ViewerComponent::SetRotationY(mfloat value)
{
    this->m_RotationEuler.y = glm::radians(value);
    glm::quat quatX = glm::angleAxis( this->m_RotationEuler.x, glm::vec3(1.0f, 0.0f, 0.0f) );
    glm::quat quatY = glm::angleAxis( this->m_RotationEuler.y, glm::vec3(0.0f, 1.0f, 0.0f) );
    glm::quat quatZ = glm::angleAxis( this->m_RotationEuler.z, glm::vec3(0.0f, 0.0f, 1.0f) );
    this->m_Direction = quatZ * quatY * quatX * glm::vec3(0.0f, 0.0f, 1.0f);
    this->m_ViewSpaceMatrix = glm::lookAtLH( this->m_Position, this->m_Position + this->m_Direction, glm::vec3( 0.0f, 1.0f, 0.0f ) );
}

void DvigEngine::ViewerComponent::SetRotationZ(mfloat value)
{
    this->m_RotationEuler.z = glm::radians(value);
    glm::quat quatX = glm::angleAxis( this->m_RotationEuler.x, glm::vec3(1.0f, 0.0f, 0.0f) );
    glm::quat quatY = glm::angleAxis( this->m_RotationEuler.y, glm::vec3(0.0f, 1.0f, 0.0f) );
    glm::quat quatZ = glm::angleAxis( this->m_RotationEuler.z, glm::vec3(0.0f, 0.0f, 1.0f) );
    this->m_Direction = quatZ * quatY * quatX * glm::vec3(0.0f, 0.0f, 1.0f);
    this->m_ViewSpaceMatrix = glm::lookAtLH( this->m_Position, this->m_Position + this->m_Direction, glm::vec3( 0.0f, 1.0f, 0.0f ) );
}

void DvigEngine::ViewerComponent::SetRotationEuler(mfloat eulerX, mfloat eulerY, mfloat eulerZ)
{
    this->m_RotationEuler.x = glm::radians(eulerX);
    this->m_RotationEuler.y = glm::radians(eulerY);
    this->m_RotationEuler.z = glm::radians(eulerZ);
    glm::quat quatX = glm::angleAxis( this->m_RotationEuler.x, glm::vec3(1.0f, 0.0f, 0.0f) );
    glm::quat quatY = glm::angleAxis( this->m_RotationEuler.y, glm::vec3(0.0f, 1.0f, 0.0f) );
    glm::quat quatZ = glm::angleAxis( this->m_RotationEuler.z, glm::vec3(0.0f, 0.0f, 1.0f) );
    this->m_Direction = quatZ * quatY * quatX * glm::vec3(0.0f, 0.0f, 1.0f);
    // this->m_Direction = glm::vec3(0.0f, 0.0f, 1.0f);
    this->m_ViewSpaceMatrix = glm::lookAtLH( this->m_Position, this->m_Position + this->m_Direction, glm::vec3( 0.0f, 1.0f, 0.0f ) );
}

void DvigEngine::ViewerComponent::SetOrthographicProjection(mfloat left, mfloat right, mfloat bottom, mfloat top)
{
    this->m_ProjectionSpaceMatrix = glm::ortho( left, right, bottom, top );
}

void DvigEngine::ViewerComponent::SetPerspectiveProjection(mfloat fov, mfloat aspect, mfloat zNear, mfloat zFar)
{
    this->m_ProjectionSpaceMatrix = glm::perspectiveLH( glm::radians(fov), aspect, zNear, zFar );
}

void DvigEngine::ViewerComponent::DoMouseLook(IWindow* window)
{
    double mouseX = window->GetMouseX();
    double mouseY = window->GetMouseY();
    if (mouseX >= window->GetWindowWidth()) {
        mouseX = 0;
        window->SetMousePrevPositionX(0);
        window->SetMousePositionX(0);
    } else if (mouseX <= 0) {
        mouseX = window->GetWindowWidth();
        window->SetMousePrevPositionX(window->GetWindowWidth());
        window->SetMousePositionX(window->GetWindowWidth());
    }
    if (mouseY >= window->GetWindowHeight()) {
        mouseY = 0;
        window->SetMousePrevPositionY(0);
        window->SetMousePositionY(0);
    } else if (mouseY <= 0) {
        mouseY = window->GetWindowHeight();
        window->SetMousePrevPositionY(window->GetWindowHeight());
        window->SetMousePositionY(window->GetWindowHeight());
    }
    if (window->GetPrevMouseX() != -1.0 && window->GetPrevMouseY() != -1.0)
    {
        double deltaX = mouseX - window->GetPrevMouseX();
        double deltaY = mouseY - window->GetPrevMouseY();
        
        mfloat rotX = this->GetRotationX();
        mfloat rotY = this->GetRotationY();
        rotX += (mfloat)deltaY / 2.5f;
        rotY += (mfloat)deltaX / 2.5f;
        if (rotX > 75.0f) {
            rotX = 75.0f;
        } else if (rotX < -75.0f) {
            rotX = -75.0f;
        }
        if (rotY < -360) {
            rotY = 360 + rotY;
        } else if (rotY > 360) {
            rotY = 360 - rotY;
        }
        
        this->SetRotationX(rotX);
        this->SetRotationY(rotY);
    }

    window->SetMousePrevPositionX(mouseX);
    window->SetMousePrevPositionY(mouseY);
}