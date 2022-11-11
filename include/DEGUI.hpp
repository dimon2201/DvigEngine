#ifndef _DE_LIB_GUI_H_
#define _DE_LIB_GUI_H_

#include "DECore.hpp"

namespace DvigEngine2
{
    class Application;

    class IWindow : public IHelperObject
    {
        DV_MACRO_FRIENDS(DvigEngine2::Engine, DvigEngine2::Application)

        public:
            void Init();
            void Free() override final;
            static void Start();
            virtual void Update() = 0;

            DV_FUNCTION_INLINE deint32 GetWindowIndex() { return m_WindowIndex; }

        private:
            deint32 m_WindowIndex;
            void* m_GLFWWindow;
            MemoryObject* m_UserData = nullptr;
    };

    class WindowStack
    {
        DV_MACRO_FRIENDS(DvigEngine2::Application, DvigEngine2::IWindow)

        private:
            static void* m_GLFWWindows[DV_MAX_GUI_WINDOW_COUNT];
            static IWindow* m_WindowInstances[DV_MAX_GUI_WINDOW_COUNT];
    };
}

#endif