#include "../../include/DEApplication.hpp"
#include "../../include/DEGUI.hpp"
#include "../../include/DERendering.hpp"

void DvigEngine2::Application::Init()
{
    DvigEngine2::RenderingSystem::Init();
}

void DvigEngine2::Application::Free()
{

}

void DvigEngine2::Application::RemoveWindow(deint32 index)
{
    if (DvigEngine2::WindowStack::m_WindowInstances[index] != nullptr) { DvigEngine2::WindowStack::m_WindowInstances[index]->Free(); }
}

void DvigEngine2::Application::WaitForWindows()
{
    DvigEngine2::IWindow::Wait();
}