#include "DECore.hpp"
#include "DEThirdPartyMath.hpp"

namespace DvigEngine
{
    class IWindow;
    class RenderingSystem;

    class Application : public IHelperObject
    {
        public:
            void Init();
            void Free() override final;
            template <typename T>
            IWindow* AddWindow(const char* USID, const char* caption, glm::uvec2& size)
            {
                Engine* engine = Engine::GetClassInstance();
                T* window = nullptr;
                engine->Create <T> ( &window, &USID[0] );
                window->Init( this, &caption[0], size );
                return window;
            }
            void RemoveWindow(deint32 index);
            IWindow* GetWindow(const char* USID);
            deusize GetWindowCount();
            void WaitForWindows();

        private:
    };
}