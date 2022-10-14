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
    engineInputData.m_MemoryPoolsCount = 2u;
    engineInputData.m_MemoryPoolsData = memoryPoolsData;
    engineInputData.m_ReservedMemoryPoolID = 1;
    engineInputData.m_StorageMemoryPoolID = 2;
    engineInputData.m_RequestedThreadCount = 1;

    Engine::Init(&engineInputData);
    Engine* engine = Engine::GetInstance();
    
    clock_t ts = clock();
    engine->StartThreads();

    // Create strings
    String* strings[2]; // memory for 2 Strings
    engine->Create<String>((const void** const)&strings[0], "MyStringID_0", nullptr);
    engine->Create<String>((const void** const)&strings[1], "MyStringID_1", nullptr);
    strings[0][0] = "Hello";
    strings[1][0] = " world!";
    
    DV_XMACRO_CREATE_STRING_GLOBAL_SCOPE_CAPACITY(4);
    DV_XMACRO_CREATE_STRING(myString, "MyStringID_Second", "How are you doing?");
    DV_XMACRO_CREATE_STRING(anotherString, "MyStringID_Second2", "...");
    DV_XMACRO_CREATE_STRING(andAnotherString, "MyStringID_Second22", "Wow!");
    engine->DeleteChunk( anotherString->m_MemoryObject );
    
    // Create entities
    Entity* entities[2];
    engine->Create<Entity>((const void** const)&entities[0], "EntityID_0", nullptr);

    // HashMap* hashMaps[2];
    // engine->Create<String>((const void** const)&hashMaps[0], "MyHashMap", nullptr);
    // hashMaps[0]->Insert(  );

    engine->StopThreads();
    clock_t te = clock();
    
    std::cout << "Output : " << anotherString->GetData()->m_Chars << strings[1]->GetData()->m_Chars << std::endl;
    std::cout << "Success!" << std::endl;
    std::cout << "Time : " << te - ts << std::endl;

    Engine::Free();

    return 0;
}