#ifndef _DE_LIB_CORE_H_
#define _DE_LIB_CORE_H_
#include <iostream>
namespace DvigEngine
{
    #define DVIG_VERSION_MAJOR                                  0
    #define DVIG_VERSION_MINOR                                  1
    #define DVIG_VERSION_RELEASE                                0
    #define DVIG_MAKE_VERSION_WORD                              ((DVIG_VERSION_MAJOR << 16) | (DVIG_VERSION_MINOR << 8) | DVIG_VERSION_RELEASE)
    #define DVIG_CURRENT_VERSION                                DVIG_MAKE_VERSION_WORD

    #define DVIG_DEBUG
    #define DVIG_TRUE                                           1
    #define DVIG_FALSE                                          0
    #define DVIG_NULL                                           4294967295

    #define DVIG_MEMORY_KiB                                     1024
    #define DVIG_MEMORY_MiB                                     DVIG_MEMORY_KiB * DVIG_MEMORY_KiB
    #define DVIG_MEMORY_GiB                                     DVIG_MEMORY_KiB * DVIG_MEMORY_MiB

    #define DVIG_MAX_STRING_BYTE_WIDTH                          48

    #define DVIG_XMACRO_DECLARE_STATIC_CLASS(T) private: T();

    #define DVIG_ASSERT(S, E) if (!S) { Debug::PushErrorOrWarning(E); }

    typedef signed char                                         sint8;
    typedef unsigned char                                       uint8;
    typedef signed short                                        sint16;
    typedef unsigned short                                      uint16;
    typedef signed int                                          sint32;
    typedef unsigned int                                        uint32;
    typedef signed long long                                    sint64;
    typedef unsigned long long                                  uint64;
    typedef float                                               float32;
    typedef double                                              float64;
    typedef uint64                                              resultword;
    typedef uint8                                               string[DVIG_MAX_STRING_BYTE_WIDTH];

    #if defined(_WIN32) || defined(_WIN64)
        // Windows OS
        #define DVIG_MACRO_OS_VALUE                             "Windows"
        #define DVIG_MACRO_OS_WINDOWS
    #elif defined(unix) || defined(__unix) || defined(__unix__)
        // Unix OS
        #define DVIG_MACRO_OS_VALUE                             "Unix"
        #define DVIG_MACRO_OS_UNIX
    #endif

    #if defined(__ILP32__) || defined(__arm__) || defined(_M_ARM) || defined(__i386__) || defined(_M_IX86) || defined(_X86_)
        // 32-bit architecture
        #define DVIG_MACRO_ARCH_VALUE                           32
        #define DVIG_MACRO_ARCH_32_BIT
        #define DVIG_MACRO_ARCH_WORD_BYTE_WIDTH                 4
        #define DVIG_ENTITY_ID_BYTE_WIDTH                       4
        typedef sint32                                          ssize;
        typedef uint32                                          usize;
        typedef sint32                                          scpuint;
        typedef uint32                                          ucpuint;
        typedef uint32                                          cpuword;
        typedef cpuword                                         boolean;
    #elif defined(__amd64__) || defined(_M_AMD64) || defined(_M_X64) || defined(__aarch64__) || defined(__ia64__) || defined(_M_IA64)
        // 64-bit architecture
        #define DVIG_MACRO_ARCH_VALUE                           64
        #define DVIG_MACRO_ARCH_64_BIT
        #define DVIG_MACRO_ARCH_WORD_BYTE_WIDTH                 8
        #define DVIG_ENTITY_ID_BYTE_WIDTH                       8
        typedef sint64                                          ssize;
        typedef uint64                                          usize;
        typedef sint64                                          scpuint;
        typedef uint64                                          ucpuint;
        typedef uint64                                          cpuword;
        typedef cpuword                                         boolean;
    #endif

    #if defined(__GNUC__) || defined(__GNUG__)
        // GNU compiler
        #define DVIG_COMPILER_VALUE                             0
        #define DVIG_FUNCTION_INLINE                            inline __attribute__((always_inline))
        #define DVIG_TRAILING_ZEROS_COUNT(x)                    __builtin_ctz(x)
    #elif defined(_MSC_VER)
        // Microsoft compiler
        // #include <intrin.h>
        // #define DVIG_COMPILER_VALUE                             1
        #define DVIG_FUNCTION_INLINE                            inline __forceinline
        // #define DVIG_TRAILING_ZEROS_COUNT(x)                    (32 - __lzcnt(x))
    #else
        // Other...
        #define DVIG_COMPILER_VALUE                             2
        #define DVOG_FUNCTION_INLINE                            inline
    #endif

