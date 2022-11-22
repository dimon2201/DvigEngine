#include "../../include/DEOpenGL4.hpp"

void (*DvigEngine::GL4::_Init)(void) = nullptr;
void (*DvigEngine::GL4::Enable)(deuint32 cap) = nullptr;
void (*DvigEngine::GL4::Disable)(deuint32 cap) = nullptr;
void (*DvigEngine::GL4::Viewport)(deint32 x, deint32 y, deisize width, deisize height) = nullptr;
void (*DvigEngine::GL4::Clear)(deuint32 mask) = nullptr;
void (*DvigEngine::GL4::ClearColor)(defloat32 red, defloat32 green, defloat32 blue, defloat32 alpha) = nullptr;
void (*DvigEngine::GL4::GenBuffers)(deisize n, deuint32* buffers) = nullptr;
void (*DvigEngine::GL4::GenVertexArrays)(deisize n, deuint32* arrays) = nullptr;
void (*DvigEngine::GL4::GenFramebuffers)(deisize n, deuint32* ids) = nullptr;
void (*DvigEngine::GL4::BindBuffer)(deuint32 target, deuint32 buffer) = nullptr;
void (*DvigEngine::GL4::BindBufferBase)(deuint32 target, deuint32 index, deuint32 buffer) = nullptr;
void (*DvigEngine::GL4::BindVertexArray)(deuint32 buffer) = nullptr;
void (*DvigEngine::GL4::BindFramebuffer)(deuint32 target, deuint32 framebuffer) = nullptr;
void (*DvigEngine::GL4::BufferData)(deuint32 target, demachword size, const void* data, deuint32 usage) = nullptr;
void (*DvigEngine::GL4::BufferSubData)(deuint32 target, demachword offset, demachword size, const void* data) = nullptr;
void (*DvigEngine::GL4::FramebufferTexture2D)(deuint32 target, deuint32 attachment, deuint32 textarget, deuint32 texture, deuint32 level) = nullptr;
DvigEngine::deuint32 (*DvigEngine::GL4::CreateShader)(deuint32 shaderType) = nullptr;
void (*DvigEngine::GL4::ShaderSource)(deuint32 shader, deisize count, const deuchar** string, const deint32* length) = nullptr;
void (*DvigEngine::GL4::CompileShader)(deuint32 shader) = nullptr;
void (*DvigEngine::GL4::GetShaderiv)(deint32 shader, deuint32 pname, deint32* params) = nullptr;
void (*DvigEngine::GL4::GetShaderInfoLog)(deuint32 shader, deisize maxLength, deisize* length, deuchar* infoLog) = nullptr;
void (*DvigEngine::GL4::GetProgramiv)(deuint32 program, deuint32 pname, deint32* params) = nullptr;
void (*DvigEngine::GL4::DeleteShader)(deuint32 shader) = nullptr;
DvigEngine::deuint32 (*DvigEngine::GL4::CreateProgram)(void) = nullptr;
void (*DvigEngine::GL4::AttachShader)(deuint32 program, deuint32 shader) = nullptr;
void (*DvigEngine::GL4::LinkProgram)(deuint32 program) = nullptr;
void (*DvigEngine::GL4::DetachShader)(deuint32 program, deuint32 shader) = nullptr;
void (*DvigEngine::GL4::DeleteProgram)(deuint32 program) = nullptr;
void (*DvigEngine::GL4::EnableVertexAttribArray)(deuint32 index) = nullptr;
void (*DvigEngine::GL4::VertexAttribPointer)(deuint32 index, deint32 size, deuint32 type, bool normalized, deisize stride, const void* pointer) = nullptr;
DvigEngine::deuint32 (*DvigEngine::GL4::GetUniformBlockIndex)(deuint32 program, const char* uniformBlockName) = nullptr;
void (*DvigEngine::GL4::UniformBlockBinding)(deuint32 program, deuint32 uniformBlockIndex, deuint32 uniformBlockBinding) = nullptr;
void (*DvigEngine::GL4::UseProgram)(deuint32 program) = nullptr;
void (*DvigEngine::GL4::DrawArrays)(deuint32 mode, deint32 first, deisize count) = nullptr;
void (*DvigEngine::GL4::DrawElements)(deuint32 mode, deisize count, deuint32 type, void* indices) = nullptr;
void (*DvigEngine::GL4::DrawElementsInstanced)(deuint32 mode, deisize count, deuint32 type, void* indices, deisize instancecount) = nullptr;
void (*DvigEngine::GL4::DeleteFramebuffers)(deint32 n, deuint32* framebuffers) = nullptr;
void (*DvigEngine::GL4::DrawElementsBaseVertex)(deuint32 mode, deisize count, deuint32 type, void* indices, deint32 baseVertex) = nullptr;
DvigEngine::deuint32 (*DvigEngine::GL4::GetError)(void) = nullptr;

