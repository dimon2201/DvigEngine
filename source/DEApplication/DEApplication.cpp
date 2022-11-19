#include "../../include/DEApplication.hpp"
#include "../../include/DEGUI.hpp"
#include "../../include/DERendering.hpp"

void DvigEngine::Application::Init()
{
    
}

void DvigEngine::Application::Free()
{
    this->GetEngine()->Delete( this->GetMemoryObject() );
}

void DvigEngine::Application::RemoveWindow(deint32 index)
{
    if (DvigEngine::WindowStack::m_WindowInstances[index] != nullptr) { DvigEngine::WindowStack::m_WindowInstances[index]->Free(); }
}

DvigEngine::IWindow* DvigEngine::Application::GetWindow(const char* USID)
{
    const deusize usidByteWidth = DvigEngine::String::CharactersCount( (const deuchar*)&USID[0] );
    DvigEngine::deint32 cycle = 0;
    for (DvigEngine::deint32 i = 0; i < DV_MAX_GUI_WINDOW_COUNT; ++i)
    {
        if (DvigEngine::WindowStack::m_WindowInstances[i] == nullptr) { continue; }
        if (DvigEngine::String::CompareCharacters( (const char*)&DvigEngine::WindowStack::m_WindowInstances[i]->GetUSID()[0],
                                                    &USID[0], usidByteWidth) == DV_TRUE) {
            return DvigEngine::WindowStack::m_WindowInstances[i];
        }
    }

    return nullptr;
}

DvigEngine::deusize DvigEngine::Application::GetWindowCount()
{
    deusize windowCount = 0;
    for (DvigEngine::deint32 i = 0; i < DV_MAX_GUI_WINDOW_COUNT; ++i)
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