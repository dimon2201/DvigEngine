#ifndef _DV_H_MACROS
#define _DV_H_MACROS

#ifndef _DV_EOF
#define _DV_EOF
#endif

#define DV_TRUE                                         1
#define DV_FALSE                                        0
#define DV_ZERO                                         0
#define DV_NULL                                         -1

#define DV_DEBUG                                        DV_FALSE

#define DV_ENGINE_VERSION_MAJOR                         1
#define DV_ENGINE_VERSION_MINOR                         0
#define DV_ENGINE_VERSION_RELEASE                       0
#define DV_ENGINE_VERSION_NUMBER                        ((DV_ENGINE_VERSION_MAJOR << 16) | \
                                                        (DV_ENGINE_VERSION_MINOR << 8) | \
                                                        (DV_ENGINE_VERSION_RELEASE))

#if defined(__ILP32__) || defined(__arm__) || defined(_M_ARM) || defined(__i386__) || defined(_M_IX86) || defined(_X86_)
    // 32-bit architecture
    #define DV_MACRO_ARCH_VALUE                         32
    #define DV_MACRO_ARCH_32_BIT
    #define DV_MACRO_ARCH_WORD_BYTE_WIDTH               4
    #define DV_ENTITY_ID_BYTE_WIDTH                     4
#elif defined(__amd64__) || defined(_M_AMD64) || defined(_M_X64) || defined(__aarch64__) || defined(__ia64__) || defined(_M_IA64)
    // 64-bit architecture
    #define DV_MACRO_ARCH_VALUE                         64
    #define DV_MACRO_ARCH_64_BIT
    #define DV_MACRO_ARCH_WORD_BYTE_WIDTH               8
    #define DV_ENTITY_ID_BYTE_WIDTH                     8
#endif
#if defined(__GNUC__) || defined(__GNUG__)
    // GNU compiler
    #define DV_COMPILER_VALUE                           0
    #define DV_FUNCTION_INLINE                          inline __attribute__((always_inline))
    #define DV_TRAILING_ZEROS_COUNT(x)                  __builtin_ctz(x)
#elif defined(_MSC_VER)
    // Microsoft compiler
    #define DV_COMPILER_VALUE                           1
    #define DV_FUNCTION_INLINE                          inline __forceinline
    #define DV_TRAILING_ZEROS_COUNT(x)                  (32 - __lzcnt(x))
#else
    // Other...
    #define DV_COMPILER_VALUE                           2
    #define DV_FUNCTION_INLINE                          inline
#endif

#define DV_MEMORY_KiB                                   1024
#define DV_MEMORY_MiB                                   DV_MEMORY_KiB * DV_MEMORY_KiB
#define DV_MEMORY_GiB                                   DV_MEMORY_KiB * DV_MEMORY_MiB

#define DV_MEMORY_COMMON_STRING_BYTE_WIDTH              32
#define DV_MEMORY_COMMON_HASH_MAP_TABLE_SIZE            1024
#define DV_MEMORY_COMMON_HASH_MAP_TABLE_BYTE_WIDTH      DV_MEMORY_COMMON_HASH_MAP_TABLE_SIZE * DV_MACRO_ARCH_WORD_BYTE_WIDTH
#define DV_MEMORY_COMMON_EXTRA_DATA                     DV_MACRO_ARCH_WORD_BYTE_WIDTH

#define DV_MEMORY_POOL_RESERVED_ID                      0
#define DV_MEMORY_POOL_BYTE_WIDTH                       32 * DV_MEMORY_MiB
#define DV_MEMORY_POOL_COMPONENT_STORAGE_BYTE_WIDTH     DV_MEMORY_POOL_BYTE_WIDTH / 2

#define DV_MAX_HASH_MAP_MEMORY_BLOCK_BYTE_WIDTH         1024
#define DV_MAX_HASH_MAP_LIST_ENTRY_COUNT                100
#define DV_MAX_REGISTERED_COMPONENT_COUNT               512
#define DV_MAX_JOB_QUEUE_THREAD_JOB_ARGUMENT_COUNT      4
#define DV_MAX_JOB_QUEUE_THREAD_JOB_COUNT               12

#define DV_COMPONENT_DWORD_COUNT_PER_COMPONENT_COUNT    (DV_MAX_REGISTERED_COMPONENT_COUNT >> 5)

#define DV_ASSERT(E) \
    if (E == DV_FALSE) \
    { \
        exit(0); \
    }

#define DV_ASSERT_PTR(E) \
    if (E == nullptr) \
    { \
        exit(0); \
    }

