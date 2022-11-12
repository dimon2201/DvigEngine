#ifndef _DE_LIB_CORE_H_
#define _DE_LIB_CORE_H_

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

namespace DvigEngine2
{
    class MemoryObject;
    class DynamicBuffer;
    class FixedSet;
    class HashMap;
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
    typedef void                (*depjob)(demachword*, deint32);

    template <typename T>
    class Ptr
    {
        public:
            DV_FUNCTION_INLINE static T Add(T* ptr, const deusize value) { return (T)((demachword)*ptr + (demachword)value); }
            DV_FUNCTION_INLINE static T Subtract(T* ptr, const deusize value) { return (T)((demachword)*ptr - (demachword)value); }
    };

    class ICommon
    {
        DV_MACRO_FRIENDS(Engine)

        public:
            virtual ~ICommon() {}
            virtual void Free() = 0;

            DV_FUNCTION_INLINE deuchar* GetUSID() { return &m_USID[0]; }
            DV_FUNCTION_INLINE demachint GetUIID() { return m_UIID; }
            DV_FUNCTION_INLINE MemoryObject* GetMemoryObject() { return m_MemoryObject; }
            DV_FUNCTION_INLINE Engine* GetEngine() { return m_Engine; }

        private:
            void SetUSIDAndUIIDAndMemoryObjectAndEngine(deuchar* USID, const demachint UIID, MemoryObject* memoryObject, Engine* engine);

        private:
            destring m_USID;
            demachint m_UIID;
            MemoryObject* m_MemoryObject;
            Engine* m_Engine;
    };

    class IProperty
    { };

    class ILayout : public ICommon
    {
        public:
            deusize m_LayoutByteWidth;
    };

    class IComponent : public ILayout
    { };

    class ISystem
    { };

    class IHelperObject : public ILayout
    {
        DV_MACRO_FRIENDS(Engine)
    };

    class INode : public ICommon
    {
        DV_MACRO_FRIENDS(Engine)

        public:
            DV_FUNCTION_INLINE static INode* GetRootNode() { return m_RootNode; }
            INode* GetChildNode(const char* USID);
            IComponent* GetComponent(const char* USID);
            IHelperObject* GetHelperObject(const char* USID);

            void Init();
            void Free() override final;
            void AddChildNode(INode* const node);
            void AddHelperObject(IHelperObject* const helperObject);
            void RemoveChildNode(const char* USID);
            void RemoveComponent(const char* USID);
            void RemoveHelperObject(const char* USID);

        private:
            static INode* m_RootNode;
            INode* m_ParentNode;
            DynamicBuffer* m_ChildNodes;
            DynamicBuffer* m_Components;
            DynamicBuffer* m_HelperObjects;
            dedword m_ComponentBitSet[DV_COMPONENT_DWORD_COUNT_PER_COMPONENT_COUNT];
    };

    class MemoryObjectProperty : public IProperty
    {
        public:
            void* m_Address;
            deusize m_ByteWidth;
            deint32 m_MemoryPoolIndex;
    };
    
    class MemoryObject : public IHelperObject
    {
        DV_MACRO_FRIENDS(Engine)

        public:
            void Init() {}
            void Free() override final {}

            DV_FUNCTION_INLINE void* GetAddress() { return m_Address; }
            DV_FUNCTION_INLINE deusize GetByteWidth() { return m_ByteWidth; }
            DV_FUNCTION_INLINE deint32 GetMemoryPoolIndex() { return m_MemoryPoolIndex; }

            template<typename T>
            DV_FUNCTION_INLINE T Unwrap() { return (T)m_Address; }

        // private:
        //     DV_XMACRO_GETTER_PROPERTY(MemoryObjectProperty)

        private:
            // MemoryObjectProperty m_Prop;
            void* m_Address;
            deusize m_ByteWidth;
            demachword m_FreeFlag;
            deint32 m_MemoryPoolIndex;
    };

    class StringProperty : public IProperty
    {
        public:
            destring m_Chars;
            deusize m_ByteWidth;
    };

    class String : public IHelperObject
    {
        DV_MACRO_FRIENDS(DvigEngine2::Engine)

        public:
            static deusize CharactersCount(const destring op1);
            static deresult CompareCharacters(const char* op1, const char* op2, const deusize opByteWidth);
            static deresult CompareCharacterStrings(const destring op1, const destring op2);
            static MemoryObject* ConcateCharacters(destring op1, destring op2);

            void Init() {}
            void Free() override final {}

