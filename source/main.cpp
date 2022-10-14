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

float Multithreading(int idx, void* mem, unsigned memByteWidth)
{
    unsigned offset = 0, cnt = memByteWidth;
    if (idx == 1) { offset = memByteWidth / 2; cnt = memByteWidth; } else if (idx == 0) { offset = 0; cnt = memByteWidth / 2; }
    float* address = (float*)((dvmachword)mem + offset);
    float val = 0.0f;
    for (unsigned i = offset; i < cnt; ++i)
    {
        float addrVal = *address++;
        val += 2397483.0f + sqrtf(addrVal) * sqrtf(addrVal * 348354.0f) * sqrtf(addrVal * addrVal * addrVal * sqrtf(addrVal * addrVal));
    }

    return val;
}

int main()
{
    MEMORY_POOL_DATA memoryPoolsData[2];
    memoryPoolsData[0].m_ByteWidth = 65536;
    memoryPoolsData[1].m_ByteWidth = 65536;

    ENGINE_USER_DATA engineUserData;
    engineUserData.m_Version = DV_ENGINE_VERSION_NUMBER;
    engineUserData.m_MemoryPoolsCount = 2u;
    engineUserData.m_MemoryPoolsData = memoryPoolsData;
    engineUserData.m_ReservedMemoryPoolID = 1;
    engineUserData.m_RequestedThreadCount = 1;

    Engine::Init(&engineUserData);
    Engine* engine = Engine::GetInstance();

    String** string = (String**)Engine::Allocate(0, 4 * sizeof(String));
    STRING_DATA string0Data = STRING_DATA("Hello");
    STRING_DATA string1Data = STRING_DATA(" world!");

    float* mem = (float*)malloc(4 * 1024 * 1024);

    engine->StartThreads();
    clock_t ts = clock();
    engine->Create<String>((const void** const)&string[0], "MyStringID_0", &string0Data);
    engine->Create<String>((const void** const)&string[1], "MyStringID_1", &string1Data);
    engine->StopThreads();
    clock_t te = clock();

    std::cout << (string[0])->GetData()->m_Chars << (string[1])->GetData()->m_Chars << std::endl;
    std::cout << "Success!" << std::endl;
    std::cout << "Time : " << te - ts << std::endl;

    Engine::Free();

    // void* mem = malloc(4 * 1024 * 1024);
    // memset(mem, 127, 4 * 1024 * 1024);
    // clock_t ts = clock();
    // std::thread t1(Multithreading, 0, mem, 1 * 1024 * 1024);
    // std::thread t2(Multithreading, 1, mem, 1 * 1024 * 1024);
    // t1.join();
    // t2.join();
    // // float v1 = Multithreading(0, mem, 1 * 1024 * 1024);
    // clock_t te = clock();
    // std::cout << te - ts << std::endl;

    return 0;
}