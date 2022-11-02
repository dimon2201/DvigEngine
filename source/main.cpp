#include "../include/DECore.hpp"


int main()
{
    DvigEngine2::MemoryPoolProperty memoryPoolsData[4];
    memoryPoolsData[0].m_ByteWidth = 1 * 1024 * 1024;
    memoryPoolsData[1].m_ByteWidth = 465536;
    memoryPoolsData[2].m_ByteWidth = 465536;
    memoryPoolsData[3].m_ByteWidth = 465536;

    DvigEngine2::EngineInputProperty engineInputData;
    engineInputData.m_Version = DV_ENGINE_VERSION_NUMBER;
    engineInputData.m_MemoryPoolsCount = 4u;
    engineInputData.m_MemoryPoolsData = memoryPoolsData;
    engineInputData.m_SystemMemoryPoolIndex = 1;
    engineInputData.m_PrototypeStorageMemoryPoolIndex = 2;
    engineInputData.m_StorageMemoryPoolIndex = 3;
    engineInputData.m_RequestedThreadCount = 1;

    DvigEngine2::Engine::Init(&engineInputData);
    DvigEngine2::Engine* engine = DvigEngine2::Engine::GetClassInstance();

    DvigEngine2::ICommon* commons[2];
    engine->Create( (DvigEngine2::INode**)&commons[0], "MyNode_0", nullptr );

    // node[0]->AddComponent(&comp);

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