#include <iostream>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <thread>
#include "../include/DvigEngine.hpp"

using namespace DvigEngine;

class MemoryPoolShell : IShell
{
    public:
        void printMemoryPoolInfo(Engine* engine)
        {
            MEMORY_POOL_DATA* mp0 = (MEMORY_POOL_DATA*)engine->GetMemoryPoolByID(0)->GetData();
            MEMORY_POOL_DATA* mp1 = (MEMORY_POOL_DATA*)engine->GetMemoryPoolByID(1)->GetData();
            
            std::cout << "Pool 0 Address : " << (dvmachword)mp0->m_Address << std::endl;
            std::cout << "Pool 1 Address : " << (dvmachword)mp1->m_Address << std::endl;
        }
};

int main()
{
    MEMORY_POOL_DATA memoryPoolsData[3];
    memoryPoolsData[0].m_ByteWidth = 65536;
    memoryPoolsData[1].m_ByteWidth = 65536;
    memoryPoolsData[2].m_ByteWidth = 65536;

    ENGINE_INPUT_DATA engineInputData;
    engineInputData.m_Version = DV_ENGINE_VERSION_NUMBER;
    engineInputData.m_MemoryPoolsCount = 3u;
    engineInputData.m_MemoryPoolsData = memoryPoolsData;
    engineInputData.m_ReservedMemoryPoolID = 1;
    engineInputData.m_StorageMemoryPoolID = 2;
    engineInputData.m_RequestedThreadCount = 1;

    Engine::Init(&engineInputData);
    Engine* engine = Engine::GetInstance();
    
    clock_t ts = clock();
    engine->StartThreads();

    String* strings[128];
    DV_XMACRO_CREATE_STRING(strings, 0, helloString, "HelloStringID_0", "hello");

    HashMap* hashMaps[1];
    engine->Create<HashMap>((const void** const)&hashMaps[0], "MyHashMap_0", nullptr);
    hashMaps[0]->Init();
    hashMaps[0]->Insert((*helloString)->GetData()->m_Chars, (void*)255);
    void* value = hashMaps[0]->Find((*helloString)->GetData()->m_Chars);
    std::cout << (dvmachword)value << std::endl;

    struct MyComponent : IComponent { dvint32 a = 1; };
    struct AnotherComponent : IComponent { dvint32 a = 2; };
    engine->RegisterComponent<MyComponent>();
    engine->RegisterComponent<AnotherComponent>();
    // Create entities
    Entity* entities[2];
    ENTITY_DATA entityData;
    engine->Create<Entity>((const void** const)&entities[0], "EntityID_0", &entityData);
    MyComponent component;
    AnotherComponent anotherComponent;
    engine->AddComponent<MyComponent>( entities[0], &component );
    engine->AddComponent<AnotherComponent>( entities[0], &anotherComponent );
    MyComponent* c1 = engine->GetComponent<MyComponent>( entities[0] );
    AnotherComponent* c2 = engine->GetComponent<AnotherComponent>( entities[0] );
    std::cout << c1->a << " " << c2->a << std::endl;

    engine->StopThreads();
    clock_t te = clock();

    std::cout << "Success!" << std::endl;
    std::cout << "Time : " << te - ts << std::endl;

    Engine::Free();

    return 0;
}