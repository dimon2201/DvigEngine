#include "../include/DECore.hpp"

int main()
{
    DvigEngine2::MemoryPoolProperty memoryPoolsData[2];
    memoryPoolsData[0].m_ByteWidth = DV_MEMORY_MiB;
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

    DvigEngine2::INode* nodes[1];
    engine->Create<DvigEngine2::INode>( &nodes[0], "MyNode_0", nullptr );
    nodes[0]->Init();

    DV_XMACRO_DEF_COMPONENT( MyComp, unsigned, val );
    engine->RegisterComponent<MyComp>();

    MyComp* components[3];
    engine->Create<MyComp>( &components[0], "MyComponent_0", nullptr );
    components[0]->val = 255;
    engine->Create<MyComp>( &components[1], "MyComponent_1", nullptr );
    components[1]->val = 127;
    engine->Create<MyComp>( &components[2], "MyComponent_2", nullptr );
    components[2]->val = 111;

    engine->AddComponent<MyComp>(&nodes[0], components[0]);

    MyComp* getComp = (MyComp*)nodes[0]->GetComponent("MyComponent_0");
    std::cout << getComp->val << std::endl;

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