#include "../../include/DEApplication.hpp"
#include "../../include/DEGUI.hpp"
#include "../../include/DERendering.hpp"

void DvigEngine2::Application::Init()
{
    
}

void DvigEngine2::Application::Free()
{
    this->GetEngine()->Delete( this->GetMemoryObject() );
}

void DvigEngine2::Application::RemoveWindow(deint32 index)
{
    if (DvigEngine2::WindowStack::m_WindowInstances[index] != nullptr) { DvigEngine2::WindowStack::m_WindowInstances[index]->Free(); }
}

DvigEngine2::IWindow* DvigEngine2::Application::GetWindow(const char* USID)
{
    const deusize usidByteWidth = DvigEngine2::String::CharactersCount( (const deuchar*)&USID[0] );
    DvigEngine2::deint32 cycle = 0;
    for (DvigEngine2::deint32 i = 0; i < DV_MAX_GUI_WINDOW_COUNT; ++i)
    {
        if (DvigEngine2::WindowStack::m_WindowInstances[i] == nullptr) { continue; }
        if (DvigEngine2::String::CompareCharacters( (const char*)&DvigEngine2::WindowStack::m_WindowInstances[i]->GetUSID()[0],
                                                    &USID[0], usidByteWidth) == DV_TRUE) {
            return DvigEngine2::WindowStack::m_WindowInstances[i];
        }
    }

    return nullptr;
}

DvigEngine2::deusize DvigEngine2::Application::GetWindowCount()
{
    deusize windowCount = 0;
    for (DvigEngine2::deint32 i = 0; i < DV_MAX_GUI_WINDOW_COUNT; ++i)
    {
        if (DvigEngine2::WindowStack::m_WindowInstances[i] != nullptr) {
            windowCount += 1;
        }
    }

    return windowCount;
}

void DvigEngine2::Application::WaitForWindows()
{
    DvigEngine2::IWindow::Wait();
}