#define DV_XMACRO_DECLARE_COMMON_CLASS(T) \
    public: \
        T(); \
        virtual ~T() {}; \
        DV_FUNCTION_INLINE deuchar* GetSID() { return &m_SID[0]; } \
        DV_FUNCTION_INLINE deusize GetSIDByteWidth() { return m_SIDByteWidth; } \
    private: \
        DV_FUNCTION_INLINE void SetSID(deuchar* stringID) { \
            m_SIDByteWidth = 0; \
            while (stringID[m_SIDByteWidth] != 0) { m_SID[m_SIDByteWidth] = stringID[m_SIDByteWidth]; ++m_SIDByteWidth; } \
            m_SID[m_SIDByteWidth] = 0; \
        }; \
    private: \
        demachword m_IID; \
        deuchar m_SID[DV_MEMORY_COMMON_STRING_BYTE_WIDTH]; \
        deusize m_SIDByteWidth; \
        deuchar m_ExtraData[DV_MEMORY_COMMON_EXTRA_DATA];

#define DV_MACRO_DECLARE_CREATION_DEPENDENT_CLASS(T) \
    public: \
        T(); \
        virtual ~T() {}; \

#define DV_MACRO_DECLARE_SINGLETON(T, function_access) \
    function_access: \
        static T* GetInstance(); \
    private: \
        T(); \
        T(const T&) = delete; \
        T(T&&) = delete; \
        T& operator=(T&&) = delete; \
        void operator=(const T&) = delete; \
        void* operator new(deuint64) = delete; \
        static T* m_Instance;

#define DV_MACRO_DEFINE_SINGLETON(T) \
    T* T::m_Instance = nullptr; \
    T* T::GetInstance() { DV_ASSERT_PTR(m_Instance); return m_Instance; }

#define DV_MACRO_GETTER(T) \
    return T;

#define DV_MACRO_ASSERTED_GETTER(A, T) \
    DV_ASSERT_PTR(A) \
    DV_MACRO_GETTER(T)

#define DV_MACRO_SETTER(T, TO) \
    T = TO;

#define DV_XMACRO_GETTER_DATA(T) \
        DV_FUNCTION_INLINE T* GetData() { DV_MACRO_GETTER((T*)&m_Data) };

#define DV_XMACRO_STRING(T, TO) \
    deusize strByteWidth = 0; \
    deuchar* strCopy = (deuchar*)T; \
    while (*strCopy != 0) { TO.m_Chars[strByteWidth++] = *strCopy++; }

#define DV_XMACRO_XCONCATE(A, B) A ## B
#define DV_MACRO_CONCATE(A, B) DV_XMACRO_XCONCATE(A, B)

#define DV_XMACRO_FRIEND_PREFIX
#define DV_XMACRO_FRIEND friend
#define DV_XMACRO_FRIEND_BLANK Blank
#define DV_XMACRO_FRIEND_TEMPLATE DV_XMACRO_FRIEND DV_XMACRO_FRIEND_PREFIX

#define DV_XMACRO_XARGS_XCNT_N(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, N, ...) N
#define DV_XMACRO_XARGS_CNT_N_SEQ \
        10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0
#define DV_XMACRO_XARGS_XCNT_XN(...) DV_XMACRO_XARGS_XCNT_N(__VA_ARGS__)
#define DV_XMACRO_XARGS_XCNT(...) DV_XMACRO_XARGS_XCNT_XN(__VA_ARGS__, DV_XMACRO_XARGS_CNT_N_SEQ)
#define DV_XMACRO_XARGS_CNT(...) DV_XMACRO_XARGS_XCNT(__VA_ARGS__)
#define DV_MACRO_ARGS_CNT(...) DV_XMACRO_XARGS_CNT(__VA_ARGS__)

#define DV_XMACRO_FRIENDS_SEQ_1(_0, ...) DV_XMACRO_FRIEND_TEMPLATE _0 ;
#define DV_XMACRO_FRIENDS_SEQ_2(_0, _1, ...) DV_XMACRO_FRIEND_TEMPLATE _0 ; DV_XMACRO_FRIEND_TEMPLATE _1 ;
#define DV_XMACRO_FRIENDS_SEQ_3(_0, _1, _2, ...) DV_XMACRO_FRIEND_TEMPLATE _0 ; DV_XMACRO_FRIEND_TEMPLATE _1 ; DV_XMACRO_FRIEND_TEMPLATE _2 ;
#define DV_XMACRO_FRIENDS_SEQ_4(_0, _1, _2, _3, ...) DV_XMACRO_FRIEND_TEMPLATE _0 ; DV_XMACRO_FRIEND_TEMPLATE _1 ; DV_XMACRO_FRIEND_TEMPLATE _2 ; DV_XMACRO_FRIEND_TEMPLATE _3 ;
#define DV_XMACRO_FRIENDS_SEQ_5(_0, _1, _2, _3, _4, ...) DV_XMACRO_FRIEND_TEMPLATE _0 ; DV_XMACRO_FRIEND_TEMPLATE _1 ; DV_XMACRO_FRIEND_TEMPLATE _2 ; DV_XMACRO_FRIEND_TEMPLATE _3 ; DV_XMACRO_FRIEND_TEMPLATE _4 ;
#define DV_XMACRO_FRIENDS_SEQ_6(_0, _1, _2, _3, _4, _5, ...) DV_XMACRO_FRIEND_TEMPLATE _0 ; DV_XMACRO_FRIEND_TEMPLATE _1 ; DV_XMACRO_FRIEND_TEMPLATE _2 ; DV_XMACRO_FRIEND_TEMPLATE _3 ; DV_XMACRO_FRIEND_TEMPLATE _4 ; DV_XMACRO_FRIEND_TEMPLATE _5 ;
#define DV_XMACRO_XFRIENDS(...) DV_MACRO_CONCATE(DV_XMACRO_FRIENDS_SEQ_, DV_MACRO_ARGS_CNT(__VA_ARGS__))(__VA_ARGS__, DV_XMACRO_FRIEND_BLANK)
#define DV_MACRO_FRIENDS(...) DV_XMACRO_XFRIENDS(__VA_ARGS__)

