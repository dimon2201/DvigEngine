#ifndef _DE_LIB_GUI_H_
#define _DE_LIB_GUI_H_

#include "DECore.hpp"
#include "DEThirdPartyMath.hpp"
#include "DEThirdPartyWindow.hpp"

namespace DvigEngine
{
    class Application;

    class IWindow : public IHelperObject
    {
        DV_MACRO_FRIENDS(DvigEngine::Engine, DvigEngine::Application)

        public:
            void Init(Application* app, const char* caption, glm::uvec2& size);
            void Free() override final;
            static void Wait();
            virtual void Start() = 0;
            virtual void Update() = 0;
            void CaptureKeyState(int key);
            
            DV_FUNCTION_INLINE static IWindow* GetWindowInstance(GLFWwindow* window) { return (IWindow*)glfwGetWindowUserPointer(window); }
            DV_FUNCTION_INLINE Application* GetApplication() { return m_App; }
            DV_FUNCTION_INLINE deint32 GetWindowIndex() { return m_WindowIndex; }
            DV_FUNCTION_INLINE void* GetGLFWWindow() { return m_GLFWWindow; }
            DV_FUNCTION_INLINE int GetKeyState(int key) { return m_GLFWKeyStates[key]; }
            DV_FUNCTION_INLINE double GetMouseX() { double x = -1.0, y = -1.0; glfwGetCursorPos(this->m_GLFWWindow, &x, &y); return x; }
            DV_FUNCTION_INLINE double GetMouseY() { double x = -1.0, y = -1.0; glfwGetCursorPos(this->m_GLFWWindow, &x, &y); return y; }
            DV_FUNCTION_INLINE MemoryObject* GetUserData() { return m_UserData; }
            DV_FUNCTION_INLINE void SetMousePosition(double x, double y) { glfwSetCursorPos(this->m_GLFWWindow, x, y); }

        private:
            Application* m_App;
            deint32 m_WindowIndex;
            GLFWwindow* m_GLFWWindow;
            debool m_GLFWKeyStates[256];
            MemoryObject* m_UserData = nullptr;
    };

    class WindowStack
    {
        DV_MACRO_FRIENDS(DvigEngine::Application, DvigEngine::IWindow)

        private:
            static void* m_GLFWWindows[DV_MAX_GUI_WINDOW_COUNT];
            static IWindow* m_WindowInstances[DV_MAX_GUI_WINDOW_COUNT];
    };
}

#endif