#include "../../include/DEGUI.hpp"
#include "../../include/DERendering.hpp"
#include "../../include/DEThirdPartyMath.hpp"
#include "../../include/DEThirdPartyWindow.hpp"

void* DvigEngine2::WindowStack::m_GLFWWindows[] = {};
DvigEngine2::IWindow* DvigEngine2::WindowStack::m_WindowInstances[] = {};

void DvigEngine2::IWindow::Init(Application* app, const char* caption, glm::uvec2& size)
{
    // Create GLFW window
    GLFWwindow* window = glfwCreateWindow(size.x, size.y, &caption[0], NULL, NULL);
    glfwMakeContextCurrent(window);

    // Load OpenGL4 procedures
    // if needed
    DvigEngine2::GL4::LoadGL4();

    DV_ASSERT_PTR(window);

    // Assign to member variables
    this->m_App = app;
    this->m_GLFWWindow = (void*)window;

    // Add to global stack
    deint32 cycle = 0;
    while (DvigEngine2::WindowStack::m_GLFWWindows[cycle] != nullptr && ++cycle < DV_MAX_GUI_WINDOW_COUNT);

    DV_ASSERT( cycle < DV_MAX_GUI_WINDOW_COUNT );

    DvigEngine2::WindowStack::m_GLFWWindows[cycle] = this->m_GLFWWindow;
    DvigEngine2::WindowStack::m_WindowInstances[cycle] = this;

    this->m_WindowIndex = cycle;
    
    // Run start function
    this->Start();
}

void DvigEngine2::IWindow::Free()
{
    DvigEngine2::Engine* engine = this->GetEngine();

    if (this->m_UserData != nullptr) {
        engine->Delete( this->m_UserData );
    }

    glfwDestroyWindow( (GLFWwindow*)this->m_GLFWWindow );
    this->m_GLFWWindow = nullptr;
    this->m_WindowIndex = DV_NULL;

    engine->Delete( this->GetMemoryObject() );
}

void DvigEngine2::IWindow::Wait()
{
    deint32 cycle = 0;
    deusize presentWindowCount = 0;
    do
    {
        // Count non-zero windows
        presentWindowCount = 0;
        for (deint32 i = 0; i < DV_MAX_GUI_WINDOW_COUNT; ++i) {
            if (DvigEngine2::WindowStack::m_GLFWWindows[i] != nullptr) { presentWindowCount += 1; }
        }

        // Wrap cycle value
        if (cycle >= DV_MAX_GUI_WINDOW_COUNT) { cycle = 0; }

        // Get windows
        GLFWwindow** pGLFWWindow = (GLFWwindow**)&DvigEngine2::WindowStack::m_GLFWWindows[cycle];
        IWindow* windowInstance = DvigEngine2::WindowStack::m_WindowInstances[cycle];
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