#define DV_XMACRO_PUSH_JOB_ARGS_1(_0, ...) argumentMemory[0] = (demachword)_0 ;
#define DV_XMACRO_PUSH_JOB_ARGS_2(_0, _1, ...) argumentMemory[0] = (demachword)_0 ; argumentMemory[1] = (demachword)_1 ;
#define DV_XMACRO_PUSH_JOB_ARGS_3(_0, _1, _2, ...) argumentMemory[0] = (demachword)_0 ; argumentMemory[1] = (demachword)_1 ; argumentMemory[2] = (demachword)_2 ;
#define DV_XMACRO_PUSH_JOB_ARGS_4(_0, _1, _2, _3, ...) argumentMemory[0] = (demachword)_0 ; argumentMemory[1] = (demachword)_1 ; argumentMemory[2] = (demachword)_2 ; argumentMemory[3] = (demachword)_3 ;
#define DV_XMACRO_PUSH_JOB_ARGS_5(_0, _1, _2, _3, _4, ...) argumentMemory[0] = (demachword)_0 ; argumentMemory[1] = (demachword)_1 ; argumentMemory[2] = (demachword)_2 ; argumentMemory[3] = (demachword)_3 ; argumentMemory[4] = (demachword)_4 ;
#define DV_XMACRO_PUSH_JOB_ARGS_6(_0, _1, _2, _3, _4, _5, ...) argumentMemory[0] = (demachword)_0 ; argumentMemory[1] = (demachword)_1 ; argumentMemory[2] = (demachword)_2 ; argumentMemory[3] = (demachword)_3 ; argumentMemory[4] = (demachword)_4 ; argumentMemory[5] = (demachword)_5 ;
#define DV_XMACRO_XPUSH_JOB(...) DV_MACRO_CONCATE(DV_XMACRO_PUSH_JOB_ARGS_, DV_MACRO_ARGS_CNT(__VA_ARGS__))(__VA_ARGS__)
#define DV_XMACRO_PUSH_JOB(T, E, ...) \
    const deisize argumentCount = DV_MACRO_ARGS_CNT(__VA_ARGS__); \
    demachword argumentMemory[DV_MAX_JOB_QUEUE_THREAD_JOB_ARGUMENT_COUNT]; \
    DV_XMACRO_XPUSH_JOB(__VA_ARGS__) \
    m_Instance->m_Data.m_CurrentJobQueueCursor++; \
    if (m_Instance->m_Data.m_CurrentJobQueueCursor >= m_Instance->m_Data.m_RequestedThreadCount) { m_Instance->m_Data.m_CurrentJobQueueCursor = 0; } \
    auto l = [] (demachword* arg0, deusize arg1) { E->T(arg0, arg1); }; \
    m_Instance->m_Data.m_JobQueues[m_Instance->m_Data.m_CurrentJobQueueCursor].Push(l, &argumentMemory[0], argumentCount);

#define DV_XMACRO_CREATE_STRING(array, index, var, id, text) \
    engine->Create<String>((const void** const)&array[index], id, nullptr); \
    String** var = &array[index]; \
    **var = text;

#define DV_XMACRO_DEFINE_COMPONENT_LAYOUT_ARGS_2(T, _0T, _0V, ...) \
    struct T : public IComponent \
    { \
        _0T _0V; \
    }

#define DV_XMACRO_DEFINE_COMPONENT_BLANK
#define DV_XMACRO_XDEFINE_COMPONENT(T, ...) DV_MACRO_CONCATE(DV_XMACRO_DEFINE_COMPONENT_LAYOUT_ARGS_, DV_MACRO_ARGS_CNT(__VA_ARGS__))(T, __VA_ARGS__, DV_XMACRO_DEFINE_COMPONENT_BLANK)
#define DV_XMACRO_DEFINE_COMPONENT(T, ...) DV_XMACRO_XDEFINE_COMPONENT(T, __VA_ARGS__)

_DV_EOF

#endif