void DvigEngine::GL4::Load()
{
    // Load GL4 procedures
    if (DvigEngine::GL4::_Init == nullptr)
    {
        DvigEngine::GL4::_Init = (void (*)())DV_TRUE;
        DvigEngine::GL4::Enable = (void (*)(deuint32 cap))GL4::GetFunc<void*>("glEnable");
        DvigEngine::GL4::Disable = (void (*)(deuint32 cap))GL4::GetFunc<void*>("glDisable");
        DvigEngine::GL4::Viewport = (void (*)(deint32 x, deint32 y, deisize width, deisize height))GL4::GetFunc<void*>("glViewport"); 
        DvigEngine::GL4::Clear = (void (*)(deuint32 mask))GL4::GetFunc<void*>("glClear");
        DvigEngine::GL4::ClearColor = (void (*)(defloat32 red, defloat32 green, defloat32 blue, defloat32 alpha))GL4::GetFunc<void*>("glClearColor");
        DvigEngine::GL4::GenBuffers = (void (*)(deisize n, deuint32* buffers))GL4::GetFunc<void*>("glGenBuffers");
        DvigEngine::GL4::GenVertexArrays = (void (*)(deisize n, deuint32* arrays))GL4::GetFunc<void*>("glGenVertexArrays");
        DvigEngine::GL4::GenFramebuffers = (void (*)(deisize n, deuint32* ids))GL4::GetFunc<void*>("glGenFramebuffers");
        DvigEngine::GL4::BindBuffer = (void (*)(deuint32 target, deuint32 buffer))GL4::GetFunc<void*>("glBindBuffer");
        DvigEngine::GL4::BindBufferBase = (void (*)(deuint32 target, deuint32 index, deuint32 buffer))GL4::GetFunc<void*>("glBindBufferBase");
        DvigEngine::GL4::BindVertexArray = (void (*)(deuint32 array))GL4::GetFunc<void*>("glBindVertexArray");
        DvigEngine::GL4::BindFramebuffer = (void (*)(deuint32 target, deuint32 framebuffer))GL4::GetFunc<void*>("glBindFramebuffer");
        DvigEngine::GL4::BufferData = (void (*)(deuint32 target, demachword size, const void* data, deuint32 usage))GL4::GetFunc<void*>("glBufferData");
        DvigEngine::GL4::BufferSubData = (void (*)(deuint32 target, demachword offset, demachword size, const void* data))GL4::GetFunc<void*>("glBufferSubData");
        DvigEngine::GL4::FramebufferTexture2D = (void (*)(deuint32 target, deuint32 attachment, deuint32 textarget, deuint32 texture, deuint32 level))GL4::GetFunc<void*>("glFramebufferTexture2D");
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
        DvigEngine::GL4::DrawArrays = (void (*)(deuint32 mode, deint32 first, deisize count))GL4::GetFunc<void*>("glDrawArrays");
        DvigEngine::GL4::DrawElements = (void (*)(deuint32 mode, deisize count, deuint32 type, void* indices))GL4::GetFunc<void*>("glDrawElements");
        DvigEngine::GL4::DrawElementsInstanced = (void (*)(deuint32 mode, deisize count, deuint32 type, void* indices, deisize instancecount))GL4::GetFunc<void*>("glDrawElementsInstanced");
        DvigEngine::GL4::DrawElementsBaseVertex = (void (*)(deuint32 mode, deisize count, deuint32 type, void* indices, deint32 baseVertex))GL4::GetFunc<void*>("glDrawElementsBaseVertex");
        DvigEngine::GL4::DeleteFramebuffers = (void (*)(deint32 n, deuint32* framebuffers))GL4::GetFunc<void*>("glDeleteFramebuffers");
        DvigEngine::GL4::GetError = (deuint32 (*)())GL4::GetFunc<void*>("glGetError");
    }
}