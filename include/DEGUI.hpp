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

        private:
            void* m_GLFWWindow;
            MemoryObject* m_UserData;
    };
}

#endif