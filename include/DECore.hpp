#ifndef _DE_CORE_H_
#define _DE_CORE_H_

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
    class MemoryPool;
    class String;
    class LinkedList;
    class HashMap;
    class DynamicBuffer;
    class ISystem;
    class Entity;
    class JobQueue;
    class Engine;

    /*** Typenames ***/
    typedef int8_t              deint8;
    typedef uint8_t             deuint8;
    typedef int16_t             deint16;
    typedef uint16_t            deuint16;
    typedef int32_t             deint32;
    typedef uint32_t            deuint32;
    typedef int64_t             deint64;
    typedef uint64_t            deuint64;
    #if defined(DV_MACRO_ARCH_32_BIT)
        typedef int32           demachint;
        typedef uint32          demachuint;
        typedef demachuint      demachword;
    #elif defined(DV_MACRO_ARCH_64_BIT)
        typedef deint64         demachint;
        typedef deuint64        demachuint;
        typedef demachuint      demachword;
    #endif
    typedef deuint8             debool;
    typedef deuint8             deuchar;
    typedef demachint           deisize;
    typedef size_t              deusize;
    typedef deuint32            dedword;
    typedef deuint64            deqword;
    typedef demachword          deresult;
    typedef deuchar             destring[DV_MEMORY_COMMON_STRING_BYTE_WIDTH];
    typedef void                (*decallback)(demachword*, deusize);

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
            void Init(Engine* engine, MemoryObject* object);

            void* m_Address;
            deusize m_ByteWidth;
            deint32 m_MemoryPoolIndex;
    };

    class MemoryObject: public IObject
    {
        public:
            DV_MACRO_FRIENDS(DvigEngine::Engine, DvigEngine::IShell)

            DV_FUNCTION_INLINE void* GetAddress() { return m_Data.m_Address; };
            DV_FUNCTION_INLINE deusize GetByteWidth() { return m_Data.m_ByteWidth; };
            DV_FUNCTION_INLINE deint32 GetMemoryPoolIndex() { return m_Data.m_MemoryPoolIndex; };

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
            void Init(Engine* engine, MemoryPool* object);

            deint32 m_Index;
            destring m_Label;
            void* m_Address;
            void* m_AddressOffset;
            deusize m_ByteWidth;
    };

    class MemoryPool : public IObject
    {
        public:
            DV_MACRO_FRIENDS(DvigEngine::Engine, DvigEngine::IShell)

            DV_FUNCTION_INLINE deint32 GetIndex() { return m_Data.m_Index; };
            DV_FUNCTION_INLINE deuchar* GetLabel() { return &m_Data.m_Label[0]; };
            DV_FUNCTION_INLINE void* GetAddress() { return m_Data.m_Address; };
            DV_FUNCTION_INLINE void* GetAddressOffset() { return m_Data.m_AddressOffset; };
            DV_FUNCTION_INLINE deusize GetByteWidth() { return m_Data.m_ByteWidth; };

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

            void Init(Engine* engine, String* object);

            destring m_Chars;
            deusize m_ByteWidth;
    };

    class String : public IObject
    {
        public:
            DV_MACRO_FRIENDS(DvigEngine::Engine, DvigEngine::IShell)

            DV_FUNCTION_INLINE deuchar* GetString() { return &m_Data.m_Chars[0]; };
            DV_FUNCTION_INLINE deusize GetByteWidth() { return m_Data.m_ByteWidth; };

            static deusize CharactersCount(destring op1);
            static deresult Compare(STRING_DATA* op1, STRING_DATA* op2);
            static deresult CompareCharacters(destring op1, destring op2);
            static MemoryObject* ConcateCharacters(destring op1, destring op2);

            DV_FUNCTION_INLINE String& operator=(const char* str) { m_Data = STRING_DATA(str); return *this; }
            DV_FUNCTION_INLINE deuchar* operator()() { return &m_Data.m_Chars[0]; }
            
        private:
            DV_XMACRO_GETTER_DATA(STRING_DATA)

        private:
            STRING_DATA m_Data;
    };

    struct LINKED_LIST_DATA_ENTRY : IData
    {
        public:
            LINKED_LIST_DATA_ENTRY* m_PrevAddress;
            MemoryObject* m_Value;
    };

    struct LINKED_LIST_DATA : IData
    {
        public:
            void Init(Engine* engine, LinkedList* object);

            deusize m_EntryCount;
            deusize m_EntryByteWidth;
            deusize m_EntryValueByteWidth;
            LINKED_LIST_DATA_ENTRY* m_Head;
    };

    class LinkedList : public IObject
    {
        public:
            DV_MACRO_FRIENDS(DvigEngine::Engine, DvigEngine::IShell)

            DV_FUNCTION_INLINE deusize GetEntryCount() { return m_Data.m_EntryCount; };
            DV_FUNCTION_INLINE deusize GetEntryByteWidth() { return m_Data.m_EntryByteWidth; };
            DV_FUNCTION_INLINE deusize GetEntryValueByteWidth() { return m_Data.m_EntryValueByteWidth; };
            DV_FUNCTION_INLINE LINKED_LIST_DATA_ENTRY* GetListHead() { return m_Data.m_Head; };

            LINKED_LIST_DATA_ENTRY* MakeEntry(void* const value);

            void Init(const deusize entryValueByteWidth);
            deint32 Insert(void* const value);
            void Replace(const deint32 index, void* const value);
            void* Find(const deint32 index);

        private:
            DV_XMACRO_GETTER_DATA(LINKED_LIST_DATA)

        private:
            LINKED_LIST_DATA m_Data;
    };

    struct HASH_MAP_MEMORY_BLOCK : IData
    {
        demachword m_EntryCount;
        deuchar m_Memory[ DV_MAX_HASH_MAP_MEMORY_BLOCK_BYTE_WIDTH ];
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
            void Init(Engine* engine, HashMap* object);

            deusize m_ListEntryCount;
            LinkedList m_MemoryBlocks;
            demachword m_HashTable[DV_MEMORY_COMMON_HASH_MAP_TABLE_BYTE_WIDTH];
    };

    class HashMap : public IObject
    {
        public:
            DV_MACRO_FRIENDS(DvigEngine::Engine, DvigEngine::IShell)

            DV_FUNCTION_INLINE deusize GetListEntryCount() { return m_Data.m_ListEntryCount; };
            DV_FUNCTION_INLINE LinkedList* GetList() { return &m_Data.m_MemoryBlocks; };
            DV_FUNCTION_INLINE demachword* GetHashTable() { return &m_Data.m_HashTable[0]; };
            DV_FUNCTION_INLINE void SetHashTableEntry(deint32 index, demachword value) { m_Data.m_HashTable[index] = value; };

            static deuint32 Hash(destring input);

            void Init();
            void Insert(destring key, void* value);
            void* Find(destring key);
            void* FindIndex(const deint32 index);

        private:
            DV_XMACRO_GETTER_DATA(HASH_MAP_DATA)

            MemoryObject* AllocateBlock();
            void InsertToMemoryBlock(deuint32 hash, destring key, void* value);

        private:
            HASH_MAP_DATA m_Data;
    };

    struct DYNAMIC_BUFFER_DATA : IData
    {
        public:
            void Init(const Engine* const engine, DynamicBuffer* const object);

            void* m_Address;
            void* m_AddressOffset;
            deusize m_ByteWidth;
            deusize m_AheadByteWidth;
            deusize m_AllocatedByteWidth;
            deint32 m_MemoryPoolIndex;
    };

    class DynamicBuffer : public IObject
    {
        public:
            DV_MACRO_FRIENDS(DvigEngine::Engine, DvigEngine::IShell)

            DV_FUNCTION_INLINE void* GetAddress() { return m_Data.m_Address; };
            DV_FUNCTION_INLINE void* GetAddressOffset() { return m_Data.m_AddressOffset; };
            DV_FUNCTION_INLINE deusize GetSize() { return m_Data.m_ByteWidth; };
            DV_FUNCTION_INLINE deusize GetAheadSize() { return m_Data.m_AheadByteWidth; };
            DV_FUNCTION_INLINE deusize GetAllocatedSize() { return m_Data.m_AllocatedByteWidth; };
            DV_FUNCTION_INLINE deint32 GetMemoryPoolIndex() { return m_Data.m_MemoryPoolIndex; };
            
            void CopyToBuffer(const void* const dataAddress, const deusize byteWidth);
            void RequestFromBuffer(const deusize requestOffset, void* const destAddress, const deusize byteWidth);

        private:
            DV_XMACRO_GETTER_DATA(DYNAMIC_BUFFER_DATA)

        private:
            DYNAMIC_BUFFER_DATA m_Data;
    };

    struct IComponent : IData
    {
        public:
            destring m_TypeName;
            deusize m_LayoutByteWidth;
            deint32 m_RegistryIndex;
    };

    class ISystem
    {
        DV_MACRO_FRIENDS(DvigEngine::Engine, DvigEngine::IShell)
        
        public:
            virtual ~ISystem() {};
            virtual void Update(Engine* engine, Entity* entity) {};
            
            destring m_TypeName;
            deint32 m_RegistryIndex;
    };

    struct ENTITY_DATA : IData
    {
        public:
            void Init(Engine* engine, Entity* object);

            void* m_SubStorageAddress;
            deusize m_SubStorageByteWidth;
            deusize m_ComponentCount;
            deuint32 m_ComponentBits[DV_COMPONENT_DWORD_COUNT_PER_COMPONENT_COUNT];
    };

    class Entity : public IObject
    {
        public:
            DV_MACRO_FRIENDS(DvigEngine::Engine, DvigEngine::IShell, DvigEngine::ENTITY_DATA)
        
            DV_FUNCTION_INLINE void* GetSubStorageAddress() { return m_Data.m_SubStorageAddress; };
            DV_FUNCTION_INLINE deusize GetSubStorageByteWidth() { return m_Data.m_SubStorageByteWidth; };
            DV_FUNCTION_INLINE deusize GetComponentCount() { return m_Data.m_ComponentCount; };
            DV_FUNCTION_INLINE debool GetComponentBit(deuint32 arrayOffset, deuint32 bitOffset) { return (m_Data.m_ComponentBits[arrayOffset] >> bitOffset) & 1; };
            DV_FUNCTION_INLINE void SetComponentBit(deuint32 arrayOffset, deuint32 bit) { m_Data.m_ComponentBits[arrayOffset] |= bit; };

        private:
            DV_XMACRO_GETTER_DATA(ENTITY_DATA)

        private:
            static deuint32 m_EntityCount;
            ENTITY_DATA m_Data;
    };

    struct JOB_QUEUE_DATA : IData
    {
        std::atomic<demachword> m_StopFlag;
        std::atomic<demachword> m_ReturnFlag;
        std::thread m_Thread;
        dedword m_JobCount;
        demachword m_JobArguments[DV_MAX_JOB_QUEUE_THREAD_JOB_ARGUMENT_COUNT * DV_MAX_JOB_QUEUE_THREAD_JOB_COUNT];
        decallback m_Jobs[DV_MAX_JOB_QUEUE_THREAD_JOB_COUNT];
    };
    
    class JobQueue : public IObject
    {
        public:
            DV_MACRO_FRIENDS(DvigEngine::Engine, DvigEngine::IShell)

            DV_FUNCTION_INLINE std::atomic<demachword>& GetStopFlag() { return m_Data.m_StopFlag; };
            DV_FUNCTION_INLINE std::atomic<demachword>& GetReturnFlag() { return m_Data.m_ReturnFlag; };
            DV_FUNCTION_INLINE std::thread& GetThread() { return m_Data.m_Thread; };
            DV_FUNCTION_INLINE deusize GetJobCount() { return m_Data.m_JobCount; };
            DV_FUNCTION_INLINE demachword GetJobArgument(deint32 arrayOffset) { return m_Data.m_JobArguments[arrayOffset]; };
            DV_FUNCTION_INLINE decallback& GetJob(deint32 arrayOffset) { return m_Data.m_Jobs[arrayOffset]; };
            
            void Push(decallback callback, void* argumentMemory, const deusize argumentCount);
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
            dedword m_Version;
            deuint32 m_MemoryPoolsCount;
            MEMORY_POOL_DATA* m_MemoryPoolsData;
            deint32 m_SystemMemoryPoolID;
            deint32 m_StorageMemoryPoolID;
            deusize m_RequestedThreadCount;
    };

    struct ENGINE_REGISTRY_DATA : IData
    {
        public:
            void* m_StorageAddress;
            deint32 m_UniqueComponentCount;
            deint32 m_UniqueSystemCount;
            HashMap m_TypeAllocationPoolID;
            HashMap m_Components;
            HashMap m_Systems;
            HashMap m_Objects;
    };

    struct ENGINE_DATA : IData
    {
        public:
            MemoryPool* m_MemoryPools;
            demachword m_CurrentJobQueueCursor;
            JobQueue* m_JobQueues;
            deusize m_MaxThreadCount;
            deusize m_RequestedThreadCount;
            void* m_UserData;
    };

    /*** Engine class declaration ***/
    class Engine
    {
        DV_MACRO_DECLARE_SINGLETON(Engine, public)

        public:
            DV_FUNCTION_INLINE MemoryPool* GetMemoryPools() { return &m_Data.m_MemoryPools[0]; };
            DV_FUNCTION_INLINE demachword GetCurrentJobQueueCursor() { return m_Data.m_CurrentJobQueueCursor; };
            DV_FUNCTION_INLINE JobQueue* GetJobQueues() { return &m_Data.m_JobQueues[0]; };
            DV_FUNCTION_INLINE deusize GetMaxThreadCount() { return m_Data.m_MaxThreadCount; };
            DV_FUNCTION_INLINE deusize GetRequestedThreadCount() { return m_Data.m_RequestedThreadCount; };
            DV_FUNCTION_INLINE void* GetUserData() { return m_Data.m_UserData; };
            DV_FUNCTION_INLINE void SetUserData(void* address) { m_Data.m_UserData = address; };

            static void Init(ENGINE_INPUT_DATA* engineInputData);
            static void Free();
            static void* Allocate(deusize memoryPoolID, deusize byteWidth);
            static MemoryObject* AllocateObject(deusize memoryPoolID, deusize byteWidth);
            static void* AllocateUsingData(MEMORY_POOL_DATA* memoryPool, deusize byteWidth);
            static void DeleteObject(MemoryObject** ppMemoryObject);
            static void CopyMemory(void* destAddress, const void* srcAddress, const deusize byteWidth);
            static void MoveMemory(void* destAddress, const void* srcAddress, const deusize byteWidth);
            static void SetMemory(void* destAddress, const demachword value, const deusize byteWidth);

            DV_FUNCTION_INLINE MemoryPool* GetMemoryPoolByID(deusize memoryPoolID) {
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
                deuchar* typeName = (deuchar*)typeid(T).name();
                const deint32 componentIndex = ++m_RegistryData.m_UniqueComponentCount;
                if (m_RegistryData.m_Components.Find(typeName) != nullptr) { return; }
                m_RegistryData.m_Components.Insert(typeName, (void*)(demachword)componentIndex);
            }

            template<typename T>
            void RegisterSystem() {
                deuchar* typeName = (deuchar*)typeid(T).name();
                const deint32 systemIndex = ++m_RegistryData.m_UniqueSystemCount;
                if (m_RegistryData.m_Systems.Find(typeName) != nullptr) { return; }
                T systemOnStack;
                Engine::SetMemory( &systemOnStack.m_TypeName[0], 0, DV_MEMORY_COMMON_STRING_BYTE_WIDTH );
                T* system = Engine::AllocateObject( 0, sizeof(T) )->Unwrap<T>();
                Engine::CopyMemory( system, &systemOnStack, sizeof(T) );
                Engine::CopyMemory( &system->m_TypeName[0], &typeName[0], String::CharactersCount( typeName ) );
                system->m_RegistryIndex = systemIndex;
                m_RegistryData.m_Systems.Insert( typeName, (void*)(demachword)system );
            }

            template<typename T>
            void AddComponent(Entity* entity, IComponent* component) {
                DV_ASSERT_PTR(component)

                deuchar* typeName = (deuchar*)typeid(T).name();
                const deusize typeNameByteWidth = String::CharactersCount( typeName );
                const deint32 registryIndex = (deint32)(demachword)m_RegistryData.m_Components.Find( typeName );
                Engine::CopyMemory( &component->m_TypeName[0], &typeName[0], typeNameByteWidth );
                component->m_TypeName[typeNameByteWidth] = 0;
                component->m_LayoutByteWidth = sizeof(T);
                component->m_RegistryIndex = registryIndex;

                const deint32 maskIndex = (deint32)((dedword)registryIndex >> 5u);
                const dedword maskBit = 1u << (dedword)registryIndex & 31u;
                if ((entity->GetData()->m_ComponentBits[ maskIndex ] >> maskBit) & 1u) { return; }
                entity->GetData()->m_ComponentBits[ maskIndex ] |= 1u << maskBit;

                const deint32 storageMemoryPoolID = m_Instance->m_InputData.m_StorageMemoryPoolID;
                MemoryPool* storageMemoryPool = &m_Instance->GetData()->m_MemoryPools[storageMemoryPoolID];
                
                Engine::AllocateObject( storageMemoryPoolID, sizeof(T) )->Unwrap<void>();
                void* const lastSubStorageAddress = (void* const)((demachword)entity->GetData()->m_SubStorageAddress + (demachword)entity->GetData()->m_SubStorageByteWidth);
                void* const moveToAddress = (void* const)((demachword)lastSubStorageAddress + sizeof(T));
                const deusize moveByteWidth = (demachword)storageMemoryPool->GetData()->m_AddressOffset - (demachword)lastSubStorageAddress;
                Engine::MoveMemory( moveToAddress, lastSubStorageAddress, moveByteWidth );
                Engine::CopyMemory( lastSubStorageAddress, component, sizeof(T) );

                entity->GetData()->m_SubStorageByteWidth += sizeof(T);
                entity->GetData()->m_ComponentCount += 1;
            }

            template<typename T>
            DV_FUNCTION_INLINE T* GetComponent(Entity* entity) {
                DV_ASSERT_PTR(entity)

                void* componentAddress = entity->GetData()->m_SubStorageAddress;
                for (deisize i = 0; i < (deisize)entity->GetData()->m_ComponentCount; ++i)
                {
                    IComponent* baseComponent = (IComponent*)componentAddress;
                    deuchar* typeName = (deuchar*)typeid(T).name();
                    if (String::CompareCharacters( &baseComponent->m_TypeName[0], &typeName[0] )) {
                        return (T*)baseComponent;
                    }
                    componentAddress = (void*)((demachword)componentAddress + (demachword)baseComponent->m_LayoutByteWidth);
                }

                return nullptr;
            }

            // Wrapper functions
            template<typename T>
            DV_FUNCTION_INLINE void Create(const void** result, const char* stringID, IData* data) {
                // DV_XMACRO_PUSH_JOB(CallCreate<T>, m_Instance, result, stringID, data)
                demachword argumentMemory[3] = { (demachword)result, (demachword)stringID, (demachword)data };
                CallCreate<T>(&argumentMemory[0], 0);
            }

        private:
            // Job functions
            template<typename T>
            DV_FUNCTION_INLINE void CallCreate(demachword* argumentMemory, deusize jobIndex) {
                const T** result = (const T**)argumentMemory[ 0 ];
                deuchar* objectID = (deuchar*)argumentMemory[ 1 ];
                IData* objectData = (IData*)argumentMemory[ 2 ];
                const deusize allocationPoolID = (const deusize)m_RegistryData.m_TypeAllocationPoolID.Find( (deuchar*)typeid(T).name() );
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