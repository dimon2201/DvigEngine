#ifndef _DE_LIB_OPENGL4_H_
#define _DE_LIB_OPENGL4_H_

#include "DECore.hpp"
#include "DEPlatform.hpp"
#include "thirdparty/DEThirdPartyWindow.hpp"

namespace DvigEngine
{
    class GL4
    {
        DV_XMACRO_DECLARE_STATIC_CLASS(GL4)

        public:
            static void Load() {
                DvigEngine::GL4::_Init = (void (*)())DV_TRUE;
                DvigEngine::GL4::Enable = (void (*)(deuint32 cap))GL4::GetFunc<void*>("glEnable");
                DvigEngine::GL4::Disable = (void (*)(deuint32 cap))GL4::GetFunc<void*>("glDisable");
                DvigEngine::GL4::Viewport = (void (*)(deint32 x, deint32 y, deisize width, deisize height))GL4::GetFunc<void*>("glViewport"); 
                DvigEngine::GL4::Clear = (void (*)(deuint32 mask))GL4::GetFunc<void*>("glClear");
                DvigEngine::GL4::ClearColor = (void (*)(defloat32 red, defloat32 green, defloat32 blue, defloat32 alpha))GL4::GetFunc<void*>("glClearColor");
                DvigEngine::GL4::GenBuffers = (void (*)(deisize n, deuint32* buffers))GL4::GetFunc<void*>("glGenBuffers");
                DvigEngine::GL4::GenVertexArrays = (void (*)(deisize n, deuint32* arrays))GL4::GetFunc<void*>("glGenVertexArrays");
                DvigEngine::GL4::GenFramebuffers = (void (*)(deisize n, deuint32* ids))GL4::GetFunc<void*>("glGenFramebuffers");
                DvigEngine::GL4::GenTextures = (void (*)(deisize n, deuint32* textures))GL4::GetFunc<void*>("glGenTextures");
                DvigEngine::GL4::BindBuffer = (void (*)(deuint32 target, deuint32 buffer))GL4::GetFunc<void*>("glBindBuffer");
                DvigEngine::GL4::BindBufferBase = (void (*)(deuint32 target, deuint32 index, deuint32 buffer))GL4::GetFunc<void*>("glBindBufferBase");
                DvigEngine::GL4::BindVertexArray = (void (*)(deuint32 array))GL4::GetFunc<void*>("glBindVertexArray");
                DvigEngine::GL4::BindFramebuffer = (void (*)(deuint32 target, deuint32 framebuffer))GL4::GetFunc<void*>("glBindFramebuffer");
                DvigEngine::GL4::BindTexture = (void (*)(deuint32 target, deuint32 texture))GL4::GetFunc<void*>("glBindTexture");
                DvigEngine::GL4::BufferData = (void (*)(deuint32 target, demachword size, const void* data, deuint32 usage))GL4::GetFunc<void*>("glBufferData");
                DvigEngine::GL4::BufferSubData = (void (*)(deuint32 target, demachword offset, demachword size, const void* data))GL4::GetFunc<void*>("glBufferSubData");
                DvigEngine::GL4::FramebufferTexture2D = (void (*)(deuint32 target, deuint32 attachment, deuint32 textarget, deuint32 texture, deuint32 level))GL4::GetFunc<void*>("glFramebufferTexture2D");
                DvigEngine::GL4::TexImage2D = (void (*)(deuint32 target, deuint32 level, deuint32 internalformat, deisize width, deisize height, deint32 border, deuint32 format, deuint32 type, const void* data))GL4::GetFunc<void*>("glTexImage2D");
                DvigEngine::GL4::TexImage3D = (void (*)(deuint32 target, deuint32 level, deuint32 internalformat, deisize width, deisize height, deisize depth, deint32 border, deuint32 format, deuint32 type, const void* data))GL4::GetFunc<void*>("glTexImage3D");
                DvigEngine::GL4::TexStorage3D = (void (*)(deuint32 target, deisize levels, deuint32 internalformat, deisize width, deisize height, deisize depth))GL4::GetFunc<void*>("glTexStorage3D");
                DvigEngine::GL4::TexParameteri = (void (*)(deuint32 target, deuint32 pname, deint32 param))GL4::GetFunc<void*>("glTexParameteri");
                DvigEngine::GL4::TexSubImage3D = (void (*)(deuint32 target, deint32 level, deint32 xoffset, deint32 yoffset, deint32 zoffset, deisize width, deisize height, deisize depth, deuint32 format, deuint32 type, const void* pixels))GL4::GetFunc<void*>("glTexSubImage3D");
                DvigEngine::GL4::ActiveTexture = (void (*)(deuint32 texture))GL4::GetFunc<void*>("glActiveTexture");
                DvigEngine::GL4::CreateShader = (DvigEngine::deuint32 (*)(deuint32 shaderType))GL4::GetFunc<void*>("glCreateShader");
                DvigEngine::GL4::ShaderSource = (void (*)(deuint32 shader, deisize count, const deuchar** string, const deint32* length))GL4::GetFunc<void*>("glShaderSource");
                DvigEngine::GL4::CompileShader = (void (*)(deuint32 shader))GL4::GetFunc<void*>("glCompileShader");
                DvigEngine::GL4::GetShaderiv = (void (*)(deint32 shader, deuint32 pname, deint32* params))GL4::GetFunc<void*>("glGetShaderiv");
                DvigEngine::GL4::GetShaderInfoLog = (void (*)(deuint32 shader, deisize maxLength, deisize* length, deuchar* infoLog))GL4::GetFunc<void*>("glGetShaderInfoLog");
                DvigEngine::GL4::GetProgramiv = (void (*)(deuint32 program, deuint32 pname, deint32* params))GL4::GetFunc<void*>("glGetProgramiv");
                DvigEngine::GL4::DeleteShader = (void (*)(deuint32 shader))GL4::GetFunc<void*>("glDeleteShader");
                DvigEngine::GL4::CreateProgram = (deuint32 (*)())GL4::GetFunc<void*>("glCreateProgram");
                DvigEngine::GL4::AttachShader = (void (*)(deuint32 program, deuint32 shader))GL4::GetFunc<void*>("glAttachShader");
                DvigEngine::GL4::LinkProgram = (void (*)(deuint32 program))GL4::GetFunc<void*>("glLinkProgram");
                DvigEngine::GL4::DetachShader = (void (*)(deuint32 program, deuint32 shader))GL4::GetFunc<void*>("glDetachShader");
                DvigEngine::GL4::DeleteProgram = (void (*)(deuint32 program))GL4::GetFunc<void*>("glDeleteProgram");
                DvigEngine::GL4::EnableVertexAttribArray = (void (*)(deuint32 index))GL4::GetFunc<void*>("glEnableVertexAttribArray");
                DvigEngine::GL4::VertexAttribPointer = (void (*)(deuint32 index, deint32 size, deuint32 type, bool normalized, deisize stride, const void* pointer))GL4::GetFunc<void*>("glVertexAttribPointer");
                DvigEngine::GL4::GetUniformBlockIndex = (DvigEngine::deuint32 (*)(deuint32 program, const char* uniformBlockName))GL4::GetFunc<void*>("glGetUniformBlockIndex");
                DvigEngine::GL4::UniformBlockBinding = (void (*)(deuint32 program, deuint32 uniformBlockIndex, deuint32 uniformBlockBinding))GL4::GetFunc<void*>("glUniformBlockBinding");
                DvigEngine::GL4::UseProgram = (void (*)(deuint32 program))GL4::GetFunc<void*>("glUseProgram");
                DvigEngine::GL4::GetUniformLocation = (deint32 (*)(deuint32 program, const char* name))GL4::GetFunc<void*>("glGetUniformLocation");
                DvigEngine::GL4::Uniform1i = (void (*)(deint32 location, deint32 v0))GL4::GetFunc<void*>("glUniform1i");
                DvigEngine::GL4::DrawArrays = (void (*)(deuint32 mode, deint32 first, deisize count))GL4::GetFunc<void*>("glDrawArrays");
                DvigEngine::GL4::DrawElements = (void (*)(deuint32 mode, deisize count, deuint32 type, void* indices))GL4::GetFunc<void*>("glDrawElements");
                DvigEngine::GL4::DrawElementsInstanced = (void (*)(deuint32 mode, deisize count, deuint32 type, void* indices, deisize instancecount))GL4::GetFunc<void*>("glDrawElementsInstanced");
                DvigEngine::GL4::DrawElementsBaseVertex = (void (*)(deuint32 mode, deisize count, deuint32 type, void* indices, deint32 baseVertex))GL4::GetFunc<void*>("glDrawElementsBaseVertex");
                DvigEngine::GL4::DeleteBuffers = (void (*)(deint32 n, const deuint32* buffers))GL4::GetFunc<void*>("glDeleteBuffers");
                DvigEngine::GL4::DeleteFramebuffers = (void (*)(deint32 n, const deuint32* framebuffers))GL4::GetFunc<void*>("glDeleteFramebuffers");
                DvigEngine::GL4::DeleteTextures = (void (*)(deint32 n, const deuint32* textures))GL4::GetFunc<void*>("glDeleteTextures");
                DvigEngine::GL4::GetError = (deuint32 (*)())GL4::GetFunc<void*>("glGetError");
            }

