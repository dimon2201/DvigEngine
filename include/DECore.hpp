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
    typedef void                (*decallback)(demachword*, deusize);

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
            virtual void Init() {}
            void Free();

            DV_FUNCTION_INLINE deuchar* GetUSID() { return &m_USID[0]; }
            DV_FUNCTION_INLINE demachint GetUIID() { return m_UIID; }
            DV_FUNCTION_INLINE ICommon** GetCreatee() { return m_Createe; }
            DV_FUNCTION_INLINE MemoryObject** GetMemoryObject() { return m_MemoryObject; }
            DV_FUNCTION_INLINE Engine* GetEngine() { return m_Engine; }

        private:
            void SetUSIDAndUIIDAndCreateeAndMemoryObjectAndEngine(deuchar* USID, const demachint UIID, ICommon** createe, MemoryObject** memoryObject, Engine* engine);

        private:
            destring m_USID;
            demachint m_UIID;
            ICommon** m_Createe;
            MemoryObject** m_MemoryObject;
            Engine* m_Engine;
    };

    class IProperty : public ICommon
    { };

    class IComponent : public IProperty
    {
        public:
            virtual void Init() {}

        public:
            deusize m_LayoutByteWidth;
    };

    class IHelperObject : public ICommon
    {
        DV_MACRO_FRIENDS(Engine)

        public:
            virtual void Init() {}
    };

    class INode : public ICommon
    {
        DV_MACRO_FRIENDS(Engine)

        public:
            DV_FUNCTION_INLINE static INode* GetRootNode() { return m_RootNode; }
            INode* GetChildNode(const char* USID);
            IComponent* GetComponent(const char* USID);
            IHelperObject* GetHelperObject(const char* USID);

            virtual void Init();
            void Free();
            void AddChildNode(INode* const node);
            void AddHelperObject(IHelperObject* const helperObject);
            void RemoveChildNode(const char* USID);
            void RemoveComponent(const char* USID);

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
        DV_MACRO_FRIENDS(DvigEngine2::Engine)

        public:
            virtual void Init() {}
            void Free() {}

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
            static deresult CompareCharacters(const destring op1, const destring op2, const deusize opByteWidth);
            static deresult CompareCharacterStrings(const destring op1, const destring op2);
            static MemoryObject* ConcateCharacters(destring op1, destring op2);

            virtual void Init() {}
            void Free() {}

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

            virtual void Init(const deint32 memoryPoolIndex, const deusize bufferByteWidth);
            void Free();
            void Insert(const deisize offset, const void* data, const deusize dataByteWidth);
            void Find(const deisize offset, void* output, const deusize copyByteWidth);
            void Remove(const deisize offset, const deusize removeByteWidth);

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

            virtual void Init(const deint32 memoryPoolIndex, const deusize reservedCapacity, const deusize entryByteWidth);
            void Free();
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

            virtual void Init(const deint32 memoryPoolIndex, const deusize reservedCapacity, const deusize entryValueByteWidth, const deusize hashTableSize);
            void Free();
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
    };

    class JobQueueProperty : public IProperty
    {
        public:
            std::atomic<demachword> m_StopFlag;
            std::atomic<demachword> m_ReturnFlag;
            std::thread m_Thread;
            dedword m_JobCount;
            demachword m_JobArguments[DV_MAX_JOB_QUEUE_THREAD_JOB_ARGUMENT_COUNT * DV_MAX_JOB_QUEUE_THREAD_JOB_COUNT];
            decallback m_Jobs[DV_MAX_JOB_QUEUE_THREAD_JOB_COUNT];
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
            DV_FUNCTION_INLINE decallback& GetJob(deint32 arrayOffset) { return m_Jobs[arrayOffset]; }
            
            virtual void Delete();
            void Push(decallback callback, void* argumentMemory, const deusize argumentCount);
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
            decallback m_Jobs[DV_MAX_JOB_QUEUE_THREAD_JOB_COUNT];
    };

    class EngineRegistryProperty : public IProperty
    {
        public:
            HashMap* m_RegisteredComponents;
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
            MemoryPool* m_MemoryPools;
            demachword m_CurrentJobQueueCursor;
            JobQueue* m_JobQueues;
            deusize m_MaxThreadCount;
            deusize m_RequestedThreadCount;
            void* m_UserData;
    };

    class Engine
    {
        DV_MACRO_DECLARE_SINGLETON(Engine, public)

        public:
            DV_FUNCTION_INLINE EngineInputProperty* GetInputData() { return &m_InputProp; }
            DV_FUNCTION_INLINE EngineRegistryProperty* GetRegistryData() { return &m_RegistryProp; }
            DV_FUNCTION_INLINE MemoryPool* GetMemoryPoolByIndex(const deint32 memoryPoolIndex) { return &(m_Instance->m_Prop.m_MemoryPools[memoryPoolIndex]); }
            static DV_FUNCTION_INLINE demachint GetGlobalUIID() { static demachint globalUIID = 0; return globalUIID++; }

            static void Init(EngineInputProperty* engineInputProperty);
            static MemoryObject* Allocate(deint32 memoryPoolIndex, deusize byteWidth);
            static void CopyMemory(void* destAddress, const void* srcAddress, const deusize byteWidth);
            static void MoveMemory(void* destAddress, const void* srcAddress, const deusize byteWidth);
            static void SetMemory(void* destAddress, const demachword value, const deusize byteWidth);
            void Delete(MemoryObject** ppMemoryObject);

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
                const deint32 registryIndex = (const deint32)(demachword)m_RegistryProp.m_RegisteredComponents->Find( typeName );
                const deint32 bitSetIndex = (const deint32)((dedword)registryIndex >> 5u);
                const dedword bitSetBit = 1u << ((dedword)registryIndex & 31u);
                std::cout << registryIndex << std::endl;
                if (registryIndex == 0 || (((*node)->m_ComponentBitSet[bitSetIndex] >> bitSetBit) & 1u) == 1u) { return; }
                (*node)->m_ComponentBitSet[bitSetIndex] |= 1u << bitSetBit;

                (*node)->m_Components->Insert( DV_NULL, component, component->m_LayoutByteWidth );
                *component->GetCreatee() = (*node)->GetComponent( (const char*)component->GetUSID() );
            }
            
            template <typename T>
            DV_FUNCTION_INLINE T* Create(T** const result, const char* USID, const IProperty* const data)
            {
                // DV_XMACRO_PUSH_JOB(CallCreate<T>, m_Instance, result, stringID, data)
                demachword argumentMemory[3] = { (demachword)result, (demachword)&USID[0], (demachword)data };
                return CallCreate<T>(&argumentMemory[0], 0);
            }

        private:
            DV_XMACRO_GETTER_PROPERTY(EngineProperty)

            template <typename T>
            T* CallCreate(demachword* argumentMemory, deint32 jobIndex)
            {
                T** result = (T**)argumentMemory[ 0 ];
                deuchar* objectUSID = (deuchar*)argumentMemory[ 1 ];
                // IProperty* const objectData = (IProperty* const)argumentMemory[ 2 ];
                // const deusize allocationPoolID = (const deusize)m_RegistryData.m_TypeAllocationPoolID.Find( (deuchar*)typeid(T).name() );
                MemoryObject* memoryObject = Engine::Allocate(0, sizeof(T));
                T typedObjectOnStack;
                T* typedObject = memoryObject->Unwrap<T*>();
                Engine::CopyMemory( typedObject, &typedObjectOnStack, sizeof(demachword) ); // copy vpointer
                typedObject->SetUSIDAndUIIDAndCreateeAndMemoryObjectAndEngine( objectUSID, Engine::GetGlobalUIID(), (ICommon**)result, &memoryObject, m_Instance );
                if (dynamic_cast<INode*>(typedObject) != nullptr)
                {
                    INode* node = (INode*)typedObject;
                    node->Init();
                }
                else if (dynamic_cast<IComponent*>(typedObject) != nullptr)
                {
                    IComponent* component = (IComponent*)typedObject;
                    component->m_LayoutByteWidth = sizeof(T);
                }
                // T typedObjectOnStack;
                // Engine::CopyMemory( typedObject, &typedObjectOnStack, sizeof(T) );
                // typedObject->SetUSID( &objectID[0] );
                // typedObject->SetCreateeAndMemoryObject( (INode**)result, memoryObject );
                // typedObject->m_Engine = (void*)m_Instance;
                *result = typedObject;
                // if (objectData == nullptr)
                // {
                //     typedObject->m_Data.Init( m_Instance, typedObject );
                //     return;
                // }
                // IData* actualObjectData = typedObject->GetData();
                // Engine::CopyMemory(actualObjectData, objectData, sizeof(typedObject->m_Data));
                // m_RegistryData.m_Objects.Insert( objectID, typedObject );
                return typedObject;
            }

        private:
            EngineRegistryProperty m_RegistryProp;
            EngineInputProperty m_InputProp;
            EngineProperty m_Prop;
    };
}

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
    class Instance;
    class Prototype;
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
        public:
            ICommon() {}
    };

    class IShell : public ICommon
    { };

    struct IData : public ICommon
    { };

    class INode : public ICommon
    {
        DV_MACRO_FRIENDS(DvigEngine::Engine)

        public:
            INode() {}
            virtual ~INode() {}

            DV_FUNCTION_INLINE INode** GetCreatee() { return m_Createe; }
            DV_FUNCTION_INLINE MemoryObject** GetMemoryObject() { return &m_MemoryObject; }

        private:
            void SetCreateeAndMemoryObject(INode** createe, MemoryObject* memoryObject);

        private:
            INode** m_Createe;
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

    class MemoryObject: public INode
    {
        DV_MACRO_FRIENDS(DvigEngine::Engine, DvigEngine::IShell)

        public:
            DV_FUNCTION_INLINE void* GetAddress() { return m_Data.m_Address; };
            DV_FUNCTION_INLINE deusize GetByteWidth() { return m_Data.m_ByteWidth; };
            DV_FUNCTION_INLINE deint32 GetMemoryPoolIndex() { return m_Data.m_MemoryPoolIndex; };

            template<typename T>
            DV_FUNCTION_INLINE T* Unwrap()
            {
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

    class MemoryPool : public INode
    {
        DV_MACRO_FRIENDS(DvigEngine::Engine, DvigEngine::IShell)
        
        public:
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
            STRING_DATA(const char* str);

            void Init(Engine* engine, String* object);

            destring m_Chars;
            deusize m_ByteWidth;
    };

    class String : public INode
    {
        DV_MACRO_FRIENDS(DvigEngine::Engine, DvigEngine::IShell)

        public:
            DV_FUNCTION_INLINE deuchar* GetString() { return &m_Data.m_Chars[0]; };
            DV_FUNCTION_INLINE deusize GetByteWidth() { return m_Data.m_ByteWidth; };

            static deusize CharactersCount(const destring op1);
            static deresult Compare(STRING_DATA* op1, STRING_DATA* op2);
            static deresult CompareCharacters(const destring op1, const destring op2);
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

    class LinkedList : public INode
    {
        DV_MACRO_FRIENDS(DvigEngine::Engine, DvigEngine::IShell)

        public:
            virtual void Delete() {}
            
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

    class HashMap : public INode
    {
        DV_MACRO_FRIENDS(DvigEngine::Engine, DvigEngine::IShell)

        public:
            DV_FUNCTION_INLINE deusize GetListEntryCount() { return m_Data.m_ListEntryCount; };
            DV_FUNCTION_INLINE LinkedList* GetList() { return &m_Data.m_MemoryBlocks; };
            DV_FUNCTION_INLINE demachword* GetHashTable() { return &m_Data.m_HashTable[0]; };
            DV_FUNCTION_INLINE void SetHashTableEntry(deint32 index, demachword value) { m_Data.m_HashTable[index] = value; };

            static deuint32 Hash(const destring input);

            void Init();
            void Insert(destring key, void* value);
            void* Find(const destring key);
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

        public:
            void* m_Address;
            void* m_AddressOffset;
            deusize m_ByteWidth;
            deusize m_AheadByteWidth;
            deusize m_AllocatedByteWidth;
            deint32 m_MemoryPoolIndex;
    };

    class DynamicBuffer : public INode
    {
        DV_MACRO_FRIENDS(DvigEngine::Engine, DvigEngine::IShell)
        
        public:
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

            virtual void Update(Engine* engine, Instance* instance) {};
            
        private:
            destring m_TypeName;
            deint32 m_RegistryIndex;
    };

    struct INSTANCE_DATA : public IData
    {
        public:
            void Init(Engine* engine, Instance* object);

        public:
            deint32 m_GlobalIndex;
            Prototype* m_ParentPrototype;
            deusize m_ComponentCount;
            void* m_ComponentSubStorageAddress;
    };

    class Instance : public INode
    {
        DV_MACRO_FRIENDS(DvigEngine::Engine, DvigEngine::IShell, DvigEngine::INSTANCE_DATA)
        
        public:
            virtual ~Instance();

            DV_FUNCTION_INLINE Prototype* GetPrototype() { return m_Data.m_ParentPrototype; }
            DV_FUNCTION_INLINE deusize GetComponentCount() { return m_Data.m_ComponentCount; }
            DV_FUNCTION_INLINE void* GetComponentSubStorageAddress() { return m_Data.m_ComponentSubStorageAddress; };

        private:
            DV_XMACRO_GETTER_DATA(INSTANCE_DATA)

        private:
            INSTANCE_DATA m_Data;
    };

    struct PROTOTYPE_DATA : public IData
    {
        DV_MACRO_FRIENDS(DvigEngine::Engine, DvigEngine::IShell, DvigEngine::Prototype, DvigEngine::Instance)

        public:
            void Init(Engine* engine, Prototype* object);

        private:
            deusize m_InstanceCount;
            void* m_InstanceSubStorageAddress;
            deusize m_InstanceSubStorageByteWidth;
            deusize m_InstanceLayoutByteWidth;
            deusize m_SharedComponentCount;
            void* m_SharedComponentSubStorageAddress;
            deusize m_SharedComponentSubStorageByteWidth;
            dedword m_SharedComponentBits[DV_COMPONENT_DWORD_COUNT_PER_COMPONENT_COUNT];
            deusize m_ComponentCount;
            dedword m_ComponentBits[DV_COMPONENT_DWORD_COUNT_PER_COMPONENT_COUNT];
    };

    class Prototype : public INode
    {
        DV_MACRO_FRIENDS(DvigEngine::Engine, DvigEngine::IShell, DvigEngine::Instance)

        public:
            Prototype() {}
            virtual ~Prototype() {}

            DV_FUNCTION_INLINE deusize GetGlobalInstanceCount() { return m_GlobalInstanceCount; };
            DV_FUNCTION_INLINE deusize GetInstanceCount() { return m_Data.m_InstanceCount; };
            DV_FUNCTION_INLINE deusize GetInstanceLayoutSize() { return m_Data.m_InstanceLayoutByteWidth; };
            DV_FUNCTION_INLINE void* GetInstanceSubStorageAddress() { return m_Data.m_InstanceSubStorageAddress; };
            DV_FUNCTION_INLINE deusize GetInstanceSubStorageSize() { return m_Data.m_InstanceSubStorageByteWidth; };
            DV_FUNCTION_INLINE void* GetSharedComponentSubStorageAddress() { return m_Data.m_SharedComponentSubStorageAddress; };
            DV_FUNCTION_INLINE deusize GetSharedComponentSubStorageSize() { return m_Data.m_SharedComponentSubStorageByteWidth; };
            DV_FUNCTION_INLINE deusize GetSharedComponentCount() { return m_Data.m_SharedComponentCount; };
            DV_FUNCTION_INLINE debool GetSharedComponentBit(deuint32 arrayOffset, deuint32 bitOffset) { return (m_Data.m_SharedComponentBits[arrayOffset] >> bitOffset) & 1u; };
            DV_FUNCTION_INLINE deusize GetComponentCount() { return m_Data.m_ComponentCount; };
            DV_FUNCTION_INLINE debool GetComponentBit(deuint32 arrayOffset, deuint32 bitOffset) { return (m_Data.m_ComponentBits[arrayOffset] >> bitOffset) & 1u; };

        private:
            DV_XMACRO_GETTER_DATA(PROTOTYPE_DATA)

        private:
            static deusize m_GlobalInstanceCount;
            PROTOTYPE_DATA m_Data;
    };

    struct JOB_QUEUE_DATA : IData
    {
        public:
            std::atomic<demachword> m_StopFlag;
            std::atomic<demachword> m_ReturnFlag;
            std::thread m_Thread;
            dedword m_JobCount;
            demachword m_JobArguments[DV_MAX_JOB_QUEUE_THREAD_JOB_ARGUMENT_COUNT * DV_MAX_JOB_QUEUE_THREAD_JOB_COUNT];
            decallback m_Jobs[DV_MAX_JOB_QUEUE_THREAD_JOB_COUNT];
    };
    
    class JobQueue : public INode
    {
        DV_MACRO_FRIENDS(DvigEngine::Engine, DvigEngine::IShell)

        public:
            virtual void Delete();

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
            deint32 m_PrototypeStorageMemoryPoolID;
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
            DV_FUNCTION_INLINE MemoryPool* GetMemoryPoolByID(const deint32 memoryPoolID) { return &(m_Instance->m_Data.m_MemoryPools[memoryPoolID]); }
            DV_FUNCTION_INLINE ENGINE_INPUT_DATA* GetPrivateInputData() { return &m_InputData; };
            DV_FUNCTION_INLINE void SetUserData(void* address) { m_Data.m_UserData = address; };

            static void Init(ENGINE_INPUT_DATA* engineInputData);
            static void Free();
            // static void* Allocate(deusize memoryPoolID, deusize byteWidth);
            static MemoryObject* ObjectAllocate(deusize memoryPoolID, deusize byteWidth);
            static void* AllocateUsingData(MEMORY_POOL_DATA* memoryPool, deusize byteWidth);
            static void ObjectDelete(MemoryObject** ppMemoryObject);
            static void CopyMemory(void* destAddress, const void* srcAddress, const deusize byteWidth);
            static void MoveMemory(void* destAddress, const void* srcAddress, const deusize byteWidth);
            static void SetMemory(void* destAddress, const demachword value, const deusize byteWidth);
            
            void StartThreads();
            void StopThreads();
            void UpdateSystems();

        public:
            // Getters not related to Engine

            // ECS related functions
            template<typename T>
            void RegisterComponent()
            {
                deuchar* typeName = (deuchar*)typeid(T).name();
                const deint32 componentIndex = ++m_RegistryData.m_UniqueComponentCount;
                if (m_RegistryData.m_Components.Find(typeName) != nullptr) { return; }
                m_RegistryData.m_Components.Insert(typeName, (void*)(demachword)componentIndex);
            }

            template<typename T>
            void RegisterSystem()
            {
                deuchar* typeName = (deuchar*)typeid(T).name();
                const deint32 systemIndex = ++m_RegistryData.m_UniqueSystemCount;
                if (m_RegistryData.m_Systems.Find(typeName) != nullptr) { return; }
                T systemOnStack;
                Engine::SetMemory( &systemOnStack.m_TypeName[0], 0, DV_MEMORY_COMMON_STRING_BYTE_WIDTH );
                T* system = Engine::ObjectAllocate( 0, sizeof(T) )->Unwrap<T>();
                Engine::CopyMemory( system, &systemOnStack, sizeof(T) );
                Engine::CopyMemory( &system->m_TypeName[0], &typeName[0], String::CharactersCount( typeName ) );
                system->m_RegistryIndex = systemIndex;
                m_RegistryData.m_Systems.Insert( typeName, (void*)(demachword)system );
            }

            template<typename T>
            DV_FUNCTION_INLINE void PrototypeAddSharedComponent(Prototype* const prototype, IComponent* const component)
            {
                const deuchar* const typeName = (const deuchar* const)typeid(T).name();
                const deusize typeNameByteWidth = String::CharactersCount( typeName );
                const deint32 registryIndex = (deint32)(demachword)m_RegistryData.m_Components.Find( typeName );
                const deint32 maskIndex = (deint32)((dedword)registryIndex >> 5u);
                const dedword maskBit = 1u << ((dedword)registryIndex & 31u);
                if (((prototype->m_Data.m_ComponentBits[ maskIndex ] >> maskBit) & 1u) ||
                    ((prototype->m_Data.m_SharedComponentBits[ maskIndex ] >> maskBit) & 1u)) { return; } // already exists
                prototype->m_Data.m_InstanceLayoutByteWidth += sizeof(T);
                prototype->m_Data.m_ComponentCount += 1;
                prototype->m_Data.m_ComponentBits[ maskIndex ] |= 1u << maskBit;

                if (component == nullptr) { return; } // nowhere to copy from

                const deusize sharedComponentByteWidth = sizeof(T);
                const deint32 prototypeStorageMemoryPoolIndex = m_Instance->GetPrivateInputData()->m_PrototypeStorageMemoryPoolID;
                void* moveFromAddress = (void*)((demachword)prototype->GetSharedComponentSubStorageAddress());
                void* moveToAddress = (void*)((demachword)moveFromAddress + sizeof(MemoryObject) + sharedComponentByteWidth);
                Engine::ObjectAllocate( prototypeStorageMemoryPoolIndex, sharedComponentByteWidth );
                Engine::MoveMemory( moveToAddress, moveFromAddress, sharedComponentByteWidth );

                T* sharedComponent = (T*)((demachword)moveFromAddress + sizeof(MemoryObject));
                Engine::CopyMemory( sharedComponent, component, sharedComponentByteWidth );
                Engine::CopyMemory( &sharedComponent->m_TypeName[0], &typeName[0], typeNameByteWidth );
                sharedComponent->m_TypeName[typeNameByteWidth] = 0;
                sharedComponent->m_LayoutByteWidth = sizeof(T);
                sharedComponent->m_RegistryIndex = registryIndex;
                
                prototype->m_Data.m_SharedComponentCount += 1;
                prototype->m_Data.m_SharedComponentSubStorageByteWidth += sharedComponentByteWidth;
                prototype->m_Data.m_SharedComponentBits[ maskIndex ] |= 1u << maskBit;
            }

            void PrototypeInstantiate(Instance** const result, const char* USID, Prototype* const prototype);
            
            template<typename T>
            DV_FUNCTION_INLINE void InstanceAddComponent(Instance* const instance, IComponent* const component)
            {
                std::cout << "i : " << (demachword)instance << std::endl; 
                deusize componentLayoutByteWidth = 0;
                const deuchar* const typeName = (const deuchar* const)typeid(T).name();
                const deint32 registryIndex = (deint32)(demachword)m_RegistryData.m_Components.Find( typeName );
                Prototype* parentPrototype = instance->m_Data.m_ParentPrototype;
                std::cout << "pp : " << (demachword)parentPrototype << " " << instance->m_Data.m_ComponentCount << " " << (demachword)instance << std::endl;
                const deint32 componentCount = parentPrototype->GetComponentCount();
                void* componentAddress = instance->m_Data.m_ComponentSubStorageAddress;
                std::cout << "cc : " << componentCount << std::endl;
                for (deint32 i = 0; i < componentCount; ++i)
                {
                    componentAddress = (void*)((demachword)componentAddress + componentLayoutByteWidth);
                    IComponent* curComponent = (IComponent*)componentAddress;
                    componentLayoutByteWidth = curComponent->m_LayoutByteWidth;
                                    std::cout << "cctn : " << curComponent->m_TypeName << std::endl;

                    if ((curComponent->m_TypeName != nullptr &&
                        String::CompareCharacters( &curComponent->m_TypeName[0], &typeName[0] ))
                        || (componentLayoutByteWidth == 0))
                    {
                        componentLayoutByteWidth = sizeof(T);
                        // Free space in Instance layout
                        if (component == nullptr)
                        {
                            // From Prototype
                            const deint32 maskIndex = (deint32)((dedword)registryIndex >> 5u);
                            const dedword maskBit = 1u << ((dedword)registryIndex & 31u);
                            if ((parentPrototype->m_Data.m_SharedComponentBits[ maskIndex ] >> maskBit) & 1u) {
                                // Insert
                                const T* const copyComponent = (const T* const)FindComponent<T>( parentPrototype->GetSharedComponentCount(), parentPrototype->GetSharedComponentSubStorageAddress() );
                                Engine::CopyMemory( curComponent, copyComponent, componentLayoutByteWidth );
                                instance->m_Data.m_ComponentCount += 1;
                            }
                            
                            return;
                        }
                        else
                        {
                            // From function argument
                            // Insert
                            const deusize typeNameByteWidth = String::CharactersCount( typeName );
                            Engine::CopyMemory( curComponent, component, componentLayoutByteWidth );
                            Engine::CopyMemory( &curComponent->m_TypeName[0], &typeName[0], typeNameByteWidth );
                            curComponent->m_TypeName[typeNameByteWidth] = 0;
                            curComponent->m_LayoutByteWidth = sizeof(T);
                            curComponent->m_RegistryIndex = registryIndex;
                            instance->m_Data.m_ComponentCount += 1;

                            return;
                        }
                    }
                }
            }

            template<typename T>
            DV_FUNCTION_INLINE T* FindComponent(const deusize count, const void* const address)
            {
                deuint32 componentCount = (const deuint32)count;
                if (componentCount == 0) { componentCount = DV_MAX_DWORD_VALUE; }
                const void* componentAddress = (void*)address;
                for (deuint32 i = 0u; i < componentCount; ++i)
                {
                    IComponent* baseComponent = (IComponent*)componentAddress;
                    deuchar* typeName = (deuchar*)typeid(T).name();
                    std::cout << baseComponent->m_TypeName << std::endl;
                    if (String::CompareCharacters( &baseComponent->m_TypeName[0], &typeName[0] )) {
                        return (T*)baseComponent;
                    }
                    if (baseComponent->m_LayoutByteWidth <= 0) { break; }

                    componentAddress = (void*)((demachword)componentAddress + (demachword)baseComponent->m_LayoutByteWidth);
                }

                return nullptr;
            }

            template<typename T>
            DV_FUNCTION_INLINE T* InstanceGetComponent(Instance* const instance)
            {
                return FindComponent <T> ( instance->m_Data.m_ComponentCount, instance->m_Data.m_ComponentSubStorageAddress );
            }

            template <typename T>
            DV_FUNCTION_INLINE deint32 CheckType(const dedword* const bitSet)
            {
                const deuchar* const typeName = (const deuchar* const)typeid(T).name();
                const deint32 registryIndex = (deint32)(demachword)m_RegistryData.m_Components.Find( typeName );
                const deint32 maskIndex = (deint32)((dedword)registryIndex >> 5u);
                const dedword maskBit = 1u << ((dedword)registryIndex & 31u);
                if ((bitSet[ maskIndex ] >> maskBit) & 1u) { return registryIndex; }
                return DV_NULL;
            }

            // Wrapper functions
            template<typename T>
            DV_FUNCTION_INLINE void ObjectCreate(T** const result, const char* ID, IData* data)
            {
                // DV_XMACRO_PUSH_JOB(CallCreate<T>, m_Instance, result, stringID, data)
                demachword argumentMemory[3] = { (demachword)result, (demachword)ID, (demachword)data };
                CallObjectCreate<T>(&argumentMemory[0], 0);
            }

        private:
            // Job functions
            template<class T>
            DV_FUNCTION_INLINE void CallObjectCreate(demachword* argumentMemory, deusize jobIndex)
            {
                T** const result = (T** const)argumentMemory[ 0 ];
                deuchar* objectID = (deuchar*)argumentMemory[ 1 ];
                IData* objectData = (IData*)argumentMemory[ 2 ];
                const deusize allocationPoolID = (const deusize)m_RegistryData.m_TypeAllocationPoolID.Find( (deuchar*)typeid(T).name() );
                MemoryObject* memoryObject = ObjectAllocate(allocationPoolID, sizeof(T));
                T* typedObject = (T*)memoryObject->Unwrap<T>();
                T typedObjectOnStack;
                Engine::CopyMemory( typedObject, (const void*)&typedObjectOnStack, sizeof(T) );
                typedObject->SetUSID( &objectID[0] );
                typedObject->SetCreateeAndMemoryObject( (INode**)result, memoryObject );
                typedObject->m_Data.m_Engine = (void*)m_Instance;
                *result = typedObject;
                if (objectData == nullptr)
                {
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