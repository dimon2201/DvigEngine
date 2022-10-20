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
    class MemoryObject;
    class String;
    class HashMap;
    class Entity;
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
    typedef dvuint8             dvbool;
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
            dvint32 m_MemoryPoolIndex;
    };

    class MemoryObject: public IObject
    {
        public:
            DV_MACRO_FRIENDS(DvigEngine::Engine, DvigEngine::IShell)

            DV_FUNCTION_INLINE void* GetAddress() { return m_Data.m_Address; };
            DV_FUNCTION_INLINE dvusize GetByteWidth() { return m_Data.m_ByteWidth; };
            DV_FUNCTION_INLINE dvint32 GetMemoryPoolIndex() { return m_Data.m_MemoryPoolIndex; };

            template<typename T>
            DV_FUNCTION_INLINE T* Unwrap() {
                return (T*)m_Data.m_Address;
            }

        private:
            DV_XMACRO_GETTER_DATA(MEMORY_OBJECT_DATA)

        private:
            MEMORY_OBJECT_DATA m_Data;
    };

    struct MEMORY_POOL_DATA : IData
    {
        public:
            dvint32 m_Index;
            dvstring m_Label;
            void* m_Address;
            void* m_AddressOffset;
            dvusize m_ByteWidth;
    };

    class MemoryPool : public IObject
    {
        public:
            DV_MACRO_FRIENDS(DvigEngine::Engine, DvigEngine::IShell)

            DV_FUNCTION_INLINE dvint32 GetIndex() { return m_Data.m_Index; };
            DV_FUNCTION_INLINE dvuchar* GetLabel() { return &m_Data.m_Label[0]; };
            DV_FUNCTION_INLINE void* GetAddress() { return m_Data.m_Address; };
            DV_FUNCTION_INLINE void* GetAddressOffset() { return m_Data.m_AddressOffset; };
            DV_FUNCTION_INLINE dvusize GetByteWidth() { return m_Data.m_ByteWidth; };

        private:
            DV_XMACRO_GETTER_DATA(MEMORY_POOL_DATA)

        private:
            MEMORY_POOL_DATA m_Data;
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

            DV_FUNCTION_INLINE dvuchar* GetString() { return &m_Data.m_Chars[0]; };
            DV_FUNCTION_INLINE dvusize GetByteWidth() { return m_Data.m_ByteWidth; };

            static dvusize CharactersCount(dvstring op1);
            static dvresult Compare(STRING_DATA* op1, STRING_DATA* op2);
            static dvresult CompareCharacters(dvstring op1, dvstring op2);
            static MemoryObject* ConcateCharacters(dvstring op1, dvstring op2);

            DV_FUNCTION_INLINE String& operator=(const char* str) { m_Data = STRING_DATA(str); return *this; }
            DV_FUNCTION_INLINE dvuchar* operator()() { return &m_Data.m_Chars[0]; }
            
        private:
            DV_XMACRO_GETTER_DATA(STRING_DATA)

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

            DV_FUNCTION_INLINE dvusize GetEntryCount() { return m_Data.m_EntryCount; };
            DV_FUNCTION_INLINE dvusize GetEntryByteWidth() { return m_Data.m_EntryByteWidth; };
            DV_FUNCTION_INLINE dvusize GetEntryValueByteWidth() { return m_Data.m_EntryValueByteWidth; };
            DV_FUNCTION_INLINE LINKED_LIST_DATA_ENTRY* GetListHead() { return m_Data.m_Head; };

            LINKED_LIST_DATA_ENTRY* MakeEntry(void* const value);

            void Init(const dvusize entryValueByteWidth);
            dvint32 Insert(void* const value);
            void Replace(const dvint32 index, void* const value);
            void* Find(const dvint32 index);

        private:
            DV_XMACRO_GETTER_DATA(LINKED_LIST_DATA)

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
            
            void* m_Key;
            void* m_Value;
    };

    struct HASH_MAP_DATA : IData
    {
        public:
            DV_FUNCTION_INLINE void Init(Engine* engine, HashMap* object) { };

            dvusize m_ListEntryCount;
            LinkedList m_MemoryBlocks;
            dvmachword m_HashTable[DV_MEMORY_COMMON_HASH_MAP_TABLE_BYTE_WIDTH];
    };

    class HashMap : public IObject
    {
        public:
            DV_MACRO_FRIENDS(DvigEngine::Engine, DvigEngine::IShell)

            DV_FUNCTION_INLINE dvusize GetListEntryCount() { return m_Data.m_ListEntryCount; };
            DV_FUNCTION_INLINE LinkedList* GetList() { return &m_Data.m_MemoryBlocks; };
            DV_FUNCTION_INLINE dvmachword* GetHashTable() { return &m_Data.m_HashTable[0]; };
            DV_FUNCTION_INLINE void SetHashTableEntry(dvint32 index, dvmachword value) { m_Data.m_HashTable[index] = value; };

            static dvuint32 Hash(dvstring input);

            void Init();
            void Insert(dvstring key, void* value);
            void* Find(dvstring key);
            void* FindIndex(const dvint32 index);

        private:
            DV_XMACRO_GETTER_DATA(HASH_MAP_DATA)

            MemoryObject* AllocateBlock();
            void InsertToMemoryBlock(dvuint32 hash, dvstring key, void* value);

        private:
            HASH_MAP_DATA m_Data;
    };

    struct IComponent : IData
    {
        public:
            dvstring m_TypeName;
            dvusize m_LayoutByteWidth;
            dvint32 m_RegistryIndex;
    };

    class ISystem
    {
        DV_MACRO_FRIENDS(DvigEngine::Engine, DvigEngine::IShell)
        
        public:
            virtual ~ISystem() {};
            virtual void Update(Engine* engine, Entity* entity) {};
            
            dvstring m_TypeName;
            dvint32 m_RegistryIndex;
    };

    struct ENTITY_DATA : IData
    {
        public:
            DV_FUNCTION_INLINE ENTITY_DATA(Engine* engine, Entity* object) { Init( engine, object ); }

            void Init(Engine* engine, Entity* object);

            void* m_SubStorageAddress;
            dvusize m_SubStorageByteWidth;
            dvusize m_ComponentCount;
            dvuint32 m_ComponentBits[DV_COMPONENT_DWORD_COUNT_PER_COMPONENT_COUNT];
    };

    class Entity : public IObject
    {
        public:
            DV_MACRO_FRIENDS(DvigEngine::Engine, DvigEngine::IShell, DvigEngine::ENTITY_DATA)
        
            DV_FUNCTION_INLINE void* GetSubStorageAddress() { return m_Data.m_SubStorageAddress; };
            DV_FUNCTION_INLINE dvusize GetSubStorageByteWidth() { return m_Data.m_SubStorageByteWidth; };
            DV_FUNCTION_INLINE dvusize GetComponentCount() { return m_Data.m_ComponentCount; };
            DV_FUNCTION_INLINE dvbool GetComponentBit(dvuint32 arrayOffset, dvuint32 bitOffset) { return (m_Data.m_ComponentBits[arrayOffset] >> bitOffset) & 1; };
            DV_FUNCTION_INLINE void SetComponentBit(dvuint32 arrayOffset, dvuint32 bit) { m_Data.m_ComponentBits[arrayOffset] |= bit; };

        private:
            DV_XMACRO_GETTER_DATA(ENTITY_DATA)

        private:
            static dvuint32 m_EntityCount;
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

            DV_FUNCTION_INLINE std::atomic<dvmachword>& GetStopFlag() { return m_Data.m_StopFlag; };
            DV_FUNCTION_INLINE std::atomic<dvmachword>& GetReturnFlag() { return m_Data.m_ReturnFlag; };
            DV_FUNCTION_INLINE std::thread& GetThread() { return m_Data.m_Thread; };
            DV_FUNCTION_INLINE dvusize GetJobCount() { return m_Data.m_JobCount; };
            DV_FUNCTION_INLINE dvmachword GetJobArgument(dvint32 arrayOffset) { return m_Data.m_JobArguments[arrayOffset]; };
            DV_FUNCTION_INLINE dvcallback& GetJob(dvint32 arrayOffset) { return m_Data.m_Jobs[arrayOffset]; };
            
            void Push(dvcallback callback, void* argumentMemory, const dvusize argumentCount);
            void Start();
            void Stop();

        private:
            DV_XMACRO_GETTER_DATA(JOB_QUEUE_DATA)

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
            dvint32 m_StorageMemoryPoolID;
            dvusize m_RequestedThreadCount;
    };

    struct ENGINE_REGISTRY_DATA : IData
    {
        public:
            void* m_StorageAddress;
            dvint32 m_UniqueComponentCount;
            dvint32 m_UniqueSystemCount;
            HashMap m_TypeAllocationPoolID;
            HashMap m_Components;
            HashMap m_Systems;
            HashMap m_Objects;
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
            DV_FUNCTION_INLINE MemoryPool* GetMemoryPools() { return &m_Data.m_MemoryPools[0]; };
            DV_FUNCTION_INLINE dvmachword GetCurrentJobQueueCursor() { return m_Data.m_CurrentJobQueueCursor; };
            DV_FUNCTION_INLINE JobQueue* GetJobQueues() { return &m_Data.m_JobQueues[0]; };
            DV_FUNCTION_INLINE dvusize GetMaxThreadCount() { return m_Data.m_MaxThreadCount; };
            DV_FUNCTION_INLINE dvusize GetRequestedThreadCount() { return m_Data.m_RequestedThreadCount; };
            DV_FUNCTION_INLINE void* GetUserData() { return m_Data.m_UserData; };
            DV_FUNCTION_INLINE void SetUserData(void* address) { m_Data.m_UserData = address; };

            static void Init(ENGINE_INPUT_DATA* engineInputData);
            static void Free();
            static void* Allocate(dvusize memoryPoolID, dvusize byteWidth);
            static MemoryObject* AllocateObject(dvusize memoryPoolID, dvusize byteWidth);
            static void* AllocateUsingData(MEMORY_POOL_DATA* memoryPool, dvusize byteWidth);
            static void DeleteObject(MemoryObject** ppMemoryObject);
            static void CopyMemory(void* destAddress, void* srcAddress, dvusize byteWidth);
            static void MoveMemory(void* destAddress, void* srcAddress, dvusize byteWidth);
            static void SetMemory(void* destAddress, dvmachword value, dvusize byteWidth);

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
                const dvint32 systemIndex = ++m_RegistryData.m_UniqueSystemCount;
                if (m_RegistryData.m_Systems.Find(typeName) != nullptr) { return; }
                T systemOnStack;
                Engine::SetMemory( &systemOnStack.m_TypeName[0], 0, DV_MEMORY_COMMON_STRING_BYTE_WIDTH );
                T* system = Engine::AllocateObject( 0, sizeof(T) )->Unwrap<T>();
                Engine::CopyMemory( system, &systemOnStack, sizeof(T) );
                Engine::CopyMemory( &system->m_TypeName[0], &typeName[0], String::CharactersCount( typeName ) );
                system->m_RegistryIndex = systemIndex;
                m_RegistryData.m_Systems.Insert( typeName, (void*)(dvmachword)system );
            }

            template<typename T>
            void AddComponent(Entity* entity, IComponent* component) {
                DV_ASSERT_PTR(component)

                dvuchar* typeName = (dvuchar*)typeid(T).name();
                const dvusize typeNameByteWidth = String::CharactersCount( typeName );
                const dvint32 registryIndex = (dvint32)(dvmachword)m_RegistryData.m_Components.Find( typeName );
                Engine::CopyMemory( &component->m_TypeName[0], &typeName[0], typeNameByteWidth );
                component->m_TypeName[typeNameByteWidth] = 0;
                component->m_LayoutByteWidth = sizeof(T);
                component->m_RegistryIndex = registryIndex;

                const dvint32 maskIndex = (dvint32)((dvdword)registryIndex >> 5u);
                const dvdword maskBit = 1u << (dvdword)registryIndex & 31u;
                if ((entity->GetData()->m_ComponentBits[ maskIndex ] >> maskBit) & 1u) { return; }
                entity->GetData()->m_ComponentBits[ maskIndex ] |= 1u << maskBit;

                const dvint32 storageMemoryPoolID = m_Instance->m_InputData.m_StorageMemoryPoolID;
                MemoryPool* storageMemoryPool = &m_Instance->GetData()->m_MemoryPools[storageMemoryPoolID];
                
                Engine::AllocateObject( storageMemoryPoolID, sizeof(T) )->Unwrap<void>();
                void* const lastSubStorageAddress = (void* const)((dvmachword)entity->GetData()->m_SubStorageAddress + (dvmachword)entity->GetData()->m_SubStorageByteWidth);
                void* const moveToAddress = (void* const)((dvmachword)lastSubStorageAddress + sizeof(T));
                const dvusize moveByteWidth = (dvmachword)storageMemoryPool->GetData()->m_AddressOffset - (dvmachword)lastSubStorageAddress;
                Engine::MoveMemory( moveToAddress, lastSubStorageAddress, moveByteWidth );
                Engine::CopyMemory( lastSubStorageAddress, component, sizeof(T) );

                entity->GetData()->m_SubStorageByteWidth += sizeof(T);
                entity->GetData()->m_ComponentCount += 1;
            }

            template<typename T>
            DV_FUNCTION_INLINE T* GetComponent(Entity* entity) {
                DV_ASSERT_PTR(entity)

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
                dvuchar* objectID = (dvuchar*)argumentMemory[ 1 ];
                IData* objectData = (IData*)argumentMemory[ 2 ];
                const dvusize allocationPoolID = (const dvusize)m_RegistryData.m_TypeAllocationPoolID.Find( (dvuchar*)typeid(T).name() );
                MemoryObject* memoryObject = AllocateObject(allocationPoolID, sizeof(T));
                T* typedObject = (T*)memoryObject->Unwrap<T>();
                typedObject->SetSID( &objectID[0] );
                typedObject->SetCreateeAndMemoryObject( (IObject**)result, memoryObject );
                *result = typedObject;
                if (objectData == nullptr) {
                    typedObject->m_Data.Init( m_Instance, typedObject );
                    return;
                }
                IData* actualObjectData = typedObject->GetData();
                Engine::CopyMemory(actualObjectData, objectData, sizeof(typedObject->m_Data));
                m_RegistryData.m_Objects.Insert( objectID, typedObject );
            }

        private:
            DV_XMACRO_GETTER_DATA(ENGINE_DATA)

        private:
            ENGINE_INPUT_DATA m_InputData;
            ENGINE_REGISTRY_DATA m_RegistryData;
            ENGINE_DATA m_Data;
    };
}

#endif