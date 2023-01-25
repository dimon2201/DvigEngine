#include "../../Include/DvigEngine/D3D11.hpp"
#include "../../Include/DvigEngine/GUI.hpp"
#include "../../Include/DvigEngine/Rendering.hpp"

DvigEngine::boolean DvigEngine::IWindow::m_IsInitialized = DVIG_FALSE;
void* DvigEngine::WindowStack::m_GLFWWindows[] = {};
DvigEngine::IWindow* DvigEngine::WindowStack::m_WindowInstances[] = {};

void DvigEngine::IWindow::Init(Application* app, const char* caption, glm::uvec2& size)
{
    if (m_IsInitialized == DVIG_FALSE) {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    }

    // Create GLFW window
    GLFWwindow* window = glfwCreateWindow(size.x, size.y, &caption[0], NULL, NULL);
    glfwMakeContextCurrent(window);

    // Set user data
    glfwSetWindowUserPointer( window, this );

    // Init OpenGL procedures and Engine systems
    // Create Framebuffer
    if (DvigEngine::IWindow::m_IsInitialized == DVIG_FALSE)
    {
        D3D11::Init( glfwGetWin32Window(window), size );
        RenderingSystem::Init();

        // Init OpenGL and systems
        // DvigEngine::GL4::Init();
        // DvigEngine::D3D11::Init( glfwGetWin32Window(window), size );
        // DvigEngine::RenderingSystem::Init();
        // DvigEngine::TextureMergerSystem::Init( DV_NULL, DV_NULL, DV_NULL );
        // DvigEngine::AudioSystem::Init();

        // Create Render target group for window
        // this->m_RenderTargetGroup = Engine::GetClassInstance()->Create<RenderTargetGroup>( "_RenderTargetGroup" );
        // this->m_RenderTargetGroup->Init(nullptr, nullptr, size);
    }

    DVIG_ASSERT( (window != nullptr), DebugResult::NULL_POINTER );

    // Assign to member variables
    this->m_App = app;
    this->m_GLFWWindow = window;
    for (sint32 i = 0; i < 256; ++i) {
        this->m_GLFWKeyStates[i] = GLFW_RELEASE;
    }

    // Add to global stack
    sint32 cycle = 0;
    while (DvigEngine::WindowStack::m_GLFWWindows[cycle] != nullptr && ++cycle < DVIG_MAX_GUI_WINDOW_COUNT);

    DVIG_ASSERT( (cycle < DVIG_MAX_GUI_WINDOW_COUNT), DebugResult::EXCEEDED_MAX_COUNT_WINDOW );

    DvigEngine::WindowStack::m_GLFWWindows[cycle] = this->m_GLFWWindow;
    DvigEngine::WindowStack::m_WindowInstances[cycle] = this;

    this->m_WindowIndex = cycle;
    
    // Run start function
    this->Start();
}

void DvigEngine::IWindow::Wait()
{
    sint32 cycle = 0;
    usize presentWindowCount = 0;
    do
    {
        // Count non-zero windows
        presentWindowCount = 0;
        for (sint32 i = 0; i < DVIG_MAX_GUI_WINDOW_COUNT; ++i) {
            if (DvigEngine::WindowStack::m_GLFWWindows[i] != nullptr) { presentWindowCount += 1; }
        }

        // Wrap cycle value
        if (cycle >= DVIG_MAX_GUI_WINDOW_COUNT) { cycle = 0; }

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

            D3D11::GetSwapChain()->Present( 0, 0 );
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