    class MemoryObject;
    class Collection;
    class Engine;

    enum class DebugResult
    {
        NULL_POINTER = 0,
        FILLED_MEMORY_POOL = 1,
        NOT_ENOUGH_MEMORY_POOL = 2,
        SECOND_INSTANCE_ENGINE = 3,
        EXCEEDED_MAX_COUNT_WINDOW = 4,
        NOT_CREATED_SWAPCHAIN_D3D11 = 5
    };

    class DebugState
    {
        public:
    };

    class Debug
    {
        friend Engine;
        DVIG_XMACRO_DECLARE_STATIC_CLASS(Debug)
        
        public:
            static void PushErrorOrWarning(DebugResult result);
            static void ProcessErrorOrWarning(DebugResult result);

        private:
            static DebugState* m_State;
    };

    class Object
    {
        friend Engine;

        public:
            virtual ~Object() {}

            void Free() {}

            DVIG_FUNCTION_INLINE const char* GetUSID() { return (const char*)&this->m_USID[0]; }
            DVIG_FUNCTION_INLINE MemoryObject* GetMemoryObject() { return this->m_MemoryObject; }

        private:
            DVIG_FUNCTION_INLINE void SetPrivateData(const char* usid, MemoryObject* memObject) {
                usize usidByteWidth = 0;
                sint32 cycle = 0;
                while (usid[cycle] != 0) { cycle += 1; }
                usidByteWidth = cycle;
                cycle = 0;
                while ((usize)cycle < usidByteWidth) {
                    this->m_USID[cycle] = usid[cycle];
                    cycle += 1;
                }
                this->m_MemoryObject = memObject;
            }

        private:
            char m_USID[DVIG_MAX_STRING_BYTE_WIDTH];
            MemoryObject* m_MemoryObject;
    };

    class Info
    {};

    template <typename T>
    class Ptr
    {
        public:
            DVIG_FUNCTION_INLINE static T Add(const void* ptr, cpuword value) {
                return (T)((cpuword)ptr + (cpuword)value);
            }

            DVIG_FUNCTION_INLINE static T Subtract(const void* ptr, cpuword value) {
                return (T)((cpuword)ptr - (cpuword)value);
            }
    };

    enum class MemoryObjectFlag
    {
        FREE = 1,
        OCCUPIED = 2,
    };

    class MemoryObject
    {
        friend Engine;

        public:
            DVIG_FUNCTION_INLINE void* GetAddress() { return (void*)Ptr<void*>::Add((void*)this, sizeof(MemoryObject)); }
            DVIG_FUNCTION_INLINE usize GetSize() { return m_ByteWidth; }

        private:
            usize m_ByteWidth;
            MemoryObjectFlag m_Flags;
    };

    class Node : public Object
    {
        public:
            void Init();
            void Free();

            void AddChild(Object* object);
            Object* FindChild(const char* label);

            template <typename T>
            T* FindChildByType()
            {
                for (uint32 i = 0; i < this->m_Childs->GetCount(); ++i) {
                    Object* instance = (*((Object**)this->m_Childs->FindAt(i)));
                    if (dynamic_cast<T*>(instance) != nullptr) {
                        return (T*)instance;
                    }
                }

                return nullptr;
            }

            void RemoveChild(const char* label);

        private:
            Collection* m_Childs;
    };

    class Component : public Object
    {
        friend Node;

        public:
            void Free() {}

            DVIG_FUNCTION_INLINE Node* GetParentNode() { return m_Parent; }

        private:
            Node* m_Parent;
    };

    class ISystem
    {
        public:
            virtual ~ISystem() {}

            DVIG_FUNCTION_INLINE virtual void UpdateComponent(Component* component) = 0;
            void Update();
    };

    class Collection : public Node
    {
        public:
            void Init(const usize entryByteWidth, const usize reservedCapacity);
            void Free();

            uint32 Insert(const void* entryMemory, const char* label);
            void* Find(const char* label);
            void* FindAt(const uint32 index);
            void Remove(const char* label);
            void RemoveAt(const uint32 index);
            void Clear();

            DVIG_FUNCTION_INLINE usize GetSize() { return m_CurrentByteWidth; }
            DVIG_FUNCTION_INLINE usize GetCount() { return m_EntryCount; }
        
        private:
            usize m_EntryByteWidth;
            usize m_ReservedCapacity;
            usize m_EntryCount;
            MemoryObject* m_MemoryObject;
            usize m_CurrentByteWidth;
            usize m_AllocatedByteWidth;
    };

    class HashedCollection : public Node
    {
        public:
            void Init(const usize entryByteWidth, const usize reservedCapacity, const usize hashTableEntryCount);
            void Free();
            