            DV_FUNCTION_INLINE deuchar* GetString() { return &m_Chars[0]; };
            DV_FUNCTION_INLINE deusize GetByteWidth() { return m_ByteWidth; };
            
        // private:
        //     DV_XMACRO_GETTER_DATA(StringProperty)

        private:
            // StringProperty m_Data;
            destring m_Chars;
            deusize m_ByteWidth;
    };

    class DynamicBufferProperty : public IProperty
    {
        public:
            deusize m_Capacity;
            MemoryObject* m_DataObject;
            deusize m_AllocatedDataByteWidth;
            deusize m_DataByteWidth;
            deint32 m_MemoryPoolIndex;
    };

    class DynamicBuffer : public IHelperObject
    {
        DV_MACRO_FRIENDS(DvigEngine2::Engine)

        public:
            DV_FUNCTION_INLINE deusize GetCapacity() { return m_Capacity; }
            DV_FUNCTION_INLINE deusize GetSize() { return m_DataByteWidth; }
            DV_FUNCTION_INLINE void* GetDataAddress() { return m_DataObject->Unwrap<void*>(); }

            void Init(const deint32 memoryPoolIndex, const deusize bufferByteWidth);
            void Free() override final;
            deint32 Insert(const deisize offset, const void* data, const deusize dataByteWidth);
            void Find(const deisize offset, void* output, const deusize copyByteWidth);
            void Remove(const deisize offset, const deusize removeByteWidth);
            void Clear();

        // private:
        //     DV_XMACRO_GETTER_PROPERTY(DynamicBufferProperty)

        private:
            // DynamicBufferProperty m_Prop;
            deusize m_Capacity;
            MemoryObject* m_DataObject;
            deusize m_AllocatedDataByteWidth;
            deusize m_DataByteWidth;
            deint32 m_MemoryPoolIndex;
    };

    class FixedSetProperty : public IProperty
    {
        public:
            deusize m_Capacity;
            deusize m_EntryByteWidth;
            MemoryObject* m_DataObject;
            deusize m_ReservedDataByteWidth;
            deusize m_AllocatedDataByteWidth;
            deusize m_DataByteWidth;
            deint32 m_MemoryPoolIndex;
    };

    class FixedSet : public IHelperObject
    {
        DV_MACRO_FRIENDS(DvigEngine2::Engine)

        public:
            DV_FUNCTION_INLINE deusize GetCapacity() { return m_Capacity; }
            DV_FUNCTION_INLINE deusize GetEntryByteWidth() { return m_EntryByteWidth; }
            DV_FUNCTION_INLINE deusize GetSize() { return m_DataByteWidth; }

            void Init(const deint32 memoryPoolIndex, const deusize reservedCapacity, const deusize entryByteWidth);
            void Free() override final;
            deint32 Insert(void* entry);
            void Replace(const deint32 index, void* entry);
            void Remove(const deint32 index);
            template <typename T>
            T Find(const deint32 index)
            {
                if (index >= (deint32)this->m_Capacity) { return (T)nullptr; }
                void* dataAddress = this->m_DataObject->Unwrap<void*>();
                void* entryAddress = DvigEngine2::Ptr<void*>::Add( &dataAddress, index * this->m_EntryByteWidth );
                return (T)entryAddress;
            }
            deint32 FindValue(void* entry);

        // private:
        //     DV_XMACRO_GETTER_PROPERTY(FixedSetProperty)

        private:
            // FixedSetProperty m_Prop;
            deusize m_Capacity;
            deusize m_EntryByteWidth;
            MemoryObject* m_DataObject;
            deusize m_ReservedDataByteWidth;
            deusize m_AllocatedDataByteWidth;
            deusize m_DataByteWidth;
            deint32 m_MemoryPoolIndex;
    };

    class HashMapKeyValuePair
    {
        public:
            destring m_Key;
            void* m_Value;
    };

    class HashMapProperty : public IProperty
    {
        public:
            deusize m_Capacity;
            deusize m_EntryValueByteWidth;
            FixedSet m_Entries;
            deusize m_HashTableSize;
            demachword* m_HashTable;
            deint32 m_MemoryPoolIndex;
    };

    class HashMap : public IHelperObject
    {
        DV_MACRO_FRIENDS(DvigEngine2::Engine)

        public:
            static deuint32 Hash(const destring input, const demachword bitMask);
            static deuint32 HashMurMur(const destring input, const demachword bitMask);

