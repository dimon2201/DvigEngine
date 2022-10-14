#ifndef _DV_H_
#define _DV_H_

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <iostream>
#include <typeinfo>
#include <functional>
#include <thread>
#include <atomic>
#include <cmath>

#include "DvigEngineMacros.hpp"

namespace DvigEngine
{
    /*** Forward declaration & Prototypes ***/
    class IShell;
    class String;
    class Engine;

    /*** Typenames ***/
    typedef int8_t              dvint8;
    typedef uint8_t             dvuint8;
    typedef int16_t             dvint16;
    typedef uint16_t            dvuint16;
    typedef int32_t             dvint32;
    typedef uint32_t            dvuint32;
    typedef int64_t             dvint64;
    typedef uint64_t            dvuint64;
    #if defined(DV_MACRO_ARCH_32_BIT)
        typedef int32           dvmachint;
        typedef uint32          dvmachuint;
        typedef machuint        dvmachword;
    #elif defined(DV_MACRO_ARCH_64_BIT)
        typedef dvint64         dvmachint;
        typedef dvuint64        dvmachuint;
        typedef dvmachuint      dvmachword;
    #endif
    typedef dvuint8             dvuchar;
    typedef dvmachint           dvisize;
    typedef size_t              dvusize;
    typedef dvuint32            dvdword;
    typedef dvuint64            dvqword;
    typedef dvmachword          dvresult;
    typedef void                (*dvcallback)(dvmachword*, dvusize);

    /*** Job function wrappers ***/
    namespace WrapperFunctions
    {
        DV_FUNCTION_INLINE void EngineCreate(Engine* engine) {
            
        }
    }

    /*** Declaration & Definition ***/
    class ICommon
    {
        public:
            DV_XMACRO_DECLARE_COMMON_CLASS(ICommon)

            ICommon(const char* str);
    };

    class IShell : ICommon
    { };

    struct IData
    { };

    struct IObject : ICommon
    {
        DV_MACRO_DECLARE_CREATION_DEPENDENT_CLASS(IObject)
    };

    struct MEMORY_POOL_DATA : IData
    {
        public:
            dvmachword m_ID;
            dvuchar m_Label[DV_MEMORY_COMMON_STRING_BYTE_WIDTH];
            void* m_Address;
            void* m_AddressOffset;
            dvusize m_ByteWidth;
    };

    class MemoryPool : IObject
    {
        public:
            DV_MACRO_FRIENDS(DvigEngine::Engine, DvigEngine::IShell)
            DV_XMACRO_GETTER_DATA(MEMORY_POOL_DATA)

        private:
            MEMORY_POOL_DATA m_Data;
    };

    struct STRING_DATA : IData
    {
        public:
            STRING_DATA() {};
            STRING_DATA(const char* str);

            dvuchar m_Chars[DV_MEMORY_COMMON_STRING_BYTE_WIDTH];
            dvusize m_ByteWidth;
    };

    class String : IObject
    {
        public:
            DV_MACRO_FRIENDS(DvigEngine::Engine, DvigEngine::IShell)
            DV_XMACRO_GETTER_DATA(STRING_DATA)

            static dvresult Compare(STRING_DATA* op1, STRING_DATA* op2);

        private:
            STRING_DATA m_Data;
    };

    struct HASH_MAP_DATA_SLOT : IData
    {
        public:
            HASH_MAP_DATA_SLOT() {};
            HASH_MAP_DATA_SLOT(String* key, void* value);
            
            STRING_DATA m_Key;
            void* m_Value;
    };

    struct HASH_MAP_DATA : IData
    {
        public:
            HASH_MAP_DATA() {}
            HASH_MAP_DATA(dvusize assocEntrySize) : m_AssocEntrySize(assocEntrySize) {}

            void* m_AssocAddress;
            dvusize m_AssocEntrySize;
            dvusize m_ListEntryCount;
            HASH_MAP_DATA_SLOT m_List[DV_MAX_HASH_MAP_LIST_ENTRY_COUNT];
            dvisize m_HashTable[DV_MEMORY_COMMON_HASH_MAP_TABLE_BYTE_WIDTH];
    };

    class HashMap : IObject
    {
        public:
            DV_MACRO_FRIENDS(DvigEngine::Engine, DvigEngine::IShell)
            DV_XMACRO_GETTER_DATA(HASH_MAP_DATA)

            static dvuint32 Hash(String* input);
            void Insert(String* key, void* value);
            void* Find(String* key);

        private:
            HASH_MAP_DATA m_Data;
    };

    struct MEMORY_CHUNK_DATA : IData
    {
        public:
            void* m_Address;
            dvusize m_ByteWidth;
    };

    class MemoryChunk : IObject
    {
        public:
            DV_MACRO_FRIENDS(DvigEngine::Engine, DvigEngine::IShell)
            DV_XMACRO_GETTER_DATA(MEMORY_CHUNK_DATA)

            template<typename T>
            DV_FUNCTION_INLINE T* Unwrap() {
                return (T*)m_Data.m_Address;
            }

