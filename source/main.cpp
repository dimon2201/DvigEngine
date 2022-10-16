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
    DV_XMACRO_CREATE_STRING(strings, 0, myString1, "MyStringID_Second1", "ABC");
    DV_XMACRO_CREATE_STRING(strings, 1, myString2, "MyStringID_Second2", " DE");
    DV_XMACRO_CREATE_STRING(strings, 2, myString3, "MyStringID_Second3", " XY");

    // std::cout << "second1 : " << (dvmachword)strings[1]->GetMemoryObject() << std::endl;
    // engine->DeleteObject( strings[0]->GetMemoryObject() );
    engine->DeleteObject( *strings[1]->GetMemoryObject() );
    // engine->DeleteObject( strings[2]->GetMemoryObject() );

    // Create entities
    Entity* entities[2];
    engine->Create<Entity>((const void** const)&entities[0], "EntityID_0", nullptr);

    engine->DeleteObject( *strings[2]->GetMemoryObject() );
    // engine->DeleteObject( strings[2]->GetMemoryObject() );

    engine->StopThreads();
    clock_t te = clock();

    std::cout << "Output : ";
    for (dvisize i = 0; i < 3; ++i)
    {
        if (strings[i] != nullptr) {
            std::cout << "(" << i << ")" << strings[i]->GetData()->m_Chars;
        }
    }
    std::cout << entities[0]->GetSID ();
    std::cout << std::endl;

    std::cout << "Success!" << std::endl;
    std::cout << "Time : " << te - ts << std::endl;

    Engine::Free();

    return 0;
}