            DV_FUNCTION_INLINE deusize GetCapacity() { return m_Capacity; }
            DV_FUNCTION_INLINE deusize GetEntryValueByteWidth() { return m_EntryValueByteWidth; }
            DV_FUNCTION_INLINE deusize GetHashTableSize() { return m_HashTableSize; }
            DV_FUNCTION_INLINE demachword* GetHashTable() { return &m_HashTable[0]; }
            DV_FUNCTION_INLINE MemoryObject* GetHashTableMemoryObject() { return Ptr<MemoryObject*>::Subtract( (MemoryObject**)&m_HashTable, sizeof(DvigEngine2::MemoryObject) ); }

            void Init(const deint32 memoryPoolIndex, const deusize reservedCapacity, const deusize entryValueByteWidth, const deusize hashTableSize);
            void Free() override final;
            deint32 Insert(const char* key, void* value);
            void* Find(const char* key);
            void Remove(const char* key);

        // private:
        //     DV_XMACRO_GETTER_PROPERTY(HashMapProperty)

        private:
            // HashMapProperty m_Prop;
            deusize m_Capacity;
            deusize m_EntryValueByteWidth;
            FixedSet m_Entries;
            deusize m_HashTableSize;
            demachword* m_HashTable;
            deint32 m_MemoryPoolIndex;
    };

    class MemoryPoolProperty : public IProperty
    {
        public:
            deint32 m_Index;
            destring m_Label;
            void* m_Address;
            void* m_AddressOffset;
            deusize m_ByteWidth;
    };

    class MemoryPool : public IHelperObject
    {
        DV_MACRO_FRIENDS(DvigEngine2::Engine)

        public:
            DV_FUNCTION_INLINE deuchar* GetLabel() { return &m_Label[0]; }
            DV_FUNCTION_INLINE void* GetBaseAddress() { return m_Address; }
            DV_FUNCTION_INLINE void* GetOffsetAddress() { return m_AddressOffset; }
            DV_FUNCTION_INLINE deusize GetByteWidth() { return m_ByteWidth; }            

        // private: 
            // DV_XMACRO_GETTER_PROPERTY(MemoryPoolProperty)

        private:
            // MemoryPoolProperty m_Prop;
            deint32 m_Index;
            destring m_Label;
            void* m_Address;
            void* m_AddressOffset;
            deusize m_ByteWidth;
            deusize m_OccupiedByteWidth;
    };

    class JobQueueProperty : public IProperty
    {
        public:
            std::atomic<demachword> m_StopFlag;
            std::atomic<demachword> m_ReturnFlag;
            std::thread m_Thread;
            dedword m_JobCount;
            demachword m_JobArguments[DV_MAX_JOB_QUEUE_THREAD_JOB_ARGUMENT_COUNT * DV_MAX_JOB_QUEUE_THREAD_JOB_COUNT];
            depjob m_Jobs[DV_MAX_JOB_QUEUE_THREAD_JOB_COUNT];
    };
    
    class JobQueue : public IHelperObject
    {
        DV_MACRO_FRIENDS(DvigEngine2::Engine)

        public:
            DV_FUNCTION_INLINE std::atomic<demachword>& GetStopFlag() { return m_StopFlag; }
            DV_FUNCTION_INLINE std::atomic<demachword>& GetReturnFlag() { return m_ReturnFlag; }
            DV_FUNCTION_INLINE std::thread& GetThread() { return m_Thread; }
            DV_FUNCTION_INLINE deusize GetJobCount() { return m_JobCount; }
            DV_FUNCTION_INLINE demachword GetJobArgument(deint32 arrayOffset) { return m_JobArguments[arrayOffset]; }
            DV_FUNCTION_INLINE depjob& GetJob(deint32 arrayOffset) { return m_Jobs[arrayOffset]; }
            
            virtual void Delete();
            void Push(depjob callback, void* argumentMemory, const deusize argumentCount);
            void Start();
            void Stop();

        // private:
        //     DV_XMACRO_GETTER_PROPERTY(JobQueueProperty)

        private:
            // JobQueueProperty m_Prop;
            std::atomic<demachword> m_StopFlag;
            std::atomic<demachword> m_ReturnFlag;
            std::thread m_Thread;
            dedword m_JobCount;
            demachword m_JobArguments[DV_MAX_JOB_QUEUE_THREAD_JOB_ARGUMENT_COUNT * DV_MAX_JOB_QUEUE_THREAD_JOB_COUNT];
            depjob m_Jobs[DV_MAX_JOB_QUEUE_THREAD_JOB_COUNT];
    };

