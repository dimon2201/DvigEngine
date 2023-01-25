#include "../../Include/DvigEngine/Application.hpp"
#include "../../Include/DvigEngine/GUI.hpp"

void DvigEngine::Application::Init()
{
    
}

void DvigEngine::Application::Free()
{
}

void DvigEngine::Application::RemoveWindow(sint32 index)
{
    if (DvigEngine::WindowStack::m_WindowInstances[index] != nullptr) { DvigEngine::WindowStack::m_WindowInstances[index]->Free(); }
}

DvigEngine::IWindow* DvigEngine::Application::GetWindow(const char* USID)
{
    const usize usidByteWidth = StringSystem::GetByteWidth( &USID[0] );
    for (sint32 i = 0; i < DVIG_MAX_GUI_WINDOW_COUNT; ++i)
    {
        if (WindowStack::m_WindowInstances[i] == nullptr) { continue; }
        if (StringSystem::GetStringsEqual( (const char*)&DvigEngine::WindowStack::m_WindowInstances[i]->GetUSID()[0],
                                            &USID[0]
            ) == DVIG_TRUE) {
            return DvigEngine::WindowStack::m_WindowInstances[i];
        }
    }

    return nullptr;
}

DvigEngine::usize DvigEngine::Application::GetWindowCount()
{
    usize windowCount = 0;
    for (sint32 i = 0; i < DVIG_MAX_GUI_WINDOW_COUNT; ++i)
    {
        if (DvigEngine::WindowStack::m_WindowInstances[i] != nullptr) {
            windowCount += 1;
        }
    }

    return windowCount;
}

void DvigEngine::Application::WaitForWindows()
{
    DvigEngine::IWindow::Wait();
}