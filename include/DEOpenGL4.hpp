#ifndef _DE_LIB_OPENGL4_H_
#define _DE_LIB_OPENGL4_H_

#include "DECore.hpp"
#include "DEPlatform.hpp"
#include "DEThirdPartyWindow.hpp"

namespace DvigEngine
{
    class GL4
    {
        DV_XMACRO_DECLARE_STATIC_CLASS(GL4)

        public:
            static void Load();

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