    class ThreadPoolJobData
    {
        public:
            demachword m_Arguments[ DV_MAX_JOB_QUEUE_THREAD_JOB_ARGUMENT_COUNT ];
            depjob m_pJob;
    };

    class ThreadPoolThreadData
    {
        public:
            std::thread m_Thread;
            deusize m_JobCount;
            ThreadPoolJobData m_Jobs[ DV_MAX_JOB_QUEUE_THREAD_JOB_COUNT ];
    };

    class ThreadPoolSystem : public ISystem
    {
        DV_MACRO_FRIENDS(DvigEngine2::Engine)
        DV_XMACRO_DECLARE_STATIC_CLASS(ThreadPoolSystem)

        public:
            static void Init();
            static void AddJob(deint32 threadIndex, depjob callback, void* arguments, const deusize argumentCount);
            static void DoJobs(demachword* arguments, deint32 threadIndex);
            static void WaitForJobs();

        private:
            static debool m_IsRunning;
            static deint32 m_ThreadCursor;
            static ThreadPoolThreadData m_ThreadQueueData[ DV_MAX_JOB_QUEUE_THREAD_COUNT ];
    };

    class EngineRegistryProperty : public IProperty
    {
        public:
            HashMap* m_RegisteredComponents;
            HashMap* m_Instances;
            HashMap* m_AllocPoolIndexMap;
    };

    class EngineInputProperty : public IProperty
    {
        public:
            dedword m_Version;
            deuint32 m_MemoryPoolsCount;
            MemoryPoolProperty* m_MemoryPoolsData;
            deint32 m_SystemMemoryPoolIndex;
            deint32 m_ComponentStorageMemoryPoolIndex;
            deusize m_RequestedThreadCount;
    };

    class EngineProperty : public IProperty
    {
        public:
            dedword m_Version;
            deuint32 m_MemoryPoolsCount;
            MemoryPoolProperty* m_MemoryPoolsData;
            deint32 m_SystemMemoryPoolIndex;
            deint32 m_ComponentStorageMemoryPoolIndex;
            deusize m_MaxThreadCount;
            deusize m_RequestedThreadCount;
            MemoryPool* m_MemoryPools;
            demachword m_CurrentJobQueueCursor;
            JobQueue* m_JobQueues;
            void* m_UserData;
    };

    class Engine
    {
        public:
            Engine(EngineInputProperty* engineInputProperty);
            void Free();
            static MemoryObject* Allocate(deint32 memoryPoolIndex, deusize byteWidth);
            static void CopyMemory(void* destAddress, const void* srcAddress, const deusize byteWidth);
            static void MoveMemory(void* destAddress, const void* srcAddress, const deusize byteWidth);
            static void SetMemory(void* destAddress, const demachword value, const deusize byteWidth);
            static Engine* GetClassInstance() { return m_EngineInstance; }
            void Delete(MemoryObject* memoryObject);

            DV_FUNCTION_INLINE EngineRegistryProperty* GetRegistryData() { return &m_RegistryProp; }
            DV_FUNCTION_INLINE MemoryPool* GetMemoryPoolByIndex(const deint32 memoryPoolIndex) { return &(this->m_Prop.m_MemoryPools[memoryPoolIndex]); }
            DV_FUNCTION_INLINE void* GetUserData() { return m_Prop.m_UserData; }
            DV_FUNCTION_INLINE ICommon* GetExistingInstance(const char* USID) { ICommon* instance = (ICommon*)m_RegistryProp.m_Instances->Find( &USID[0] ); return instance; }
            static DV_FUNCTION_INLINE demachint GetGlobalUIID() { static demachint globalUIID = 0; return globalUIID++; }

            template <typename T>
            DV_FUNCTION_INLINE void RegisterComponent()
            {
                static demachint globalComponentIndex = 0;
                const char* typeName = typeid(T).name();
                if (m_RegistryProp.m_RegisteredComponents->Find( typeName ) == nullptr) {
                    m_RegistryProp.m_RegisteredComponents->Insert( typeName, (void*)++globalComponentIndex );
                }
            }

