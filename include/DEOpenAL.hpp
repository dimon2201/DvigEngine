#ifndef _DE_LIB_OPENAL_H_
#define _DE_LIB_OPENAL_H_

#include "DECore.hpp"
#include "DEPlatform.hpp"
#include "thirdparty/DEThirdPartyAL.hpp"

namespace DvigEngine
{
    class AL
    {
        DV_XMACRO_DECLARE_STATIC_CLASS(AL)

        public:
            static void Load() {
                ALCdevice* (*COpenDevice)(const ALchar* devicename) = ALGetFunc<ALCdevice* (*)(const ALchar* devicename)>( "alcOpenDevice" );
                ALCcontext* (*CCreateContext)(ALCdevice* device, const ALCint* attrlist) = ALGetFunc<ALCcontext* (*)(ALCdevice* device, const ALCint* attrlist)>( "alcCreateContext" );
                ALCboolean* (*CMakeContextCurrent)(ALCcontext* context) = ALGetFunc<ALCboolean* (*)(ALCcontext* context)>( "alcMakeContextCurrent" );
                void (*CDestroyContext)(ALCcontext* context) = ALGetFunc<void (*)(ALCcontext* context)>( "alcDestroyContext" );
                void (*GenBuffers)(ALsizei n, ALuint* buffers) = ALGetFunc<void (*)(ALsizei n, ALuint* buffers)>( "alGenBuffers" );
                void (*BufferData)(ALuint bid, ALenum format, const ALvoid* data, ALsizei size, ALsizei freq) = ALGetFunc<void (*)(ALuint bid, ALenum format, const ALvoid* data, ALsizei size, ALsizei freq)>( "alBufferData" );
                void (*GenSources)(ALsizei n, ALuint* sources) = ALGetFunc<void (*)(ALsizei n, ALuint* sources)>( "alGenSources" );
                void (*Sourcei)(ALuint sid, ALenum param, ALint value) = ALGetFunc<void (*)(ALuint sid, ALenum param, ALint value)>( "alSourcei" );
                void (*Sourcef)(ALuint sid, ALenum param, ALfloat value) = ALGetFunc<void (*)(ALuint sid, ALenum param, ALfloat value)>( "alSourcef" );
                void (*Source3f)(ALuint sid, ALenum param, ALfloat value1, ALfloat value2, ALfloat value3) = ALGetFunc<void (*)(ALuint sid, ALenum param, ALfloat value1, ALfloat value2, ALfloat value3)>( "alSource3f" );
                void (*DeleteBuffers)(ALsizei n, ALuint* buffers) = ALGetFunc<void (*)(ALsizei n, ALuint* buffers)>( "alDeleteBuffers" );
                void (*DeleteSources)(ALsizei n, ALuint* sources) = ALGetFunc<void (*)(ALsizei n, ALuint* sources)>( "alDeleteSources" );
                void (*SourcePlay)(ALuint source) = ALGetFunc<void (*)(ALuint source)>( "alSourcePlay" );
                std::cout << COpenDevice << std::endl;
            }

            template <typename T>
            static T ALGetFunc(const char* name) {
                HMODULE hModule = LoadLibraryA( "C:\\Users\\USER100\\Documents\\GitHub\\DvigEngine\\thirdparty\\AL\\libs\\OpenAL32.dll" );
                return (T)GetProcAddress(hModule, name);
            }

        public:
            static ALCdevice* (*COpenDevice)(const ALchar* devicename);
            static ALCcontext* (*CCreateContext)(ALCdevice* device, const ALCint* attrlist);
            static ALCboolean (*CMakeContextCurrent)(ALCcontext* context);
            static void (*CDestroyContext)(ALCcontext* context);
            static void (*GenBuffers)(ALsizei n, ALuint* buffers);
            static void (*BufferData)(ALuint bid, ALenum format, const ALvoid* data, ALsizei size, ALsizei freq);
            static void (*GenSources)(ALsizei n, ALuint* sources);
            static void (*Sourcei)(ALuint sid, ALenum param, ALint value);
            static void (*Sourcef)(ALuint sid, ALenum param, ALfloat value);
            static void (*Source3f)(ALuint sid, ALenum param, ALfloat value1, ALfloat value2, ALfloat value3);
            static void (*DeleteBuffers)(ALsizei n, ALuint* buffers);
            static void (*DeleteSources)(ALsizei n, ALuint* sources);
            static void (*SourcePlay)(ALuint source);
    };
}

#endif