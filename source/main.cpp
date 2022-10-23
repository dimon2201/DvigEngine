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
    DV_XMACRO_DEFINE_COMPONENT( Comp0, deuint32, a, deuint32, b );
    DV_XMACRO_DEFINE_COMPONENT( Comp1, deuint32, a );
    engine->RegisterComponent <MyComponent1> ();
    engine->RegisterComponent <MyComponent2> ();
    engine->RegisterComponent <Comp0> ();
    engine->RegisterComponent <Comp1> ();

    Prototype* prototype[2];
    engine->ObjectCreate <Prototype> ( &prototype[0], "Prototype_0", nullptr );
    engine->ObjectCreate <Prototype> ( &prototype[1], "Prototype_1", nullptr );
    MyComponent2 myComponent2_1; myComponent2_1.a = 155;
    engine->PrototypeAddSharedComponent <MyComponent1> ( prototype[0], nullptr );
    engine->PrototypeAddSharedComponent <MyComponent2> ( prototype[0], &myComponent2_1 );
    engine->PrototypeAddSharedComponent <Comp0> ( prototype[1], nullptr );
    engine->PrototypeAddSharedComponent <Comp1> ( prototype[1], nullptr );

    Instance* protoInstance0 = engine->PrototypeInstantiate ( "Prototype_0_Instance_0", prototype[0] );
    Instance* protoInstance1 = engine->PrototypeInstantiate ( "Prototype_0_Instance_1", prototype[0] );
    Instance* protoCompInstance = engine->PrototypeInstantiate ( "Prototype_0_CompInstance_0", prototype[1] );
    MyComponent1 myComponent1_1; myComponent1_1.a = 255;
    MyComponent1 myComponent1_2; myComponent1_2.a = 205;
    engine->InstanceAddComponent <MyComponent1> ( protoInstance0, &myComponent1_1 );
    engine->InstanceAddComponent <MyComponent1> ( protoInstance0, &myComponent1_2 );
    engine->InstanceAddComponent <MyComponent2> ( protoInstance0, nullptr );
    Comp0 comp0; comp0.b = 99;
    Comp1 comp1; comp1.a = 109;
    engine->InstanceAddComponent <Comp0> ( protoCompInstance, &comp0 );
    engine->InstanceAddComponent <Comp1> ( protoCompInstance, &comp1 );
    // MyComponent1* getMyComponent1 = engine->InstanceGetComponent <MyComponent1> ( protoInstance0 );
    // MyComponent2* getMyComponent2 = engine->InstanceGetComponent <MyComponent2> ( protoInstance0 );
    // Comp0* getComp0 = engine->InstanceGetComponent <Comp0> ( protoCompInstance );
    // Comp1* getComp1 = engine->InstanceGetComponent <Comp1> ( protoCompInstance );
    // std::cout << getMyComponent1->a << " " << getMyComponent2->a <<std::endl;
    // std::cout << getComp0->b << " " << getComp1->a <<std::endl;

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