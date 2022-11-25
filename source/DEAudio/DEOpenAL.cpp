#include "../../include/DEOpenAL.hpp"

ALCdevice* (*DvigEngine::AL::COpenDevice)(const ALchar* devicename) = nullptr;
ALCcontext* (*DvigEngine::AL::CCreateContext)(ALCdevice* device, const ALCint* attrlist) = nullptr;
ALCboolean (*DvigEngine::AL::CMakeContextCurrent)(ALCcontext* context) = nullptr;
void (*DvigEngine::AL::CDestroyContext)(ALCcontext* context) = nullptr;
void (*DvigEngine::AL::GenBuffers)(ALsizei n, ALuint* buffers);
void (*DvigEngine::AL::BufferData)(ALuint bid, ALenum format, const ALvoid* data, ALsizei size, ALsizei freq) = nullptr;
void (*DvigEngine::AL::GenSources)(ALsizei n, ALuint* sources) = nullptr;
void (*DvigEngine::AL::Sourcei)(ALuint sid, ALenum param, ALint value) = nullptr;
void (*DvigEngine::AL::Sourcef)(ALuint sid, ALenum param, ALfloat value) = nullptr;
void (*DvigEngine::AL::Source3f)(ALuint sid, ALenum param, ALfloat value1, ALfloat value2, ALfloat value3) = nullptr;
void (*DvigEngine::AL::DeleteBuffers)(ALsizei n, ALuint* buffers) = nullptr;
void (*DvigEngine::AL::DeleteSources)(ALsizei n, ALuint* sources) = nullptr;
void (*DvigEngine::AL::SourcePlay)(ALuint source) = nullptr;