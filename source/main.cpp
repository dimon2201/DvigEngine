#include "../include/DEApplication.hpp"
#include "../include/DECore.hpp"
#include "../include/DEGUI.hpp"
#include "../include/DEOpenGL4.hpp"
#include "../include/DERendering.hpp"
#include "../include/DEThirdPartyMath.hpp"
#include "../include/DEThirdPartyWindow.hpp"

int main()
{
    DvigEngine::MemoryPoolInfo memoryPoolsData[2];
    memoryPoolsData[0].m_ByteWidth = 128 * DV_MEMORY_MiB;
    memoryPoolsData[1].m_ByteWidth = 24 * DV_MEMORY_KiB;

    DvigEngine::EngineInputInfo engineInputData;
    engineInputData.m_Version = DV_ENGINE_VERSION_NUMBER;
    engineInputData.m_MemoryPoolsCount = 2u;
    engineInputData.m_MemoryPoolsData = memoryPoolsData;
    engineInputData.m_SystemMemoryPoolIndex = 0;
    engineInputData.m_ComponentStorageMemoryPoolIndex = 1;
    engineInputData.m_RequestedThreadCount = 2;

    DvigEngine::Engine engine(&engineInputData);
    DvigEngine::Engine* pEngine = &engine;

    DvigEngine::ThreadPoolSystem::Init();

    // Create window
    class AppWindow : public DvigEngine::IWindow {
        public:
            void Start() override final {
                
                DvigEngine::deuchar texels[32] = 
                {
                    0,   0,   0,   255,
                    255, 0,   0,   255,
                    0,   255, 0,   255,
                    0,   0,   255, 255,
                };
                auto idx1 = DvigEngine::TextureMergerSystem::AddTexture( 2, 2, (const void*)&texels[0] );
                auto idx2 = DvigEngine::TextureMergerSystem::AddTexture( 2, 2, (const void*)&texels[0] );
                auto texture2 = DvigEngine::TextureMergerSystem::GetAtlasTexture( idx2 );
                std::cout << texture2->m_X << " " << texture2->m_Y << std::endl;


                this->prevMouseX = -1.0;
                this->prevMouseY = -1.0;

                DvigEngine::Engine::GetClassInstance()->RegisterComponent<DvigEngine::GeometryComponent>();
                DvigEngine::Engine::GetClassInstance()->RegisterComponent<DvigEngine::TransformComponent>();
                DvigEngine::Engine::GetClassInstance()->RegisterComponent<DvigEngine::ShaderComponent>();
                DvigEngine::Engine::GetClassInstance()->RegisterComponent<DvigEngine::ViewerComponent>();
                DvigEngine::Engine::GetClassInstance()->RegisterComponent<DvigEngine::PostProcessorComponent>();

                DvigEngine::defloat32 vertices[9] = {
                    -1.0f, -1.0f, -1.0f,
                    0.0f, 1.0f, -1.0f,
                    1.0f, -1.0f, -1.0f
                };
                DvigEngine::deuint32 indices[3] = {
                    0, 1, 2
                };

                DvigEngine::Engine* engine = DvigEngine::Engine::GetClassInstance();

                DvigEngine::GeometryComponent* geomComp0;
                DvigEngine::TransformComponent* transComp0;
                DvigEngine::ShaderComponent* shaderComp0;
                DvigEngine::GeometryComponent* geomComp1;
                DvigEngine::TransformComponent* transComp1;
                DvigEngine::ShaderComponent* shaderComp1;
                DvigEngine::GeometryComponent* geomComp2;
                DvigEngine::TransformComponent* viewerTransComp;
                DvigEngine::ViewerComponent* viewerViewerComp;
                DvigEngine::PostProcessorComponent* postProcessComp;
                DvigEngine::ShaderComponent* postProcessShaderComp;
                DvigEngine::INode* node0;
                DvigEngine::INode* node1;
                DvigEngine::INode* node2;
                DvigEngine::INode* viewer;
                DvigEngine::INode* nodePostProcess;

                engine->Create <DvigEngine::GeometryComponent> ( &geomComp0, "MyGeometryComponent_0" );
                engine->Create <DvigEngine::TransformComponent> ( &transComp0, "MyTransformComponent_0" );
                engine->Create <DvigEngine::ShaderComponent> ( &shaderComp0, "MyShaderComponent_0" );
                engine->Create <DvigEngine::GeometryComponent> ( &geomComp1, "MyGeometryComponent_1" );
                engine->Create <DvigEngine::TransformComponent> ( &transComp1, "MyTransformComponent_1" );
                engine->Create <DvigEngine::ShaderComponent> ( &shaderComp1, "MyShaderComponent_1" );
                engine->Create <DvigEngine::TransformComponent> ( &viewerTransComp, "ViewerTransformComponent_1" );
                engine->Create <DvigEngine::GeometryComponent> ( &geomComp2, "MyGeometryComponent_2" );
                engine->Create <DvigEngine::ViewerComponent> ( &viewerViewerComp, "ViewerComponent_0" );
                engine->Create <DvigEngine::ShaderComponent> ( &postProcessShaderComp, "MyPostProcessShaderComponent_0" );
                engine->Create <DvigEngine::PostProcessorComponent> ( &postProcessComp, "MyPostProcessComponent_0" );
                engine->Create <DvigEngine::INode> ( &node0, "MyNode_0" );
                engine->Create <DvigEngine::INode> ( &node1, "MyNode_1" );
                engine->Create <DvigEngine::INode> ( &node2, "MyNode_2" );
                engine->Create <DvigEngine::INode> ( &viewer, "Viewer_0" );
                engine->Create <DvigEngine::INode> ( &nodePostProcess, "PostProcess_0" );

                geomComp0->Init( "C:\\Users\\USER100\\Documents\\GitHub\\DvigEngine\\files\\statue.obj" );
                transComp0->Init();
                shaderComp0->Init( "C:\\Users\\USER100\\Documents\\GitHub\\DvigEngine\\files\\shaders\\renderpass.vert",
                                  "C:\\Users\\USER100\\Documents\\GitHub\\DvigEngine\\files\\shaders\\renderpass.frag" );
                geomComp1->Init( "C:\\Users\\USER100\\Documents\\GitHub\\DvigEngine\\files\\statue.obj" );
                transComp1->Init();
                shaderComp1->Init( "C:\\Users\\USER100\\Documents\\GitHub\\DvigEngine\\files\\shaders\\renderpass.vert",
                                  "C:\\Users\\USER100\\Documents\\GitHub\\DvigEngine\\files\\shaders\\renderpass.frag" );
                geomComp2->Init( "C:\\Users\\USER100\\Documents\\GitHub\\DvigEngine\\files\\statue.obj" );
                viewerTransComp->Init();
                viewerViewerComp->Init();
                postProcessShaderComp->Init( "C:\\Users\\USER100\\Documents\\GitHub\\DvigEngine\\files\\shaders\\postprocess.vert",
                                  "C:\\Users\\USER100\\Documents\\GitHub\\DvigEngine\\files\\shaders\\postprocess.frag" );
                postProcessComp->Init();
                transComp1->SetPosition( 7.0f, 0.0f, 0.0f );
                viewerViewerComp->SetPosition( 0.0f, 2.0f, 4.0f );
                node0->Init();
                node1->Init();
                viewer->Init();
                
                node0->AddComponent <DvigEngine::GeometryComponent> ( geomComp0 );
                node0->AddComponent <DvigEngine::TransformComponent> ( transComp0 );
                node0->AddComponent <DvigEngine::ShaderComponent> ( shaderComp0 );
                node1->AddComponent <DvigEngine::GeometryComponent> ( geomComp1 );
                node1->AddComponent <DvigEngine::TransformComponent> ( transComp1 );
                node1->AddComponent <DvigEngine::ShaderComponent> ( shaderComp0 );
                node2->AddComponent <DvigEngine::GeometryComponent> ( geomComp2 );
                viewer->AddComponent <DvigEngine::TransformComponent> ( viewerTransComp );
                viewer->AddComponent <DvigEngine::ViewerComponent> ( viewerViewerComp );
                nodePostProcess->AddComponent <DvigEngine::ShaderComponent> ( postProcessShaderComp );
                nodePostProcess->AddComponent <DvigEngine::PostProcessorComponent> ( postProcessComp );

                DvigEngine::GL4::Enable( GL_DEPTH_TEST );
            }
            void Update() override final {
                static demfloat fRot = 0.0f;
                if (fRot > 360.0f) { fRot = 0.0f; }
                // fRot += 0.1f;
                DvigEngine::Engine* engine = DvigEngine::Engine::GetClassInstance();
                DvigEngine::Application* app = this->GetApplication();
                DvigEngine::INode* myNode_0 = (DvigEngine::INode*)engine->GetExistingInstance( "MyNode_0" );
                myNode_0->GetComponent<DvigEngine::TransformComponent>(nullptr)->SetPosition( 0.0f, 0.0f, 0.0f );
                myNode_0->GetComponent<DvigEngine::TransformComponent>(nullptr)->SetRotationEuler( 0.0f, fRot, 0.0f );
                myNode_0->GetComponent<DvigEngine::TransformComponent>(nullptr)->SetScale( 1.0f, 1.0f, 1.0f );
                DvigEngine::INode* myNode_1 = (DvigEngine::INode*)engine->GetExistingInstance( "MyNode_1" );
                DvigEngine::ShaderComponent* myNode_0_Shader = (DvigEngine::ShaderComponent*)myNode_0->GetComponent<DvigEngine::ShaderComponent>(nullptr);

                DvigEngine::IWindow* myWindow = app->GetWindow( "MyTestWindow_0" );
                int windowWidth = 0;
                int windowHeight = 0;
                glfwGetFramebufferSize( (GLFWwindow*)myWindow->GetGLFWWindow(), &windowWidth, &windowHeight );

                DvigEngine::INode* viewer = (DvigEngine::INode*)engine->GetExistingInstance( "Viewer_0" );
                viewer->GetComponent<DvigEngine::ViewerComponent>(nullptr)->SetPerspectiveProjection( 65.0f, 640.0f/480.0f, 0.1f, 100.0f );

                DvigEngine::RenderPassInfo renderPass;
                renderPass.Type = DvigEngine::RenderPassType::FRAMEBUFFER;
                renderPass.Viewer = viewer;
                renderPass.InputRenderTargets = nullptr;
                renderPass.OutputRenderTargets = myWindow->GetRenderTargetGroup();
                DvigEngine::RenderingSystem::BeginRenderPass(&renderPass);
                DvigEngine::RenderingSystem::Viewport( 0, 0, windowWidth, windowHeight );
                DvigEngine::RenderingSystem::PaintBackground( 0.0f, 0.0f, 0.0f, 1.0f );
                DvigEngine::RenderingSystem::BeginBatch();
                DvigEngine::RenderingSystem::Draw( myNode_0 );
                // DvigEngine::RenderingSystem::Draw( myNode_1 );
                DvigEngine::RenderingSystem::EndBatch();
                DvigEngine::RenderingSystem::EndRenderPass();

                DvigEngine::INode* nodePostProcess = (DvigEngine::INode*)engine->GetExistingInstance( "PostProcess_0" );
                DvigEngine::RenderPassInfo postProcessPass;
                postProcessPass.Type = DvigEngine::RenderPassType::SCREEN_FINAL;
                postProcessPass.Viewer = viewer;
                postProcessPass.PostProcessor = nodePostProcess;
                postProcessPass.InputRenderTargets = myWindow->GetRenderTargetGroup();
                postProcessPass.OutputRenderTargets = nullptr;
                DvigEngine::RenderingSystem::BeginRenderPass(&postProcessPass);
                DvigEngine::RenderingSystem::Viewport( 0, 0, windowWidth, windowHeight );
                DvigEngine::RenderingSystem::PaintBackground( 0.0f, 0.0f, 0.0f, 1.0f );
                DvigEngine::RenderingSystem::EndRenderPass();

                myWindow->CaptureKeyState('W');
                myWindow->CaptureKeyState('S');
                myWindow->CaptureKeyState('A');
                myWindow->CaptureKeyState('D');
                myWindow->CaptureKeyState('C');

                if (myWindow->GetKeyState('W') == GLFW_PRESS) {
                    viewer->GetComponent<DvigEngine::ViewerComponent>(nullptr)->Move( 0.1f/60.0f );
                }
                if (myWindow->GetKeyState('S') == GLFW_PRESS) {
                    viewer->GetComponent<DvigEngine::ViewerComponent>(nullptr)->Move( -0.1f/60.0f );
                }
                if (myWindow->GetKeyState('A') == GLFW_PRESS) {
                    viewer->GetComponent<DvigEngine::ViewerComponent>(nullptr)->Strafe( 0.1f/60.0f );
                }
                if (myWindow->GetKeyState('D') == GLFW_PRESS) {
                    viewer->GetComponent<DvigEngine::ViewerComponent>(nullptr)->Strafe( -(0.1f/60.0f) );
                }
                if (myWindow->GetKeyState('C') == GLFW_PRESS) {
                    exit(0);
                }

                this->MouseLook();
            }
            void MouseLook() {
                double mouseX = this->GetMouseX();
                double mouseY = this->GetMouseY();
                if (mouseX >= this->GetWindowWidth()) {
                    mouseX = 0;
                    this->prevMouseX = 0;
                    this->SetMousePositionX(0);
                } else if (mouseX <= 0) {
                    mouseX = this->GetWindowWidth();
                    this->prevMouseX = this->GetWindowWidth();
                    this->SetMousePositionX(this->GetWindowWidth());
                }
                if (mouseY >= this->GetWindowHeight()) {
                    mouseY = 0;
                    this->prevMouseY = 0;
                    this->SetMousePositionY(0);
                } else if (mouseY <= 0) {
                    mouseY = this->GetWindowHeight();
                    this->prevMouseY = this->GetWindowHeight();
                    this->SetMousePositionY(this->GetWindowHeight());
                }
                if (this->prevMouseX != -1.0 && this->prevMouseY != -1.0)
                {
                    double deltaX = mouseX - this->prevMouseX;
                    double deltaY = mouseY - this->prevMouseY;
                    DvigEngine::Engine* engine = DvigEngine::Engine::GetClassInstance();
                    DvigEngine::INode* viewer = (DvigEngine::INode*)engine->GetExistingInstance( "Viewer_0" );
                    DvigEngine::ViewerComponent* viewerViewer = viewer->GetComponent<DvigEngine::ViewerComponent>(nullptr);
                    
                    demfloat rotX = viewerViewer->GetRotationX();
                    demfloat rotY = viewerViewer->GetRotationY();
                    rotX += -deltaY / 2.5f;
                    rotY += -deltaX / 2.5f;
                    if (rotX > 75.0f) {
                        rotX = 75.0f;
                    } else if (rotX < -75.0f) {
                        rotX = -75.0f;
                    }
                    if (rotY < -360) {
                        rotY = 360 + rotY;
                    } else if (rotY > 360) {
                        rotY = 360 - rotY;
                    }
                    viewerViewer->SetRotationX(rotX);
                    viewerViewer->SetRotationY(rotY);
                }

                this->prevMouseX = mouseX;
                this->prevMouseY = mouseY;
            }
        
        private:
            double prevMouseX;
            double prevMouseY;
    };

    const char* windowCaption = "DvigEngine Test";
    glm::uvec2 windowSize(640, 480);

    DvigEngine::Application* appSys;
    pEngine->Create <DvigEngine::Application> ( &appSys, "MyApplication_0" );
    appSys->Init();
    appSys->AddWindow <AppWindow> ( "MyTestWindow_0", &windowCaption[0], windowSize );
    appSys->WaitForWindows();
    
    DvigEngine::ThreadPoolSystem::Terminate();
    DvigEngine::ThreadPoolSystem::WaitForJobs();
    
    /*
        Node "House"
          GeometryComponent
        Node "House"
          GeometryComponent

        Draw()
          Bind vertex buffer
          Bind uniform buffer

        renderSys = RenderingSystem::Instance
        renderSys -> BeginBatch()
          renderSys -> Draw()
        renderSys -> EndBatch()
    */

    /*
        DvigEngine Info
        - Everything is based on objects
        - Object is something that has INTERNAL data and FUNCTIONALITY that operates on INTERNAL data
        - Component is a piece of data
        - System is an object that does not have external data but rather FUNCTIONALITY on SOMEWHAT data
        - HelperObject is an object that combines both INTERNAL data and FUNCTIONALITY on INTERNAL data

        Node
         Node
          GeometryComponent
          ScriptComponent
         Node
          GeometryComponent
    */

    std::cout << "Success!" << std::endl;

    return 0;
}