#ifndef _DE_LIB_APPLICATION_H_
#define _DE_LIB_APPLICATION_H_

#include "Core.hpp"
#include "../ThirdParty/ThirdPartyMath.hpp"

namespace DvigEngine
{
    class IWindow;

    class Application : public Node
    {
        public:
            void Init();
            void Free();

            template <typename T>
            IWindow* InsertWindow(const char* usid, const char* caption, glm::uvec2& size)
            {
                Engine* engine = Engine::GetClassInstance();
                T* window = engine->Create <T> ( &usid[0] );
                window->Init( this, &caption[0], size );
                return window;
            }
            void RemoveWindow(const sint32 index);
            void WaitForWindows();

            IWindow* GetWindow(const char* USID);
            usize GetWindowCount();
    };
}

#endif