            uint32 Insert(const void* entryMemory, const char* label);
            void* Find(const char* label);
            void* FindAt(const uint32 index);
            void Remove(const char* label);
            void Clear();

            DVIG_FUNCTION_INLINE usize GetSize() { return m_Collection->GetSize(); }
            DVIG_FUNCTION_INLINE usize GetCount() { return m_Collection->GetCount(); }

        public:
            uint32 HashMurMur(const char* input, const cpuword bitMask);

        private:
            Collection* m_Collection;
            MemoryObject* m_MemoryObject;
            uint32* m_HashTable;
            usize m_HashTableEntryCount;
    };

    class StringSystem : public ISystem
    {
        DVIG_XMACRO_DECLARE_STATIC_CLASS(StringSystem)

        public:
            static void Init();
            static void Free();

            static usize GetByteWidth(const char* string);
            static boolean GetStringsEqual(const char* string1, const char* string2);
    };

    class MemoryPoolInfo : public Info
    {
        public:
            void* m_Address;
            usize m_ByteWidth;
            usize m_OccupiedByteWidth;
            usize m_AllocationCount;
    };

    class EngineInfo : public Info
    {
        public:
            cpuword Version;
            usize SystemMemoryPoolByteWidth;
            usize ComponentStorageMemoryPoolByteWidth;
    };

    class Engine
    {
        public:
            Engine(const EngineInfo* info);
            void Free();

            template <typename T>
            T* Create(const char* usid)
            {
                DVIG_ASSERT( (Engine::m_Instance != nullptr), DebugResult::NULL_POINTER )
                
                Engine* pEngine = Engine::GetClassInstance();
                sint32 memoryPoolIndex = 0;
                T instanceOnStack;
                if (dynamic_cast<Component*>(&instanceOnStack) != nullptr) { memoryPoolIndex = 1; }
                MemoryObject* memObject = pEngine->MemoryAllocate( memoryPoolIndex, sizeof(T) );

                T* instance = (T*)memObject->GetAddress();
                pEngine->MemoryCopy( instance, &instanceOnStack, sizeof(cpuword));
                instance->SetPrivateData( (const char*)&usid[0], memObject );

                if (StringSystem::GetStringsEqual( &usid[0], "_EngineAllInstancesCollection" ) == DVIG_FALSE &&
                    StringSystem::GetStringsEqual( &usid[0], "_EngineAllInstancesHashedCollection" ) == DVIG_FALSE
                ) {
                    pEngine->m_AllInstances->Insert( &instance, &usid[0] );
                }
                
                return instance;
            }

            template <typename T>
            DVIG_FUNCTION_INLINE T* FindExistingInstance(const char* usid)
            {
                DVIG_ASSERT( (Engine::m_Instance != nullptr), DebugResult::NULL_POINTER )

                return *((T**)Engine::GetClassInstance()->m_AllInstances->Find( &usid[0] ));
            }

            DVIG_FUNCTION_INLINE void Delete(Object* instance)
            {
                DVIG_ASSERT( ((Engine::m_Instance != nullptr) && (instance != nullptr)), DebugResult::NULL_POINTER )

                Engine::GetClassInstance()->m_AllInstances->Remove( &instance->GetUSID()[0] );
                instance->Free();
            }

            MemoryObject* MemoryAllocate(const sint32 index, const usize byteWidth);
            void MemoryDelete(MemoryObject* object);
            void MemoryCopy(void* destAddress, const void* srcAddress, const usize byteWidth);
            void MemoryMove(void* destAddress, const void* srcAddress, const usize byteWidth);
            void MemorySet(void* destAddress, const DvigEngine::cpuword value, const DvigEngine::usize byteWidth);

            DVIG_FUNCTION_INLINE static Engine* GetClassInstance() { return Engine::m_Instance; }
            DVIG_FUNCTION_INLINE EngineInfo* GetEngineInfo() { return Engine::m_Instance->m_Info; }
            DVIG_FUNCTION_INLINE MemoryPoolInfo* GetMemoryPoolInfo(const uint32 index) { return &Engine::m_Instance->m_MemoryPools[index]; }
            DVIG_FUNCTION_INLINE HashedCollection* GetInstances() { return Engine::m_Instance->m_AllInstances; }

        private:
            void MemoryInsertObjectToMemoryPool(const sint32 index, const usize offset, const usize byteWidth);

        private:
            static Engine* m_Instance;
            EngineInfo* m_Info;
            MemoryPoolInfo m_MemoryPools[2];
            HashedCollection* m_AllInstances;
    };
}

#endif