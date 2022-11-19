#include "../include/DEApplication.hpp"
#include "../include/DECore.hpp"
#include "../include/DEGUI.hpp"
#include "../include/DERendering.hpp"
#include "../include/DEThirdPartyMath.hpp"
#include "../include/DEThirdPartyWindow.hpp"

void Func(DvigEngine::demachword* arguments, DvigEngine::deint32 jobIndex)
{
    std::cout << "Hello from " << jobIndex << " thread!" << std::endl;
}

int main()
{
    DvigEngine::MemoryPoolProperty memoryPoolsData[2];
    memoryPoolsData[0].m_ByteWidth = 128 * DV_MEMORY_MiB;
    memoryPoolsData[1].m_ByteWidth = 24 * DV_MEMORY_KiB;

    DvigEngine::EngineInputProperty engineInputData;
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
                DvigEngine::Engine::GetClassInstance()->RegisterComponent<DvigEngine::GeometryComponent>();
                DvigEngine::Engine::GetClassInstance()->RegisterComponent<DvigEngine::TransformComponent>();
                DvigEngine::Engine::GetClassInstance()->RegisterComponent<DvigEngine::ShaderComponent>();
                DvigEngine::Engine::GetClassInstance()->RegisterComponent<DvigEngine::ViewerComponent>();

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
                DvigEngine::TransformComponent* viewerTransComp;
                DvigEngine::ViewerComponent* viewerViewerComp;
                DvigEngine::INode* node0;
                DvigEngine::INode* node1;
                DvigEngine::INode* viewer;

                engine->Create <DvigEngine::GeometryComponent> ( &geomComp0, "MyGeometryComponent_0" );
                engine->Create <DvigEngine::TransformComponent> ( &transComp0, "MyTransformComponent_0" );
                engine->Create <DvigEngine::ShaderComponent> ( &shaderComp0, "MyShaderComponent_0" );
                engine->Create <DvigEngine::GeometryComponent> ( &geomComp1, "MyGeometryComponent_1" );
                engine->Create <DvigEngine::TransformComponent> ( &transComp1, "MyTransformComponent_1" );
                engine->Create <DvigEngine::ShaderComponent> ( &shaderComp1, "MyShaderComponent_1" );
                engine->Create <DvigEngine::TransformComponent> ( &viewerTransComp, "ViewerTransformComponent_1" );
                engine->Create <DvigEngine::ViewerComponent> ( &viewerViewerComp, "ViewerComponent_0" );
                engine->Create <DvigEngine::INode> ( &node0, "MyNode_0" );
                engine->Create <DvigEngine::INode> ( &node1, "MyNode_1" );
                engine->Create <DvigEngine::INode> ( &viewer, "Viewer_0" );

                geomComp0->Init( "C:\\Users\\USER100\\Documents\\GitHub\\DvigEngine\\files\\statue.obj" );
                transComp0->Init();
                shaderComp0->Init( "C:\\Users\\USER100\\Documents\\GitHub\\DvigEngine\\files\\shader.vert",
                                  "C:\\Users\\USER100\\Documents\\GitHub\\DvigEngine\\files\\shader.frag" );
                geomComp1->Init( "C:\\Users\\USER100\\Documents\\GitHub\\DvigEngine\\files\\moai.obj" );
                transComp1->Init();
                shaderComp1->Init( "C:\\Users\\USER100\\Documents\\GitHub\\DvigEngine\\files\\shader.vert",
                                  "C:\\Users\\USER100\\Documents\\GitHub\\DvigEngine\\files\\shader.frag" );
                viewerTransComp->Init();
                viewerViewerComp->Init();
                viewerViewerComp->SetPosition( 0.0f, 2.0f, 9.0f );
                node0->Init();
                node1->Init();
                viewer->Init();
                
                engine->AddComponent <DvigEngine::GeometryComponent> ( &node0, geomComp0 );
                engine->AddComponent <DvigEngine::TransformComponent> ( &node0, transComp0 );
                engine->AddComponent <DvigEngine::ShaderComponent> ( &node0, shaderComp0 );
                engine->AddComponent <DvigEngine::GeometryComponent> ( &node1, geomComp0 );
                engine->AddComponent <DvigEngine::TransformComponent> ( &node1, transComp1 );
                engine->AddComponent <DvigEngine::ShaderComponent> ( &node1, shaderComp1 );
                engine->AddComponent <DvigEngine::TransformComponent> ( &viewer, viewerTransComp );
                engine->AddComponent <DvigEngine::ViewerComponent> ( &viewer, viewerViewerComp );

                DvigEngine::GL4::Enable( GL_DEPTH_TEST );
            }
            void Update() override final {
                static demfloat fRot = 0.0f;
                if (fRot > 360.0f) { fRot = 0.0f; }
                fRot += 0.1f;
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

                DvigEngine::GL4::Viewport( 0, 0, windowWidth, windowHeight );
                DvigEngine::GL4::ClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
                DvigEngine::GL4::Clear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

                DvigEngine::INode* viewer = (DvigEngine::INode*)engine->GetExistingInstance( "Viewer_0" );
                viewer->GetComponent<DvigEngine::ViewerComponent>(nullptr)->SetRotationEuler( 0.0f, 0.0f, 0.0f );
                viewer->GetComponent<DvigEngine::ViewerComponent>(nullptr)->SetPerspectiveProjection( 65.0f, 640.0f/480.0f, 0.1f, 100.0f );

                DvigEngine::RenderingSystem::BeginRender(viewer);
                DvigEngine::RenderingSystem::BeginBatch();
                DvigEngine::RenderingSystem::Draw( myNode_0 );
                // DvigEngine::RenderingSystem::Draw( myNode_1 );
                DvigEngine::RenderingSystem::EndBatch();
                DvigEngine::RenderingSystem::EndRender();
            }
    };

    const char* windowCaption = "DvigEngine Test";
    glm::uvec2 windowSize(640, 480);

    DvigEngine::Application* appSys;
    pEngine->Create <DvigEngine::Application> ( &appSys, "MyApplication_0" );
    appSys->Init();
    appSys->AddWindow <AppWindow> ( "MyTestWindow_0", &windowCaption[0], windowSize );
    appSys->WaitForWindows();

    // DvigEngine::MemoryObject* mo = DvigEngine::Engine::Allocate( 0, 256 );
    // std::cout << mo << std::endl;
    // pEngine->Delete( mo );
    // mo = DvigEngine::Engine::Allocate( 0, 256 );
    // std::cout << mo->Unwrap<float*>()[0] << std::endl;

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