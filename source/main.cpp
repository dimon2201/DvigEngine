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
    memoryPoolsData[0].m_ByteWidth = 465536;
    memoryPoolsData[1].m_ByteWidth = 22465536;
    memoryPoolsData[2].m_ByteWidth = 465536;

    ENGINE_INPUT_DATA engineInputData;
    engineInputData.m_Version = DV_ENGINE_VERSION_NUMBER;
    engineInputData.m_MemoryPoolsCount = 3u;
    engineInputData.m_MemoryPoolsData = memoryPoolsData;
    engineInputData.m_SystemMemoryPoolID = 1;
    engineInputData.m_StorageMemoryPoolID = 2;
    engineInputData.m_RequestedThreadCount = 1;
    
    /*
        float myVertexBuffer[9];
        BufferC bufferC;
        buffer.CPUVisible = True;
        buffer.GPUVisible = True;
        buffer.pData = &myVertexBuffer;
        Buffer buffer;
        buffer->AddComponent<BufferC>(&bufferC);

        GeometryC geomC;
        geomC.pBuffer = &buffer;
        Create<Entity>(&entity);

        [---------------------]
        [---------------------]
    */

    Engine::Init(&engineInputData);
    Engine* engine = Engine::GetInstance();
    
    clock_t ts = clock();
    engine->StartThreads();

    // String* strings[128];
    // DV_XMACRO_CREATE_STRING(strings, 0, helloString, "HelloStringID_0", "hello");

    HashMap* hashMaps[1];
    engine->Create<HashMap>((const void** const)&hashMaps[0], "MyHashMap_0", nullptr);
    hashMaps[0]->Init();
    // hashMaps[0]->Insert((*helloString)->GetData()->m_Chars, (void*)255);
    // void* value = hashMaps[0]->Find((*helloString)->GetData()->m_Chars);
    // std::cout << (dvmachword)value << std::endl;

    struct MyComponent : IComponent { dvint32 a = 1; dvint32 b = 2; };
    struct AnotherComponent : IComponent { dvint32 a = 2; };
    // class SystemInterface
    // {
    //     public:
    //     virtual ~SystemInterface() {};
    //     virtual void Update(Engine* engine, Entity* object) {};
    //     dvuint32 val;
    // };
    // class MySystem : public SystemInterface
    // {
    //     public:
    //     ~MySystem() {};
    //     void Update(Engine* engine, Entity* object)
    //     {

    //     }
    // };
    class MySystem : public ISystem
    {
        public:
        ~MySystem() {};
        void Update(Engine* engine, Entity* entity)
        {
            AnotherComponent* component = engine->GetComponent<AnotherComponent>(entity);
            std::cout << component->a << std::endl;
        }
    };
    engine->RegisterComponent<MyComponent>();
    engine->RegisterComponent<AnotherComponent>();
    engine->RegisterSystem<MySystem>();
    // // Create entities
    Entity* entities[2];
    // ENTITY_DATA entityData;
    engine->Create<Entity>((const void** const)&entities[0], "EntityID_0", nullptr);
    MyComponent component;
    AnotherComponent anotherComponent;
    engine->AddComponent<MyComponent>( entities[0], &component );
    engine->AddComponent<AnotherComponent>( entities[0], &anotherComponent );
    MyComponent* c1 = engine->GetComponent<MyComponent>( entities[0] );
    AnotherComponent* c2 = engine->GetComponent<AnotherComponent>( entities[0] );
    std::cout << c1->a << " " << c2->a << std::endl;
    std::cout << entities[0]->GetSID () << std::endl;

    engine->UpdateSystems();

    engine->StopThreads();
    clock_t te = clock();

    std::cout << "Success!" << std::endl;
    std::cout << "Time : " << te - ts << std::endl;

    Engine::Free();

    return 0;
}