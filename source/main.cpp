#include "../include/DECore.hpp"
#include "../include/DEGraphics.hpp"

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
    engineInputData.m_RequestedThreadCount = 1;

    DvigEngine2::Engine::Init(&engineInputData);
    DvigEngine2::Engine* engine = DvigEngine2::Engine::GetClassInstance();
    engine->RegisterComponent <DvigEngine2::GeometryComponent> ();

    // Vertices
    DvigEngine2::deusize vertexCount = 9;
    float vertices[9] = {
        -1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        1.0f, -1.0f, 0.0f
    };

    DvigEngine2::GeometryComponent* geometryComp[1];
    engine->Create <DvigEngine2::GeometryComponent> ( &geometryComp[0], "MyGeometryComponent_0", nullptr );
    geometryComp[0]->Init( nullptr, (void*)&vertices[0], 4 * vertexCount );
    geometryComp[0]->val = 255;

    DvigEngine2::INode* nodes[1];
    engine->Create <DvigEngine2::INode> ( &nodes[0], "MyNode_0", nullptr );
    nodes[0]->Init();
    engine->AddExistingComponent <DvigEngine2::GeometryComponent> ( &nodes[0], "MyGeometryComponent_0" );
    DvigEngine2::GeometryComponent* getComponent = (DvigEngine2::GeometryComponent*)nodes[0]->GetComponent( "MyGeometryComponent_0" );
    std::cout << getComponent->val << std::endl;

    /*
        Node
          GeometryComponent
        Node
          GeometryComponent

        Draw()
          Bind vertex buffer
          Bind uniform buffer
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