            template <typename T>
            void AddComponent(INode** const node, IComponent* const component)
            {
                const char* typeName = typeid(T).name();
                const deint32 registryIndex = (const deint32)(demachword)m_RegistryProp.m_RegisteredComponents->Find( &typeName[0] );
                const deint32 bitSetIndex = (const deint32)((dedword)registryIndex >> 5u);
                const dedword bitSetBit = 1u << ((dedword)registryIndex & 31u);
                if (registryIndex == 0 || (((*node)->m_ComponentBitSet[bitSetIndex] >> bitSetBit) & 1u) == 1u) { return; }
                (*node)->m_ComponentBitSet[bitSetIndex] |= 1u << bitSetBit;

                // (*node)->m_Components->Insert( DV_NULL, component, component->m_LayoutByteWidth );
                // *component->GetCreatee() = (*node)->GetComponent( (const char*)component->GetUSID() );
                (*node)->m_Components->Insert( DV_NULL, &component, sizeof(void*) );
            }

            void AddHelperObject(INode** const node, IHelperObject* const helperObject)
            {
                IHelperObject* getHelperObject = (*node)->GetHelperObject( (const char*)helperObject->GetUSID() );
                if (getHelperObject != nullptr) { return; }

                // (*node)->m_HelperObjects->Insert( DV_NULL, helperObject, helperObject->m_LayoutByteWidth );
                // *helperObject->GetCreatee() = (*node)->GetHelperObject( (const char*)helperObject->GetUSID() );
            }

            template <typename T>
            void RemoveComponent(INode** const node, const char* USID)
            {
                const char* typeName = typeid(T).name();
                const deint32 registryIndex = (const deint32)(demachword)m_RegistryProp.m_RegisteredComponents->Find( typeName );
                const deint32 bitSetIndex = (const deint32)((dedword)registryIndex >> 5u);
                const dedword bitSetBit = 1u << ((dedword)registryIndex & 31u);
                if((((*node)->m_ComponentBitSet[bitSetIndex] >> bitSetBit) & 1u) == 0u) { return; }

                // Set bitset bit to zero
                (*node)->m_ComponentBitSet[bitSetIndex] ^= 1u << bitSetBit;

                // Remove component from memory
                IComponent* dataAddress = (IComponent*)(*node)->m_Components->GetDataAddress();
                IComponent* removedComponent = (*node)->GetComponent( &USID[0] );
                
                const deusize offset = (demachword)removedComponent - (demachword)dataAddress;
                (*node)->m_Components->Remove( offset, removedComponent->m_LayoutByteWidth );
            }
            
            template <typename T>
            DV_FUNCTION_INLINE T* Create(const char* USID)
            {
                // DV_XMACRO_PUSH_JOB(CallCreate<T>, m_Instance, result, stringID, data)
                
                demachword argumentMemory[1] = { (demachword)&USID[0] };

                return CallCreate<T>(&argumentMemory[0], 0);
            }

        public:
            DV_XMACRO_GETTER_PROPERTY(EngineProperty)

        private:
            template <typename T>
            T* CallCreate(demachword* argumentMemory, deint32 jobIndex)
            {
                deuchar* objectUSID = (deuchar*)argumentMemory[ 0 ];
                void* pAllocationPoolIndex = m_RegistryProp.m_AllocPoolIndexMap->Find( typeid(T).name() );
                deuint32 allocationPoolIndex = (deuint32)(demachword)pAllocationPoolIndex;
                if (pAllocationPoolIndex == nullptr) { allocationPoolIndex = 0; }
                MemoryObject* memoryObject = Engine::Allocate(allocationPoolIndex, sizeof(T));
                T typedObjectOnStack;
                T* typedObject = memoryObject->Unwrap<T*>();
                Engine::CopyMemory( typedObject, &typedObjectOnStack, sizeof(demachword) ); // copy vpointer
                typedObject->SetUSIDAndUIIDAndMemoryObjectAndEngine( objectUSID, Engine::GetGlobalUIID(), memoryObject, this );
                if (dynamic_cast<INode*>(typedObject) != nullptr)
                {
                    INode* node = (INode*)typedObject;
                    node->Init();
                }
                else if (dynamic_cast<ILayout*>(typedObject) != nullptr)
                {
                    ILayout* layout = (ILayout*)typedObject;
                    layout->m_LayoutByteWidth = sizeof(T);
                }
                this->m_RegistryProp.m_Instances->Insert( (const char*)&objectUSID[0], (void*)typedObject );
                return typedObject;
            }

        public:
            static Engine* m_EngineInstance;

        private:
            EngineRegistryProperty m_RegistryProp;
            // EngineInputProperty m_InputProp;
            EngineProperty m_Prop;
    };
}

#endif