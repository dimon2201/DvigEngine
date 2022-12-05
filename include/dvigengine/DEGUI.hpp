#ifndef _DE_LIB_GUI_H_
#define _DE_LIB_GUI_H_

#include "DECore.hpp"
#include "../thirdparty/DEThirdPartyMath.hpp"
#include "../thirdparty/DEThirdPartyWindow.hpp"

namespace DvigEngine
{
    #define DVIG_MAX_GUI_WINDOW_COUNT 16

    class Application;
    class RenderTargetGroup;

    class IWindow : public IHelper
    {
        friend Engine;
        friend Application;

        public:
            void Init(Application* app, const char* caption, glm::uvec2& size);
            void Free() {}

            virtual void Start() = 0;
            virtual void Update() = 0;
            static void Wait();
            DVIG_FUNCTION_INLINE void CaptureKeyState(int key) { this->m_GLFWKeyStates[key] = glfwGetKey( this->m_GLFWWindow, key ); }
            
            // DV_FUNCTION_INLINE static IWindow* GetWindowInstance(GLFWwindow* window) { return (IWindow*)glfwGetWindowUserPointer(window); }
            // DV_FUNCTION_INLINE Application* GetApplication() { return m_App; }
            // DV_FUNCTION_INLINE deint32 GetWindowIndex() { return m_WindowIndex; }
            // DV_FUNCTION_INLINE void* GetGLFWWindow() { return m_GLFWWindow; }
            DVIG_FUNCTION_INLINE int GetKeyState(int key) { return m_GLFWKeyStates[key]; }
            DVIG_FUNCTION_INLINE double GetMouseX() { double x = -1.0, y = -1.0; glfwGetCursorPos(this->m_GLFWWindow, &x, &y); return x; }
            DVIG_FUNCTION_INLINE double GetMouseY() { double x = -1.0, y = -1.0; glfwGetCursorPos(this->m_GLFWWindow, &x, &y); return y; }
            DVIG_FUNCTION_INLINE double GetPrevMouseX() { return this->m_PrevMouseX; }
            DVIG_FUNCTION_INLINE double GetPrevMouseY() { return this->m_PrevMouseY; }
            DVIG_FUNCTION_INLINE int GetWindowWidth() { int width = 0, height = 0; glfwGetFramebufferSize(this->m_GLFWWindow, &width, &height); return width; }
            DVIG_FUNCTION_INLINE int GetWindowHeight() { int width = 0, height = 0; glfwGetFramebufferSize(this->m_GLFWWindow, &width, &height); return height; }
            // DV_FUNCTION_INLINE RenderTargetGroup* GetRenderTargetGroup() { return m_RenderTargetGroup; }
            // DV_FUNCTION_INLINE MemoryObject* GetUserData() { return m_UserData; }
            DVIG_FUNCTION_INLINE void SetMousePositionX(double value) { glfwSetCursorPos(this->m_GLFWWindow, value, this->GetMouseY()); }
            DVIG_FUNCTION_INLINE void SetMousePositionY(double value) { glfwSetCursorPos(this->m_GLFWWindow, this->GetMouseX(), value); }
            DVIG_FUNCTION_INLINE void SetMousePosition(double x, double y) { glfwSetCursorPos(this->m_GLFWWindow, x, y); }
            DVIG_FUNCTION_INLINE void SetMousePrevPositionX(double value) { this->m_PrevMouseX = value; }
            DVIG_FUNCTION_INLINE void SetMousePrevPositionY(double value) { this->m_PrevMouseY = value; }


        private:
            static boolean m_IsInitialized;

        private:
            Application* m_App;
            sint32 m_WindowIndex;
            GLFWwindow* m_GLFWWindow;
            boolean m_GLFWKeyStates[256];
            RenderTargetGroup* m_RenderTargetGroup;
            double m_PrevMouseX;
            double m_PrevMouseY;
            MemoryObject* m_UserData = nullptr;
    };

    class WindowStack
    {
        friend Application;
        friend IWindow;

        private:
            static void* m_GLFWWindows[DVIG_MAX_GUI_WINDOW_COUNT];
            static IWindow* m_WindowInstances[DVIG_MAX_GUI_WINDOW_COUNT];
    };
}

#endif