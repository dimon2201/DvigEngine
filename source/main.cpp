#include "../include/DEApplication.hpp"
#include "../include/DECore.hpp"
#include "../include/DEGUI.hpp"
#include "../include/DERendering.hpp"
#include "../include/DEThirdPartyMath.hpp"
#include "../include/DEThirdPartyWindow.hpp"

void Func(DvigEngine2::demachword* arguments, DvigEngine2::deint32 jobIndex)
{
    std::cout << "Hello from " << jobIndex << " thread!" << std::endl;
}

int main()
{
    DvigEngine2::MemoryPoolProperty memoryPoolsData[2];
    memoryPoolsData[0].m_ByteWidth = 128 * DV_MEMORY_MiB;
    memoryPoolsData[1].m_ByteWidth = 24 * DV_MEMORY_KiB;

    DvigEngine2::EngineInputProperty engineInputData;
    engineInputData.m_Version = DV_ENGINE_VERSION_NUMBER;
    engineInputData.m_MemoryPoolsCount = 2u;
    engineInputData.m_MemoryPoolsData = memoryPoolsData;
    engineInputData.m_SystemMemoryPoolIndex = 0;
    engineInputData.m_ComponentStorageMemoryPoolIndex = 1;
    engineInputData.m_RequestedThreadCount = 2;

    DvigEngine2::Engine engine(&engineInputData);
    DvigEngine2::Engine* pEngine = &engine;

    DvigEngine2::ThreadPoolSystem::Init();

    // Create window
    class AppWindow : public DvigEngine2::IWindow {
        public:
            void Start() override final {
                DvigEngine2::Engine::GetClassInstance()->RegisterComponent<DvigEngine2::GeometryComponent>();
                DvigEngine2::Engine::GetClassInstance()->RegisterComponent<DvigEngine2::TransformComponent>();
                DvigEngine2::Engine::GetClassInstance()->RegisterComponent<DvigEngine2::ShaderComponent>();
                DvigEngine2::Engine::GetClassInstance()->RegisterComponent<DvigEngine2::ViewerComponent>();

                DvigEngine2::defloat32 vertices[9] = {
                    -1.0f, -1.0f, -1.0f,
                    0.0f, 1.0f, -1.0f,
                    1.0f, -1.0f, -1.0f
                };
                DvigEngine2::deuint32 indices[3] = {
                    0, 1, 2
                };

                DvigEngine2::Engine* engine = DvigEngine2::Engine::GetClassInstance();

                DvigEngine2::GeometryComponent* geomComp0;
                DvigEngine2::TransformComponent* transComp0;
                DvigEngine2::ShaderComponent* shaderComp0;
                DvigEngine2::GeometryComponent* geomComp1;
                DvigEngine2::TransformComponent* transComp1;
                DvigEngine2::ShaderComponent* shaderComp1;
                DvigEngine2::TransformComponent* viewerTransComp;
                DvigEngine2::ViewerComponent* viewerViewerComp;
                DvigEngine2::INode* node0;
                DvigEngine2::INode* node1;
                DvigEngine2::INode* viewer;

                engine->Create <DvigEngine2::GeometryComponent> ( &geomComp0, "MyGeometryComponent_0" );
                engine->Create <DvigEngine2::TransformComponent> ( &transComp0, "MyTransformComponent_0" );
                engine->Create <DvigEngine2::ShaderComponent> ( &shaderComp0, "MyShaderComponent_0" );
                engine->Create <DvigEngine2::GeometryComponent> ( &geomComp1, "MyGeometryComponent_1" );
                engine->Create <DvigEngine2::TransformComponent> ( &transComp1, "MyTransformComponent_1" );
                engine->Create <DvigEngine2::ShaderComponent> ( &shaderComp1, "MyShaderComponent_1" );
                engine->Create <DvigEngine2::TransformComponent> ( &viewerTransComp, "ViewerTransformComponent_1" );
                engine->Create <DvigEngine2::ViewerComponent> ( &viewerViewerComp, "ViewerComponent_0" );
                engine->Create <DvigEngine2::INode> ( &node0, "MyNode_0" );
                engine->Create <DvigEngine2::INode> ( &node1, "MyNode_1" );
                engine->Create <DvigEngine2::INode> ( &viewer, "Viewer_0" );

                geomComp0->Init( "C:\\Users\\USER100\\Documents\\GitHub\\DvigEngine\\files\\moai.obj" );
                transComp0->Init();
                shaderComp0->Init( "C:\\Users\\USER100\\Documents\\GitHub\\DvigEngine\\files\\shader.vert",
                                  "C:\\Users\\USER100\\Documents\\GitHub\\DvigEngine\\files\\shader.frag" );
                geomComp1->Init( "C:\\Users\\USER100\\Documents\\GitHub\\DvigEngine\\files\\moai.obj" );
                transComp1->Init();
                shaderComp1->Init( "C:\\Users\\USER100\\Documents\\GitHub\\DvigEngine\\files\\shader.vert",
                                  "C:\\Users\\USER100\\Documents\\GitHub\\DvigEngine\\files\\shader.frag" );
                viewerTransComp->Init();
                viewerViewerComp->Init();
                node0->Init();
                node1->Init();
                viewer->Init();
                
                engine->AddComponent <DvigEngine2::GeometryComponent> ( &node0, geomComp0 );
                engine->AddComponent <DvigEngine2::TransformComponent> ( &node0, transComp0 );
                engine->AddComponent <DvigEngine2::ShaderComponent> ( &node0, shaderComp0 );
                engine->AddComponent <DvigEngine2::GeometryComponent> ( &node1, geomComp0 );
                engine->AddComponent <DvigEngine2::TransformComponent> ( &node1, transComp1 );
                engine->AddComponent <DvigEngine2::ShaderComponent> ( &node1, shaderComp1 );
                engine->AddComponent <DvigEngine2::TransformComponent> ( &viewer, viewerTransComp );
                engine->AddComponent <DvigEngine2::ViewerComponent> ( &viewer, viewerViewerComp );

                DvigEngine2::GL4::Enable( GL_DEPTH_TEST );
            }
            void Update() override final {
                DvigEngine2::Engine* engine = DvigEngine2::Engine::GetClassInstance();
                DvigEngine2::Application* app = this->GetApplication();
                DvigEngine2::INode* myNode_0 = (DvigEngine2::INode*)engine->GetExistingInstance( "MyNode_0" );
                myNode_0->GetComponent<DvigEngine2::TransformComponent>(nullptr)->SetPosition( 0.0f, 0.0f, 0.0f );
                myNode_0->GetComponent<DvigEngine2::TransformComponent>(nullptr)->SetRotationEuler( 0.0f, 0.0f, 0.0f );
                myNode_0->GetComponent<DvigEngine2::TransformComponent>(nullptr)->SetScale( 1.0f, 1.0f, 1.0f );
                DvigEngine2::INode* myNode_1 = (DvigEngine2::INode*)engine->GetExistingInstance( "MyNode_1" );
                DvigEngine2::ShaderComponent* myNode_0_Shader = (DvigEngine2::ShaderComponent*)myNode_0->GetComponent<DvigEngine2::ShaderComponent>(nullptr);

                DvigEngine2::IWindow* myWindow = app->GetWindow( "MyTestWindow_0" );
                int windowWidth = 0;
                int windowHeight = 0;
                glfwGetFramebufferSize( (GLFWwindow*)myWindow->GetGLFWWindow(), &windowWidth, &windowHeight );

                DvigEngine2::GL4::Viewport( 0, 0, windowWidth, windowHeight );
                DvigEngine2::GL4::ClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
                DvigEngine2::GL4::Clear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

                DvigEngine2::INode* viewer = (DvigEngine2::INode*)engine->GetExistingInstance( "Viewer_0" );
                static demfloat fRot = 0.0f;
                if (fRot > 360.0f) { fRot = 0.0f; }
                fRot += 0.01f;
                viewer->GetComponent<DvigEngine2::ViewerComponent>(nullptr)->SetRotation( glm::vec3(0.0f), fRot, 0.0f, 0.0f );

                DvigEngine2::RenderingSystem::BeginRender(viewer);
                DvigEngine2::RenderingSystem::BeginBatch();
                DvigEngine2::RenderingSystem::Draw( myNode_0 );
                // DvigEngine2::RenderingSystem::Draw( myNode_1 );
                DvigEngine2::RenderingSystem::EndBatch();
                DvigEngine2::RenderingSystem::EndRender();
            }
    };

    const char* windowCaption = "DvigEngine Test";
    glm::uvec2 windowSize(640, 480);

    DvigEngine2::Application* appSys;
    pEngine->Create <DvigEngine2::Application> ( &appSys, "MyApplication_0" );
    appSys->Init();
    appSys->AddWindow <AppWindow> ( "MyTestWindow_0", &windowCaption[0], windowSize );
    appSys->WaitForWindows();

    // DvigEngine2::MemoryObject* mo = DvigEngine2::Engine::Allocate( 0, 256 );
    // std::cout << mo << std::endl;
    // pEngine->Delete( mo );
    // mo = DvigEngine2::Engine::Allocate( 0, 256 );
    // std::cout << mo->Unwrap<float*>()[0] << std::endl;

    DvigEngine2::ThreadPoolSystem::Terminate();
    DvigEngine2::ThreadPoolSystem::WaitForJobs();
    
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