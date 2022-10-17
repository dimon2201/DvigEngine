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
    class MemoryObject;
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
    typedef dvuchar             dvstring[DV_MEMORY_COMMON_STRING_BYTE_WIDTH];
    typedef void                (*dvcallback)(dvmachword*, dvusize);

    /*** Declaration & Definition ***/
    class ICommon
    {
        DV_XMACRO_DECLARE_COMMON_CLASS(ICommon)
        DV_MACRO_FRIENDS(DvigEngine::Engine)
    };

    class IShell : public ICommon
    { };

    struct IData
    { };

    class IObject : public ICommon
    {
        DV_MACRO_DECLARE_CREATION_DEPENDENT_CLASS(IObject)
        DV_MACRO_FRIENDS(DvigEngine::Engine)

        public:
            DV_FUNCTION_INLINE IObject** GetCreatee() { return m_Createe; }
            DV_FUNCTION_INLINE MemoryObject** GetMemoryObject() { return &m_MemoryObject; };

        private:
            void SetCreateeAndMemoryObject(IObject** createe, MemoryObject* memoryObject);

        private:
            IObject** m_Createe;
            MemoryObject* m_MemoryObject;
    };

    struct MEMORY_OBJECT_DATA : IData
    {
        public:
            void* m_Address;
            dvusize m_ByteWidth;
            dvmachword m_MemoryPoolID;
    };

    class MemoryObject: public IObject
    {
        public:
            DV_MACRO_FRIENDS(DvigEngine::Engine, DvigEngine::IShell)
            DV_XMACRO_GETTER_DATA(MEMORY_OBJECT_DATA)

            template<typename T>
            DV_FUNCTION_INLINE T* Unwrap() {
                return (T*)m_Data.m_Address;
            }

        private:
            MEMORY_OBJECT_DATA m_Data;
    };

    struct MEMORY_POOL_DATA : IData
    {
        public:
            dvmachword m_ID;
            dvstring m_Label;
            void* m_Address;
            void* m_AddressOffset;
            dvusize m_ByteWidth;
    };

    class MemoryPool : public IObject
    {
        public:
            DV_MACRO_FRIENDS(DvigEngine::Engine, DvigEngine::IShell)
            DV_XMACRO_GETTER_DATA(MEMORY_POOL_DATA)

        private:
            MEMORY_POOL_DATA m_Data;
            MemoryObject* m_MemoryObject;
    };

    struct STRING_DATA : IData
    {
        public:
            STRING_DATA() {};
            STRING_DATA(const char* str);

            dvstring m_Chars;
            dvusize m_ByteWidth;
    };

    class String : public IObject
    {
        public:
            DV_MACRO_FRIENDS(DvigEngine::Engine, DvigEngine::IShell)
            DV_XMACRO_GETTER_DATA(STRING_DATA)

            static dvusize CharactersCount(dvstring op1);
            static dvresult Compare(STRING_DATA* op1, STRING_DATA* op2);
            static dvresult CompareCharacters(dvstring op1, dvstring op2);
            static MemoryObject* ConcateCharacters(dvstring op1, dvstring op2);

            DV_FUNCTION_INLINE String& operator=(const char* str) { m_Data = STRING_DATA(str); return *this; }
            DV_FUNCTION_INLINE dvuchar* operator()() { return &m_Data.m_Chars[0]; }
            
        private:
            STRING_DATA m_Data;
    };

    struct LINKED_LIST_DATA_ENTRY : IData
    {
        LINKED_LIST_DATA_ENTRY* m_PrevAddress;
        MemoryObject* m_Value;
    };

    struct LINKED_LIST_DATA : IData
    {
        dvusize m_EntryCount;
        dvusize m_EntryByteWidth;
        dvusize m_EntryValueByteWidth;
        LINKED_LIST_DATA_ENTRY* m_Head;
    };

    class LinkedList : public IObject
    {
        public:
            DV_MACRO_FRIENDS(DvigEngine::Engine, DvigEngine::IShell)
            DV_XMACRO_GETTER_DATA(LINKED_LIST_DATA)

            LINKED_LIST_DATA_ENTRY* MakeEntry(void* const value);

            void Init(const dvusize capacity, const dvusize entryValueByteWidth);
            dvint32 Insert(void* const value);
            void Replace(const dvint32 index, void* const value);
            void* Find(const dvint32 index);

        private:
            LINKED_LIST_DATA m_Data;
    };

    struct HASH_MAP_DATA_ENTRY : IData
    {
        public:
            HASH_MAP_DATA_ENTRY() {};
            HASH_MAP_DATA_ENTRY(String* key, void* value);
            
            STRING_DATA m_Key;
            void* m_Value;
    };

    struct HASH_MAP_DATA : IData
    {
        public:
            void* m_AssocAddress;
            dvusize m_AssocEntryByteWidth;
            dvusize m_ListEntryCount;
            dvqword m_HashTable[DV_MEMORY_COMMON_HASH_MAP_TABLE_BYTE_WIDTH];
            LinkedList m_LinkedList;
    };

    class HashMap : public IObject
    {
        public:
            DV_MACRO_FRIENDS(DvigEngine::Engine, DvigEngine::IShell)
            DV_XMACRO_GETTER_DATA(HASH_MAP_DATA)

            static dvuint32 Hash(STRING_DATA* input);

            void Init(void* const assocAddress, const dvusize assocEntryByteWidth);
            void Insert(STRING_DATA* key, void* value);
            void* Find(STRING_DATA* key);

        private:
            HASH_MAP_DATA m_Data;
    };

    struct IComponent : IData
    { };

    struct ENTITY_DATA : IData
    {
        public:
            void* m_Offset;
            dvuint32 m_ComponentsBits[DV_COMPONENT_BIT_MASK_BYTE_WIDTH];
    };

    class Entity : public IObject
    {
        public:
            DV_MACRO_FRIENDS(DvigEngine::Engine, DvigEngine::IShell)
            DV_XMACRO_GETTER_DATA(ENTITY_DATA)
        
        private:
            ENTITY_DATA m_Data;
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
    
    class JobQueue : public IObject
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

    struct ENGINE_INPUT_DATA : IData
    {
        public:
            dvdword m_Version;
            dvuint32 m_MemoryPoolsCount;
            MEMORY_POOL_DATA* m_MemoryPoolsData;
            dvmachint m_ReservedMemoryPoolID;
            dvmachint m_StorageMemoryPoolID;
            dvusize m_RequestedThreadCount;
    };

    struct ENGINE_DATA : IData
    {
        public:
            MemoryPool* m_MemoryPools;
            HashMap m_Registry;
            dvusize m_RegisteredComponentCount;
            dvmachword m_CurrentJobQueueCursor;
            JobQueue* m_JobQueues;
            dvusize m_RequestedThreadCount;
            dvusize m_MaxThreadCount;
            void* m_UserData;
    };

    /*** Engine class declaration ***/
    class Engine
    {
        DV_MACRO_DECLARE_SINGLETON(Engine, public)

        public:
            DV_XMACRO_GETTER_DATA(ENGINE_DATA)

            static void Init(ENGINE_INPUT_DATA* engineInputData);
            static void Free();
            static void* Allocate(dvusize memoryPoolID, dvusize byteWidth);
            static MemoryObject* AllocateObject(dvusize memoryPoolID, dvusize byteWidth);
            static void* AllocateUsingData(MEMORY_POOL_DATA* memoryPool, dvusize byteWidth);
            static void DeleteObject(MemoryObject** ppMemoryObject);
            static void CopyMemory(void* destAddress, void* srcAddress, dvusize byteWidth);
            static void MoveMemory(void* destAddress, void* srcAddress, dvusize byteWidth);

            DV_FUNCTION_INLINE MemoryPool* GetMemoryPoolByID(dvusize memoryPoolID) {
                DV_ASSERT_PTR(m_Instance)
                DV_ASSERT_PTR(m_Instance->m_Data.m_MemoryPools)
                return &(m_Instance->m_Data.m_MemoryPools[memoryPoolID]);
            }
            
            void StartThreads();
            void StopThreads();

        public:
            // Wrapper functions
            template<typename T>
            DV_FUNCTION_INLINE void Register() {
                
            }
            
            template<typename T>
            DV_FUNCTION_INLINE void Create(const void** const result, const char* stringID, IData* data) {
                // DV_XMACRO_PUSH_JOB(CallCreate<T>, m_Instance, result, stringID, data)
                dvmachword argumentMemory[3] = { (dvmachword)result, (dvmachword)stringID, (dvmachword)data };
                CallCreate<T>(&argumentMemory[0], 0);
            }

        private:
            // Job functions
            template<typename T>
            DV_FUNCTION_INLINE void CallCreate(dvmachword* argumentMemory, dvusize jobIndex) {
                const T** result = (const T**)argumentMemory[ 0 ];
                const char* stringID = (const char*)argumentMemory[ 1 ];
                IData* data = (IData*)argumentMemory[ 2 ];
                MemoryObject* memoryObject = AllocateObject(0, sizeof(T));
                ICommon* cmnObj = memoryObject->Unwrap<ICommon>();
                cmnObj->SetSID( &stringID[0] );
                IObject* obj = (IObject*)cmnObj;
                obj->SetCreateeAndMemoryObject( (IObject**)result, memoryObject );
                T* typedObj = (T*)obj;
                *result = typedObj;
                // Engine::CopyMemory((void*)typedObj->GetSID(), (void*)&stringID[0], typedObj->Char());
                if (data == nullptr) { return; }
                IData* objectData = typedObj->GetData();
                Engine::CopyMemory(objectData, data, sizeof(typedObj->m_Data));
            }

        private:
            ENGINE_INPUT_DATA m_InputData;
            ENGINE_DATA m_Data;
    };
}

#endif