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

    Engine::Init(&engineInputData);
    Engine* engine = Engine::GetClassInstance();

    DV_XMACRO_DEFINE_COMPONENT( MyComponent1, deuint32, a, deuint32, b );
    DV_XMACRO_DEFINE_COMPONENT( MyComponent2, deuint32, a );
    engine->RegisterComponent <MyComponent1> ();
    engine->RegisterComponent <MyComponent2> ();

    Prototype* prototype[1];
    engine->ObjectCreate <Prototype> ( &prototype[0], "Prototype_0", nullptr );
    engine->PrototypeSetComponent <MyComponent1> ( prototype[0] );
    engine->PrototypeSetComponent <MyComponent2> ( prototype[0] );

    Instance* protoInstance0 = engine->PrototypeInstantiate ( "Prototype_0_Instance_0", prototype[0] );
    MyComponent1 myComponent1; myComponent1.a = 255;
    MyComponent2 myComponent2; myComponent2.a = 128;
    engine->InstanceAddComponent <MyComponent1> ( protoInstance0, &myComponent1 );
    engine->InstanceAddComponent <MyComponent2> ( protoInstance0, &myComponent2 );
    MyComponent1* getMyComponent1 = engine->InstanceGetComponent <MyComponent1> ( protoInstance0 );
    MyComponent2* getMyComponent2 = engine->InstanceGetComponent <MyComponent2> ( protoInstance0 );
    std::cout << getMyComponent1->a << " " << getMyComponent2->a << std::endl;

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