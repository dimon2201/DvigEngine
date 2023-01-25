#include <iostream>
#include "../include/DvigEngine/Application.hpp"
#include "../include/DvigEngine/Core.hpp"
#include "../include/DvigEngine/D3D11.hpp"
#include "../include/DvigEngine/GUI.hpp"
#include "../include/DvigEngine/Rendering.hpp"

using DvigEngine::cpuword;
using DvigEngine::EngineInfo;
using DvigEngine::Engine;
using DvigEngine::MemoryObject;
using DvigEngine::Application;
using DvigEngine::IWindow;
using DvigEngine::RenderPassInfo;
using DvigEngine::RenderBatchInfo;
using DvigEngine::RenderingSystem;
using DvigEngine::GeometryComponent;
using DvigEngine::TransformComponent;
using DvigEngine::ShaderComponent;
using DvigEngine::ViewerComponent;
using DvigEngine::Node;
using DvigEngine::Component;
using DvigEngine::ISystem;

int main()
{
    EngineInfo engineInfo;
    engineInfo.Version = DVIG_CURRENT_VERSION;
    engineInfo.SystemMemoryPoolByteWidth = 128 * DVIG_MEMORY_MiB;
    engineInfo.ComponentStorageMemoryPoolByteWidth = 4 * DVIG_MEMORY_MiB;

    Engine engine(&engineInfo);
    Engine* pEngine = Engine::GetClassInstance();

    class MySystem : public ISystem
    {
        public:
            DVIG_FUNCTION_INLINE void UpdateComponent(Component* component) override final
            {
                if (dynamic_cast<TransformComponent*>(component) != nullptr)
                {
                    TransformComponent* transform = (TransformComponent*)component;
                    transform->SetScale( 0.5f, 0.5f, 0.5f );
                }
            }
    };

    class MyWindow : public IWindow
    {
        public:
            void Free()
            {

            }

            void Start() override final
            {
                Engine* engine = Engine::GetClassInstance();

                this->m_Geometry = engine->Create<GeometryComponent>("NewGeometryComponent");
                this->m_Geometry->Init( "C:\\Users\\USER100\\Documents\\GitHub\\DvigEngine\\file\\meshes\\statue.obj" );

                this->m_Transform1 = engine->Create<TransformComponent>("NewTransformComponent1");
                this->m_Transform1->Init();
                this->m_Transform1->SetPosition( 0.0f, 0.0f, 0.0f );
                this->m_Transform1->SetScale( 1.0f, 1.0f, 1.0f );
                this->m_Transform2 = engine->Create<TransformComponent>("NewTransformComponent2");
                this->m_Transform2->Init();
                this->m_Transform2->SetPosition( 12.0f, 0.0f, 0.0f );
                this->m_Transform2->SetScale( 1.0f, 1.0f, 1.0f );
                this->m_Shader = engine->Create<ShaderComponent>("NewShaderComponent");
                this->m_Shader->Init();
                this->m_Shader->SetVertexShader( "C:\\Users\\USER100\\Documents\\GitHub\\DvigEngine\\file\\shaders\\renderpass_vertex.ext" );
                this->m_Shader->SetPixelShader( "C:\\Users\\USER100\\Documents\\GitHub\\DvigEngine\\file\\shaders\\renderpass_pixel.ext" );

                this->m_ViewerTransform = engine->Create<TransformComponent>("NewViewerTransformComponent");
                this->m_ViewerTransform->Init();
                this->m_ViewerTransform->SetPosition( 0.5f, 0.0f, 0.0f );
                this->m_ViewerViewer = engine->Create<ViewerComponent>("NewViewerViewerComponent");
                this->m_ViewerViewer->Init();
                this->m_ViewerViewer->SetPosition( 0.0f, 0.0f, -4.0f );
                this->m_ViewerViewer->SetRotationEuler( 0.0f, 0.0f, 0.0f );
                this->m_ViewerViewer->SetPerspectiveProjection( 65.0f, 640.0f/480.0f, 0.01f, 100.0f );
            
                this->m_Node1 = engine->Create<Node>("NewNode1");
                this->m_Node1->Init();
                this->m_Node1->AddChild( this->m_Geometry );
                this->m_Node1->AddChild( this->m_Transform1 );
                this->m_Node1->AddChild( this->m_Shader );
                this->m_Node2 = engine->Create<Node>("NewNode2");
                this->m_Node2->Init();
                this->m_Node2->AddChild( this->m_Geometry );
                this->m_Node2->AddChild( this->m_Transform2 );
                this->m_Node2->AddChild( this->m_Shader );

                this->m_NodeViewer = engine->Create<Node>("NewViewerNode");
                this->m_NodeViewer->Init();
                this->m_NodeViewer->AddChild( this->m_ViewerTransform );
                this->m_NodeViewer->AddChild( this->m_ViewerViewer );

                MySystem sys;
                sys.Update();
            }
            
            void Update() override final
            {
                Engine* engine = Engine::GetClassInstance();

                RenderPassInfo renderPass;
                renderPass.InputRenderTargets = nullptr;
                renderPass.OutputRenderTargets = nullptr;
                renderPass.Viewer = this->m_NodeViewer;

                RenderBatchInfo batch;
                batch.Geometry = engine->FindExistingInstance<GeometryComponent>( "NewGeometryComponent" );
                batch.Shader = engine->FindExistingInstance<ShaderComponent>( "NewShaderComponent" );

                RenderingSystem::BeginRenderPass( &renderPass );
                RenderingSystem::Viewport( 0.0f, 0.0f, 640.0f, 480.0f );
                RenderingSystem::BeginBatch( &batch );
                RenderingSystem::Draw( this->m_Node1 );
                RenderingSystem::Draw( this->m_Node2 );
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

                // this->m_ViewerViewer->DoMouseLook( this );
            }

        private:
            GeometryComponent* m_Geometry;
            TransformComponent* m_Transform1;
            TransformComponent* m_Transform2;
            ShaderComponent* m_Shader;
            Node* m_Node1;
            Node* m_Node2;
            TransformComponent* m_ViewerTransform;
            ViewerComponent* m_ViewerViewer;
            Node* m_NodeViewer;
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