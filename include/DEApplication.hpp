#include "DECore.hpp"

namespace DvigEngine2
{
    class IWindow;
    class RenderingSystem;

    class ApplicationSystem : public ISystem
    {
        DV_MACRO_DECLARE_SINGLETON(ApplicationSystem, public)

        public:
            void Init();
            void Free() override final;
            template <typename T>
            IWindow* AddWindow(const char* USID)
            {
                T* window = engine->Create <T> ( &USID[0] );
                window->Init();
                return window;
            }
            void Start();

            DV_FUNCTION_INLINE RenderingSystem* GetRenderingSystem() { if (m_RenderingSystem != nullptr) { return m_RenderingSystem; } return nullptr; }

        private:
            RenderingSystem* m_RenderingSystem = nullptr;
    };
}