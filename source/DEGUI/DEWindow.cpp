#include "../../include/DEGUI.hpp"

#include "../../thirdparty/glfw_win64/include/GLFW/glfw3.h"

void DvigEngine2::IWindow::Init()
{
    // Create GLFW window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    GLFWwindow* window = glfwCreateWindow(640, 480, "DvigEngine Test", NULL, NULL);

    DV_ASSERT_PTR(window);

    // Assign to member variables
    this->m_GLFWWindow = (void*)window;
}

void DvigEngine2::IWindow::Free()
{
    DvigEngine2::Engine* engine = this->GetEngine();

    if (this->m_UserData != nullptr) {
        engine->Delete( this->m_UserData );    
    }

    engine->Delete( this->GetMemoryObject() );
}

void DvigEngine2::IWindow::Start()
{
    DV_ASSERT_PTR(this->m_GLFWWindow)

    GLFWwindow* glfwWindow = (GLFWwindow*)this->m_GLFWWindow;
    while (!glfwWindowShouldClose(glfwWindow))
    {
        this->Update();

        glfwSwapBuffers(glfwWindow);
        glfwPollEvents();
    }
}