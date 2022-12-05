#include <iostream>
#include "../include/dvigengine/DEApplication.hpp"
#include "../include/dvigengine/DECore.hpp"
#include "../include/dvigengine/DED3D11.hpp"
#include "../include/dvigengine/DEGUI.hpp"
#include "../include/dvigengine/DERendering.hpp"

using DvigEngine::cpuword;
using DvigEngine::EngineInfo;
using DvigEngine::Engine;
using DvigEngine::MemoryObject;
using DvigEngine::Application;
using DvigEngine::IWindow;
using DvigEngine::RenderPassInfo;
using DvigEngine::RenderingSystem;
using DvigEngine::GeometryComponent;
using DvigEngine::TransformComponent;
using DvigEngine::ShaderComponent;
using DvigEngine::ViewerComponent;
using DvigEngine::INode;

int main()
{
    EngineInfo engineInfo;
    engineInfo.Version = DVIG_CURRENT_VERSION;
    engineInfo.SystemMemoryPoolByteWidth = 128 * DVIG_MEMORY_MiB;
    engineInfo.ComponentStorageMemoryPoolByteWidth = 4 * DVIG_MEMORY_MiB;

    Engine engine(&engineInfo);
    Engine* pEngine = Engine::GetClassInstance();

    class MyWindow : public IWindow
    {
        public:
            void Free() override final
            {

            }

            void Start() override final
            {
                Engine* engine = Engine::GetClassInstance();

                this->m_Geometry = engine->Create<GeometryComponent>("NewGeometryComponent");
                this->m_Geometry->Init( "C:\\Users\\USER100\\Documents\\GitHub\\DvigEngine2\\file\\meshes\\statue.obj" );
                this->m_Transform = engine->Create<TransformComponent>("NewTransformComponent");
                this->m_Transform->Init();
                this->m_Transform->SetPosition( 0.5f, 0.0f, 0.0f );
                this->m_Shader = engine->Create<ShaderComponent>("NewShaderComponent");
                this->m_Shader->Init( "C:\\Users\\USER100\\Documents\\GitHub\\DvigEngine2\\file\\shaders" );
            
                this->m_Node = engine->Create<INode>("NewNode");
                this->m_Node->Init();
                this->m_Node->InsertChildInstance( this->m_Geometry );
                this->m_Node->InsertChildInstance( this->m_Transform );
                this->m_Node->InsertChildInstance( this->m_Shader );

                this->m_ViewerTransform = engine->Create<TransformComponent>("NewViewerTransformComponent");
                this->m_ViewerTransform->Init();
                this->m_ViewerTransform->SetPosition( 0.5f, 0.0f, 0.0f );
                this->m_ViewerViewer = engine->Create<ViewerComponent>("NewViewerViewerComponent");
                this->m_ViewerViewer->Init();
                this->m_ViewerViewer->SetPosition( 0.0f, 0.0f, -4.0f );
                this->m_ViewerViewer->SetRotationEuler( 0.0f, 0.0f, 0.0f );
                this->m_ViewerViewer->SetPerspectiveProjection( 65.0f, 640.0f/480.0f, 0.01f, 100.0f );
            
                this->m_Node = engine->Create<INode>("NewNode");
                this->m_Node->Init();
                this->m_Node->InsertChildInstance( this->m_Geometry );
                this->m_Node->InsertChildInstance( this->m_Transform );
                this->m_Node->InsertChildInstance( this->m_Shader );

                this->m_NodeViewer = engine->Create<INode>("NewViewerNode");
                this->m_NodeViewer->Init();
                this->m_NodeViewer->InsertChildInstance( this->m_ViewerTransform );
                this->m_NodeViewer->InsertChildInstance( this->m_ViewerViewer );
            }
            
            void Update() override final
            {
                RenderPassInfo renderPass;
                renderPass.InputRenderTargets = nullptr;
                renderPass.OutputRenderTargets = nullptr;
                renderPass.Viewer = this->m_NodeViewer;
                RenderingSystem::BeginRenderPass( &renderPass );
                RenderingSystem::Viewport( 0.0f, 0.0f, 640.0f, 480.0f );
                RenderingSystem::BeginBatch();
                RenderingSystem::Draw( this->m_Node );
                RenderingSystem::EndBatch();
                RenderingSystem::EndRenderPass();

                this->CaptureKeyState('C');
                this->CaptureKeyState('W');
                this->CaptureKeyState('A');
                this->CaptureKeyState('S');
                this->CaptureKeyState('D');

                if (this->GetKeyState('C') == GLFW_PRESS) { exit(0); }
                if (this->GetKeyState('W') == GLFW_PRESS) { this->m_ViewerViewer->Move( 0.15f ); }
                if (this->GetKeyState('A') == GLFW_PRESS) { this->m_ViewerViewer->Strafe( -0.15f ); }
                if (this->GetKeyState('S') == GLFW_PRESS) { this->m_ViewerViewer->Move( -0.15f ); }
                if (this->GetKeyState('D') == GLFW_PRESS) { this->m_ViewerViewer->Strafe( 0.15f ); }

                this->m_ViewerViewer->DoMouseLook( this );
            }

        private:
            GeometryComponent* m_Geometry;
            TransformComponent* m_Transform;
            ShaderComponent* m_Shader;
            INode* m_Node;
            TransformComponent* m_ViewerTransform;
            ViewerComponent* m_ViewerViewer;
            INode* m_NodeViewer;
    };
    
    glm::uvec2 windowDimensions = glm::uvec2( 640u, 480u );
    Application* app = pEngine->Create<Application>("NewApp");
    app->Init();
    app->InsertWindow<MyWindow>( "NewWindow", "DvigEngine Test", windowDimensions );
    app->WaitForWindows();

    pEngine->Free();

    std::cout << "Success!" << std::endl;

    int val;
    std::cin >> val;

    return 0;
}