        private:
            MEMORY_CHUNK_DATA m_Data;
    };

    struct ENTITY_DATA : IData
    {
        public:
            void* m_Offset;
            dvuint32 m_ComponentsBits[DV_COMPONENT_BIT_MASK_BYTE_WIDTH];
    };

    class Entity : IObject
    {
        public:
            DV_MACRO_FRIENDS(DvigEngine::Engine, DvigEngine::IShell)
            DV_XMACRO_GETTER_DATA(ENTITY_DATA)
        
        private:
            ENTITY_DATA m_Data;
    };

    enum JOB_TYPE
    {
        CREATE = 0u
    };

    struct JOB_QUEUE_DATA : IData
    {
        std::atomic<dvmachword> m_StopFlag;
        std::atomic<dvmachword> m_ReturnFlag;
        std::thread m_Thread;
        dvdword m_JobCount;
        dvmachword m_JobArguments[DV_MAX_JOB_QUEUE_THREAD_JOB_ARGUMENT_COUNT * DV_MAX_JOB_QUEUE_THREAD_JOB_COUNT];
        dvcallback m_Jobs[DV_MAX_JOB_QUEUE_THREAD_JOB_COUNT];
    };
    
    class JobQueue : IObject
    {
        public:
            DV_MACRO_FRIENDS(DvigEngine::Engine, DvigEngine::IShell)
            DV_XMACRO_GETTER_DATA(JOB_QUEUE_DATA)
            
            void Push(dvcallback callback, void* argumentMemory, const dvusize argumentCount);
            void Start();
            void Stop();

        private:
            JOB_QUEUE_DATA m_Data;
    };

    struct ENGINE_USER_DATA : IData
    {
        public:
            dvdword m_Version;
            dvuint32 m_MemoryPoolsCount;
            dvmachint m_ReservedMemoryPoolID;
            MEMORY_POOL_DATA* m_MemoryPoolsData;
            dvusize m_RequestedThreadCount;
    };

    struct ENGINE_DATA : IData
    {
        public:
            void* m_GlobalMemoryPoolAddress;
            dvusize m_GlobalMemoryPoolByteWidth;
            dvusize m_RegisteredComponentCount;
            void* m_ComponentStorage;
            void* m_ComponentStorageOffset;
            dvusize m_ComponentStorageByteWidth;
            MemoryPool* m_MemoryPools;
            dvmachword m_CurrentJobQueueCursor;
            JobQueue* m_JobQueues;
            dvusize m_RequestedThreadCount;
            dvusize m_MaxThreadCount;
    };

    /*** Engine class declaration ***/
    class Engine
    {
        DV_MACRO_DECLARE_SINGLETON(Engine, public)

        public:
            DV_XMACRO_GETTER_DATA(ENGINE_DATA)

            static void Init(ENGINE_USER_DATA* engineInfo);
            static void Free();
            static void* Allocate(dvusize memoryPoolID, dvusize byteWidth);
            static MemoryChunk* AllocateChunk(dvusize memoryPoolID, dvusize byteWidth);
            static void* AllocateUsingData(MEMORY_POOL_DATA* memoryPool, dvusize byteWidth);
            static void CopyMemory(void* destAddress, void* srcAddress, dvusize byteWidth);

            DV_FUNCTION_INLINE MemoryPool* GetMemoryPoolByID(dvusize memoryPoolID) {
                DV_ASSERT_PTR(m_Instance)
                DV_ASSERT_PTR(m_Instance->m_Data.m_MemoryPools)
                return &(m_Instance->m_Data.m_MemoryPools[memoryPoolID]);
            }

        public:
            template<typename T>
            DV_FUNCTION_INLINE void Create(const void** const result, const char* stringID, IData* data) {
                DV_XMACRO_PUSH_JOB(CallCreate<T>, m_Instance, result, stringID, data)
            }

            void StartThreads();
            void StopThreads();

        private:
            // Job functions
            template<typename T>
            DV_FUNCTION_INLINE void CallCreate(dvmachword* argumentMemory, dvusize jobIndex) {
                MemoryChunk* object = AllocateChunk(0, sizeof(T));
                T* unwObject = object->Unwrap<T>();
                const T** result = (const T**)argumentMemory[ 0 ];
                const char* stringID = (const char*)argumentMemory[ 1 ];
                IData* data = (IData*)argumentMemory[ 2 ];
                dvuchar* stringIDPtr = (dvuchar*)stringID;
                while (*++stringIDPtr != 0);
                Engine::CopyMemory((void*)&unwObject->m_SID[0], (void*)&stringID[0], (dvusize)(stringIDPtr - (dvuchar*)stringID));
                if (data == nullptr) { *result = unwObject; return; /*unwObject;*/ }
                IData* objectData = unwObject->GetData();
                Engine::CopyMemory(objectData, data, sizeof(unwObject->m_Data));
                *result = unwObject;
            }

        private:
            ENGINE_USER_DATA m_UserData;
            ENGINE_DATA m_Data;
    };
}

#endif