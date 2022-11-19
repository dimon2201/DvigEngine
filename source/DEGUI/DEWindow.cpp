#include "../../include/DEGUI.hpp"
#include "../../include/DERendering.hpp"
#include "../../include/DEThirdPartyMath.hpp"
#include "../../include/DEThirdPartyWindow.hpp"

void* DvigEngine::WindowStack::m_GLFWWindows[] = {};
DvigEngine::IWindow* DvigEngine::WindowStack::m_WindowInstances[] = {};

void DvigEngine::IWindow::Init(Application* app, const char* caption, glm::uvec2& size)
{
    // Create GLFW window
    GLFWwindow* window = glfwCreateWindow(size.x, size.y, &caption[0], NULL, NULL);
    glfwMakeContextCurrent(window);

    // Init OpenGL procedures
    DvigEngine::GL4::Load();
    DvigEngine::RenderingSystem::Init();

    DV_ASSERT_PTR(window);

    // Assign to member variables
    this->m_App = app;
    this->m_GLFWWindow = (void*)window;

    // Add to global stack
    deint32 cycle = 0;
    while (DvigEngine::WindowStack::m_GLFWWindows[cycle] != nullptr && ++cycle < DV_MAX_GUI_WINDOW_COUNT);

    DV_ASSERT( cycle < DV_MAX_GUI_WINDOW_COUNT );

    DvigEngine::WindowStack::m_GLFWWindows[cycle] = this->m_GLFWWindow;
    DvigEngine::WindowStack::m_WindowInstances[cycle] = this;

    this->m_WindowIndex = cycle;
    
    // Run start function
    this->Start();
}

void DvigEngine::IWindow::Free()
{
    DvigEngine::Engine* engine = this->GetEngine();

    if (this->m_UserData != nullptr) {
        engine->Delete( this->m_UserData );
    }

    glfwDestroyWindow( (GLFWwindow*)this->m_GLFWWindow );
    this->m_GLFWWindow = nullptr;
    this->m_WindowIndex = DV_NULL;

    engine->Delete( this->GetMemoryObject() );
}

void DvigEngine::IWindow::Wait()
{
    deint32 cycle = 0;
    deusize presentWindowCount = 0;
    do
    {
        // Count non-zero windows
        presentWindowCount = 0;
        for (deint32 i = 0; i < DV_MAX_GUI_WINDOW_COUNT; ++i) {
            if (DvigEngine::WindowStack::m_GLFWWindows[i] != nullptr) { presentWindowCount += 1; }
        }

        // Wrap cycle value
        if (cycle >= DV_MAX_GUI_WINDOW_COUNT) { cycle = 0; }

        // Get windows
        GLFWwindow** pGLFWWindow = (GLFWwindow**)&DvigEngine::WindowStack::m_GLFWWindows[cycle];
        IWindow* windowInstance = DvigEngine::WindowStack::m_WindowInstances[cycle];
        cycle += 1;

        // Get next window
        if (*pGLFWWindow == nullptr) { continue; }

        // Functionality
        if (!glfwWindowShouldClose(*pGLFWWindow))
        {
            windowInstance->Update();

            glfwSwapBuffers( *pGLFWWindow );
            glfwPollEvents();
        }
        else
        {
            // Destroy window
            windowInstance->Free();

            presentWindowCount -= 1;
        }
    } while (presentWindowCount > 0);
}

void DvigEngine::IWindow::SetOnKeyPressCallback(GLFWkeyfun callback)
{
    glfwSetKeyCallback( (GLFWwindow*)this->m_GLFWWindow, callback );
}