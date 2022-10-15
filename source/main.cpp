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
    String* strings[6];
    DV_XMACRO_CREATE_STRING(strings, 0, myString1, "MyStringID_Second1", "Hello,");
    DV_XMACRO_CREATE_STRING(strings, 1, myString2, "MyStringID_Second2", " man,");
    DV_XMACRO_CREATE_STRING(strings, 2, myString3, "MyStringID_Second3", " How are you doing");
    DV_XMACRO_CREATE_STRING(strings, 3, myString4, "MyStringID_Second4", " today?");

    // DV_XMACRO_CREATE_STRING(anotherString, "MyStringID_Second2", "...");
    // DV_XMACRO_CREATE_STRING(andAnotherString, "MyStringID_Second22", "Wow!");
    // engine->DeleteChunk( myString->m_MemoryObject );
    // std::cout << (dvmachword)strings[0] << std::endl;

    // std::cout << "entity1 : " << (dvmachword)entities[0] << std::endl;
    std::cout << "second1 : " << (dvmachword)strings[1]->GetMemoryObject() << std::endl;
    engine->DeleteObject( strings[2]->GetMemoryObject() );
    
    // Create entities
    Entity* entities[2];
    engine->Create<Entity>((const void** const)&entities[0], "EntityID_0", nullptr);
    // std::cout << "entity2 : " << (dvmachword)entities[0]->GetMemoryObject() << std::endl;
    std::cout << "fourth2 : " << (dvmachword)strings[3]->GetMemoryObject() << std::endl;

        engine->DeleteObject( strings[3]->GetMemoryObject() );


    // HashMap* hashMaps[2];
    // engine->Create<String>((const void** const)&hashMaps[0], "MyHashMap", nullptr);
    // hashMaps[0]->Insert(  );

    engine->StopThreads();
    clock_t te = clock();
    
    std::cout << "Output : " << strings[0]->GetData()->m_Chars
                             << strings[1]->GetData()->m_Chars
                             << strings[0]->GetData()->m_Chars
                             << entities[0]->GetSID()
                             << std::endl;
    std::cout << "Success!" << std::endl;
    std::cout << "Time : " << te - ts << std::endl;

    Engine::Free();

    return 0;
}