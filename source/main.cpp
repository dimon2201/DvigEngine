#include "../include/DECore.hpp"

using namespace DvigEngine;

int main()
{
    MEMORY_POOL_DATA memoryPoolsData[4];
    memoryPoolsData[0].m_ByteWidth = 465536;
    memoryPoolsData[1].m_ByteWidth = 465536;
    memoryPoolsData[2].m_ByteWidth = 465536;
    memoryPoolsData[3].m_ByteWidth = 465536;

    ENGINE_INPUT_DATA engineInputData;
    engineInputData.m_Version = DV_ENGINE_VERSION_NUMBER;
    engineInputData.m_MemoryPoolsCount = 4u;
    engineInputData.m_MemoryPoolsData = memoryPoolsData;
    engineInputData.m_SystemMemoryPoolID = 1;
    engineInputData.m_PrototypeStorageMemoryPoolID = 2;
    engineInputData.m_StorageMemoryPoolID = 3;
    engineInputData.m_RequestedThreadCount = 1;

    DV_XMACRO_DEFINE_COMPONENT( MyComponent, demachword, _m_Reserved );

    Engine::Init(&engineInputData);
    Engine* engine = Engine::GetInstance();

    engine->RegisterComponent <MyComponent> ();

    Prototype* prototype[1];
    engine->ObjectCreate <Prototype> ( &prototype[0], "Prototype_0", nullptr );
    engine->PrototypeSetComponent <MyComponent> ( prototype[0] );

    std::cout << engine->GetMemoryPoolByID( 2 )->GetAddressOffset() << std::endl;
    std::cout << engine->GetMemoryPoolByID( 3 )->GetAddressOffset() << std::endl;

    Engine::Free();

    // Prototype** monster;
    // engine->Create <Prototype> ( monster );
    // monster->AddComponent <Geometry> ( &mesh );
    // monster->AddComponent <Script> ( &script );

    // Instance* monster_Instance0 = monster->Instantiate(  );
    // monster_Instance0->SetComponent <Script> ( &userScript );

    std::cout << "Success!" << std::endl;

    return 0;
}