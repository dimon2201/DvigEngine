#ifndef _DE_LIB_GUI_H_
#define _DE_LIB_GUI_H_

#include "DECore.hpp"

namespace DvigEngine2
{
    class IWindow : public IHelperObject
    {
        DV_MACRO_FRIENDS(DvigEngine2::Engine)

        public:
            void Init();
            void Free() override final;
            static void Start();
            virtual void Update() = 0;

            DV_FUNCTION_INLINE deint32 GetWindowIndex() { return m_WindowIndex; }

        public:
            static void* m_GLFWWindows[DV_MAX_GUI_WINDOW_COUNT];
            static IWindow* m_WindowInstances[DV_MAX_GUI_WINDOW_COUNT];

        private:
            deint32 m_WindowIndex;
            void* m_GLFWWindow;
            MemoryObject* m_UserData;
    };
}

#endif