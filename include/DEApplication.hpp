#include "DECore.hpp"

namespace DvigEngine2
{
    class IWindow;
    class RenderingSystem;

    class Application : public IHelperObject
    {
        public:
            void Init();
            void Free() override final;
            template <typename T>
            IWindow* AddWindow(const char* USID)
            {
                Engine* engine = this->GetEngine();
                T* window = engine->Create <T> ( &USID[0] );
                window->Init();
                return window;
            }
            void RemoveWindow(deint32 index);
            void Start();

            DV_FUNCTION_INLINE RenderingSystem* GetRenderingSystem() { if (m_RenderingSystem != nullptr) { return m_RenderingSystem; } return nullptr; }

        private:
            RenderingSystem* m_RenderingSystem = nullptr;
    };
}