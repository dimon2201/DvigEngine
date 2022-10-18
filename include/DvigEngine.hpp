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
        DV_MACRO_FRIENDS(DvigEngine::Engine)
        DV_XMACRO_DECLARE_COMMON_CLASS(ICommon)
    };

    class IShell : public ICommon
    { };

    struct IData
    { };

    class IObject : public ICommon
    {
        DV_MACRO_FRIENDS(DvigEngine::Engine)
        DV_MACRO_DECLARE_CREATION_DEPENDENT_CLASS(IObject)

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

            void Init(const dvusize entryValueByteWidth);
            dvint32 Insert(void* const value);
            void Replace(const dvint32 index, void* const value);
            void* Find(const dvint32 index);

        private:
            LINKED_LIST_DATA m_Data;
    };

    struct HASH_MAP_MEMORY_BLOCK : IData
    {
        dvmachword m_EntryCount;
        dvuchar m_Memory[ DV_MAX_HASH_MAP_MEMORY_BLOCK_BYTE_WIDTH ];
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
            dvusize m_ListEntryCount;
            LinkedList m_MemoryBlocks;
            dvmachword m_HashTable[DV_MEMORY_COMMON_HASH_MAP_TABLE_BYTE_WIDTH];
    };

    struct Entry { dvmachword chars; void* value; };

    class HashMap : public IObject
    {
        public:
            DV_MACRO_FRIENDS(DvigEngine::Engine, DvigEngine::IShell)
            DV_XMACRO_GETTER_DATA(HASH_MAP_DATA)

            static dvuint32 Hash(dvstring input);

            void Init();
            void Insert(dvstring key, void* value);
            void* Find(dvstring key);
            void* FindIndex(const dvint32 index);

        private:
            MemoryObject* AllocateBlock();
            void InsertToMemoryBlock(dvuint32 hash, dvstring key, void* value);

            HASH_MAP_DATA m_Data;
    };

    struct IComponent : IData
    {
        public:
            dvstring m_TypeName;
            dvusize m_LayoutByteWidth;
            dvint32 m_RegistryIndex;
    };

    class ISystem : IObject
    {
        DV_MACRO_FRIENDS(DvigEngine::Engine)
        DV_MACRO_DECLARE_SINGLETON(ISystem, public)
        
        public:
            virtual void Run(void* workMemory);

            dvstring m_TypeName;
            dvint32 m_RegistryIndex;
    };

    struct ENTITY_DATA : IData
    {
        public:
            ENTITY_DATA();

            void* m_SubStorageAddress;
            dvusize m_SubStorageByteWidth;
            dvusize m_ComponentCount;
            dvuint32 m_ComponentBits[DV_COMPONENT_DWORD_COUNT_PER_COMPONENT_COUNT];
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
            dvint32 m_SystemMemoryPoolID;
            dvint32 m_EntityStorageMemoryPoolID;
            dvint32 m_ComponentStorageMemoryPoolID;
            dvusize m_RequestedThreadCount;
    };

    struct ENGINE_REGISTRY_DATA : IData
    {
        public:
            void* m_EntityStorageAddress;
            dvusize m_EntityCount;
            dvint32 m_UniqueComponentCount;
            dvint32 m_UniqueSystemCount;
            HashMap m_TypeAllocationPoolID;
            HashMap m_Components;
            HashMap m_Systems;
    };

    struct ENGINE_DATA : IData
    {
        public:
            MemoryPool* m_MemoryPools;
            dvmachword m_CurrentJobQueueCursor;
            JobQueue* m_JobQueues;
            dvusize m_MaxThreadCount;
            dvusize m_RequestedThreadCount;
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
            void UpdateSystems();

        public:
            // ECS related functions
            template<typename T>
            void RegisterComponent() {
                dvuchar* typeName = (dvuchar*)typeid(T).name();
                const dvint32 componentIndex = ++m_RegistryData.m_UniqueComponentCount;
                if (m_RegistryData.m_Components.Find(typeName) != nullptr) { return; }
                m_RegistryData.m_Components.Insert(typeName, (void*)(dvmachword)componentIndex);
            }

            template<typename T>
            void RegisterSystem() {
                dvuchar* typeName = (dvuchar*)typeid(T).name();
                ++m_RegistryData.m_UniqueSystemCount;
                if (m_RegistryData.m_Systems.Find(typeName) != nullptr) { return; }
                T::m_Instance = (T*)Engine::AllocateObject( 0, sizeof(T) )->GetData()->m_Address;
                m_RegistryData.m_Systems.Insert( typeName, (void*)T::m_Instance );
            }

            template<typename T>
            void AddComponent(Entity* entity, IComponent* ccomponent) {
                dvuchar* typeName = (dvuchar*)typeid(T).name();
                const dvusize typeNameByteWidth = String::CharactersCount( typeName );
                const dvint32 registryIndex = (dvint32)(dvmachword)m_RegistryData.m_Components.Find( typeName );
                T icomponent;
                T* component = &icomponent;
                Engine::CopyMemory( &component->m_TypeName[0], &typeName[0], typeNameByteWidth );
                component->m_TypeName[typeNameByteWidth] = 0;
                component->m_LayoutByteWidth = sizeof(T);
                component->m_RegistryIndex = registryIndex;

                const dvint32 maskIndex = (dvint32)((dvdword)registryIndex >> 5u);
                const dvdword maskBit = 1u << (dvdword)registryIndex & 31u;
                if ((entity->GetData()->m_ComponentBits[ maskIndex ] >> maskBit) & 1u) { return; }
                entity->GetData()->m_ComponentBits[ maskIndex ] |= 1u << maskBit;

                const dvint32 componentStorageMemoryPoolID = m_Instance->m_InputData.m_ComponentStorageMemoryPoolID;
                MemoryPool* componentStorageMemoryPool = &m_Instance->GetData()->m_MemoryPools[componentStorageMemoryPoolID];
                
                Engine::Allocate( componentStorageMemoryPoolID, sizeof(T) );
                void* const lastSubStorageAddress = (void* const)((dvmachword)entity->GetData()->m_SubStorageAddress + (dvmachword)entity->GetData()->m_SubStorageByteWidth);
                void* const moveToAddress = (void* const)((dvmachword)lastSubStorageAddress + sizeof(T));
                const dvusize moveByteWidth = (dvmachword)componentStorageMemoryPool->GetData()->m_AddressOffset - (dvmachword)lastSubStorageAddress;
                Engine::MoveMemory( moveToAddress, lastSubStorageAddress, moveByteWidth );
                Engine::CopyMemory( lastSubStorageAddress, component, sizeof(T) );

                entity->GetData()->m_SubStorageByteWidth += sizeof(T);
                entity->GetData()->m_ComponentCount += 1;
            }

            template<typename T>
            DV_FUNCTION_INLINE T* GetComponent(Entity* entity) {
                void* componentAddress = entity->GetData()->m_SubStorageAddress;
                for (dvisize i = 0; i < (dvisize)entity->GetData()->m_ComponentCount; ++i)
                {
                    IComponent* baseComponent = (IComponent*)componentAddress;
                    dvuchar* typeName = (dvuchar*)typeid(T).name();
                    if (String::CompareCharacters( &baseComponent->m_TypeName[0], &typeName[0] )) {
                        return (T*)baseComponent;
                    }
                    componentAddress = (void*)((dvmachword)componentAddress + (dvmachword)baseComponent->m_LayoutByteWidth);
                }

                return nullptr;
            }

            // Wrapper functions
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
                const char* objectID = (const char*)argumentMemory[ 1 ];
                IData* objectData = (IData*)argumentMemory[ 2 ];
                const dvusize allocationPoolID = (const dvusize)m_RegistryData.m_TypeAllocationPoolID.Find( (dvuchar*)typeid(T).name() );
                MemoryObject* memoryObject = AllocateObject(allocationPoolID, sizeof(T));
                T* typedObj = (T*)memoryObject->Unwrap<T>();
                typedObj->SetSID( &objectID[0] );
                typedObj->SetCreateeAndMemoryObject( (IObject**)result, memoryObject );
                *result = typedObj;
                if (objectData == nullptr) { return; }
                IData* actualObjectData = typedObj->GetData();
                Engine::CopyMemory(actualObjectData, objectData, sizeof(typedObj->m_Data));
            }

        private:
            ENGINE_INPUT_DATA m_InputData;
            ENGINE_REGISTRY_DATA m_RegistryData;
            ENGINE_DATA m_Data;
    };
}

#endif