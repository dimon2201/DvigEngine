#include "../include/DEApplication.hpp"
#include "../include/DECore.hpp"
#include "../include/DEGUI.hpp"
#include "../include/DERendering.hpp"
#include "../include/DEThirdPartyMath.hpp"

void Func(DvigEngine2::demachword* arguments, DvigEngine2::deint32 jobIndex)
{
    std::cout << "Hello from " << jobIndex << " thread!" << std::endl;
}

int main()
{
    DvigEngine2::MemoryPoolProperty memoryPoolsData[2];
    memoryPoolsData[0].m_ByteWidth = 4 * DV_MEMORY_MiB;
    memoryPoolsData[1].m_ByteWidth = 24 * DV_MEMORY_KiB;

    DvigEngine2::EngineInputProperty engineInputData;
    engineInputData.m_Version = DV_ENGINE_VERSION_NUMBER;
    engineInputData.m_MemoryPoolsCount = 2u;
    engineInputData.m_MemoryPoolsData = memoryPoolsData;
    engineInputData.m_SystemMemoryPoolIndex = 0;
    engineInputData.m_ComponentStorageMemoryPoolIndex = 1;
    engineInputData.m_RequestedThreadCount = 2;

    // DvigEngine2::Engine::Init(&engineInputData);
    // DvigEngine2::Engine* engine = DvigEngine2::Engine::GetClassInstance();
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

                DvigEngine2::defloat32 vertices[9] = {
                    -1.0f, -1.0f, -1.0f,
                    0.0f, 1.0f, -1.0f,
                    1.0f, -1.0f, -1.0f
                };
                DvigEngine2::deuint32 indices[3] = {
                    0, 1, 2
                };

                DvigEngine2::Engine* engine = DvigEngine2::Engine::GetClassInstance();

                DvigEngine2::GeometryComponent* geomComp;
                DvigEngine2::TransformComponent* transComp;
                DvigEngine2::ShaderComponent* shaderComp;
                DvigEngine2::INode* node;

                engine->Create <DvigEngine2::GeometryComponent> ( &geomComp, "MyGeometryComponent_0" );
                engine->Create <DvigEngine2::TransformComponent> ( &transComp, "MyTransformComponent_0" );
                engine->Create <DvigEngine2::ShaderComponent> ( &shaderComp, "MyShaderComponent_0" );
                engine->Create <DvigEngine2::INode> ( &node, "MyNode_0" );

                geomComp->Init( nullptr, nullptr, &vertices[0], &indices[0],
                                sizeof(DvigEngine2::defloat32) * 9,
                                sizeof(DvigEngine2::deuint32) * 3
                              );
                transComp->Init( 0.0f, 0.0f, 0.0f );
                shaderComp->Init( "C:\\Users\\USER100\\Documents\\GitHub\\DvigEngine\\files\\shader.vert",
                                  "C:\\Users\\USER100\\Documents\\GitHub\\DvigEngine\\files\\shader.frag" );
                
                engine->AddComponent <DvigEngine2::GeometryComponent> ( &node, geomComp );
                engine->AddComponent <DvigEngine2::TransformComponent> ( &node, transComp );
                engine->AddComponent <DvigEngine2::ShaderComponent> ( &node, shaderComp );
            }
            void Update() override final {
                DvigEngine2::Engine* engine = DvigEngine2::Engine::GetClassInstance();
                DvigEngine2::Application* app = this->GetApplication();
                DvigEngine2::INode* myNode_0 = (DvigEngine2::INode*)engine->GetExistingInstance( "MyNode_0" );
                DvigEngine2::ShaderComponent* myNode_0_Shader = (DvigEngine2::ShaderComponent*)myNode_0->GetComponent<DvigEngine2::ShaderComponent>(nullptr);

                DvigEngine2::GL4::Viewport( 0, 0, 640, 480 );
                DvigEngine2::GL4::ClearColor( 0.0f, 0.0f, 1.0f, 1.0f );
                DvigEngine2::GL4::Clear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
                DvigEngine2::RenderingSystem::BeginRender();

                DvigEngine2::RenderingSystem::BeginBatch();
                DvigEngine2::RenderingSystem::Draw( myNode_0 );
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