            template <typename T>
            static T GetFunc(const char* name) {
                void* proc = (void*)glfwGetProcAddress( &name[0] );
                if( proc == nullptr ||
                   (proc == (void*)0x1) || (proc == (void*)0x2) || (proc == (void*)0x3) ||
                   (proc == (void*)-1) )
                {
                    HMODULE hModule = LoadLibraryA( "opengl32.dll" );
                    proc = (void*)GetProcAddress(hModule, name);
                }

                return (T)proc;
            }

        public:
            static void (*_Init)(void);
            static void (*Enable)(deuint32 cap);
            static void (*Disable)(deuint32 cap);
            static void (*Viewport)(deint32 x, deint32 y, deisize width, deisize height);
            static void (*Clear)(deuint32 mask);
            static void (*ClearColor)(defloat32 red, defloat32 green, defloat32 blue, defloat32 alpha);
            static void (*GenBuffers)(deisize n, deuint32* buffers);
            static void (*GenVertexArrays)(deisize n, deuint32* arrays);
            static void (*GenFramebuffers)(deisize n, deuint32* ids);
            static void (*GenTextures)(deisize n, deuint32* textures);
            static void (*BindBuffer)(deuint32 target, deuint32 buffer);
            static void (*BindBufferBase)(deuint32 target, deuint32 index, deuint32 buffer);
            static void (*BindVertexArray)(deuint32 buffer);
            static void (*BindFramebuffer)(deuint32 target, deuint32 framebuffer);
            static void (*BindTexture)(deuint32 target, deuint32 texture);
            static void (*BufferData)(deuint32 target, demachword size, const void* data, deuint32 usage);
            static void (*BufferSubData)(deuint32 target, demachword offset, demachword size, const void* data);
            static void (*FramebufferTexture2D)(deuint32 target, deuint32 attachment, deuint32 textarget, deuint32 texture, deuint32 level);
            static void (*TexImage2D)(deuint32 target, deuint32 level, deuint32 internalformat, deisize width, deisize height, deint32 border, deuint32 format, deuint32 type, const void* data);
            static void (*TexImage3D)(deuint32 target, deuint32 level, deuint32 internalformat, deisize width, deisize height, deisize depth, deint32 border, deuint32 format, deuint32 type, const void* data);
            static void (*TexStorage3D)(deuint32 target, deisize levels, deuint32 internalformat, deisize width, deisize height, deisize depth);
            static void (*TexParameteri)(deuint32 target, deuint32 pname, deint32 param);
            static void (*TexSubImage3D)(deuint32 target, deint32 level, deint32 xoffset, deint32 yoffset, deint32 zoffset, deisize width, deisize height, deisize depth, deuint32 format, deuint32 type, const void* pixels);
            static void (*ActiveTexture)(deuint32 texture);
            static deuint32 (*CreateShader)(deuint32 shaderType);
            static void (*ShaderSource)(deuint32 shader, deisize count, const deuchar** string, const deint32* length);
            static void (*CompileShader)(deuint32 shader);
            static void (*GetShaderiv)(deint32 shader, deuint32 pname, deint32* params);
            static void (*GetShaderInfoLog)(deuint32 shader, deisize maxLength, deisize* length, deuchar* infoLog);
            static void (*GetProgramiv)(deuint32 program, deuint32 pname, deint32* params);
            static void (*DeleteShader)(deuint32 shader);
            static deuint32 (*CreateProgram)(void);
            static void (*AttachShader)(deuint32 program, deuint32 shader);
            static void (*LinkProgram)(deuint32 program);
            static void (*DetachShader)(deuint32 program, deuint32 shader);
            static void (*DeleteProgram)(deuint32 program);
            static void (*EnableVertexAttribArray)(deuint32 index);
            static void (*VertexAttribPointer)(deuint32 index, deint32 size, deuint32 type, bool normalized, deisize stride, const void* pointer);
            static deuint32 (*GetUniformBlockIndex)(deuint32 program, const char* uniformBlockName);
            static void (*UniformBlockBinding)(deuint32 program, deuint32 uniformBlockIndex, deuint32 uniformBlockBinding);
            static void (*UseProgram)(deuint32 program);
            static deint32 (*GetUniformLocation)(deuint32 program, const char* name);
            static void (*Uniform1i)(deint32 location, deint32 v0);
            static void (*DrawArrays)(deuint32 mode, deint32 first, deisize count);
            static void (*DrawElements)(deuint32 mode, deisize count, deuint32 type, void* indices);
            static void (*DrawElementsInstanced)(deuint32 mode, deisize count, deuint32 type, void* indices, deisize instancecount);
            static void (*DrawElementsBaseVertex)(deuint32 mode, deisize count, deuint32 type, void* indices, deint32 baseVertex);
            static void (*DeleteBuffers)(deint32 n, const deuint32* buffers);
            static void (*DeleteFramebuffers)(deint32 n, const deuint32* framebuffers);
            static void (*DeleteTextures)(deint32 n, const deuint32* textures);
            static deuint32 (*GetError)();
    };
}

#endif