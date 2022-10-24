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

    engine->PrototypeAddSharedComponent <MyComponent1> ( prototype[0], nullptr );
    engine->PrototypeAddSharedComponent <Comp0> ( prototype[1], nullptr );
    engine->PrototypeAddSharedComponent <Comp1> ( prototype[0], nullptr );

    Instance* instances[2];
    engine->PrototypeInstantiate( &instances[0], "inst_proto_0", prototype[0] );
    engine->PrototypeInstantiate( &instances[1], "inst_proto_1", prototype[1] );

    std::cout << "addrs : " << (demachword)instances[1] << " " << (demachword)instances[0] << std::endl;

    // MyComponent1 comp1; comp1.a = 1;
    // engine->InstanceAddComponent <MyComponent1> ( instances[0], &comp1 );
    Comp0 comp2; comp2.a = 2;
    engine->InstanceAddComponent <Comp0> ( instances[1], &comp2 );
    Comp1 comp3; comp3.a = 3;
    engine->InstanceAddComponent <Comp1> ( instances[0], &comp3 );
    MyComponent1 comp1; comp1.a = 1;
    engine->InstanceAddComponent <MyComponent1> ( instances[0], &comp1 );

    MyComponent1* getComp1 = engine->InstanceGetComponent <MyComponent1> ( instances[0] );
    Comp1* getComp3 = engine->InstanceGetComponent <Comp1> ( instances[0] );
    std::cout << "look" << std::endl;
    Comp0* getComp2 = engine->InstanceGetComponent <Comp0> ( instances[1] );

    std::cout << getComp1->a << " " << getComp2->a << " " << getComp3->a << std::endl;

    Engine::Free();

    std::cout << "Success!" << std::endl;

    return 0;
}