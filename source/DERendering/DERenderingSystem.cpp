#include "../../include/DERendering.hpp"
#include "../../include/DEThirdPartyWindow.hpp"

void (*DvigEngine2::GL4::_Init)(void) = nullptr;
void (*DvigEngine2::GL4::Viewport)(deint32 x, deint32 y, deisize width, deisize height) = nullptr;
void (*DvigEngine2::GL4::Clear)(deuint32 mask) = nullptr;
void (*DvigEngine2::GL4::ClearColor)(defloat32 red, defloat32 green, defloat32 blue, defloat32 alpha) = nullptr;
void (*DvigEngine2::GL4::GenBuffers)(deisize n, deuint32* buffers) = nullptr;
void (*DvigEngine2::GL4::BindBuffer)(deuint32 target, deuint32 buffer) = nullptr;
void (*DvigEngine2::GL4::BufferData)(deuint32 target, demachword size, const void* data, deuint32 usage) = nullptr;
void (*DvigEngine2::GL4::CreateShader)(deuint32 shaderType) = nullptr;
void (*DvigEngine2::GL4::ShaderSource)(deuint32 shader, deisize count, const deuchar** string, const deint32* length) = nullptr;
void (*DvigEngine2::GL4::CompileShader)(deuint32 shader) = nullptr;
void (*DvigEngine2::GL4::GetShaderiv)(deint32 shader, deuint32 pname, deint32* params) = nullptr;
void (*DvigEngine2::GL4::GetShaderInfoLog)(deuint32 shader, deisize maxLength, deisize* length, deuchar* infoLog) = nullptr;
void (*DvigEngine2::GL4::DeleteShader)(deuint32 shader) = nullptr;
void (*DvigEngine2::GL4::CreateProgram)(void) = nullptr;
void (*DvigEngine2::GL4::AttachShader)(deuint32 program, deuint32 shader) = nullptr;
void (*DvigEngine2::GL4::LinkProgram)(deuint32 program) = nullptr;
void (*DvigEngine2::GL4::DetachShader)(deuint32 program, deuint32 shader) = nullptr;
void (*DvigEngine2::GL4::UseProgram)(deuint32 program) = nullptr;
void (*DvigEngine2::GL4::DrawElements)(deuint32 mode, deisize count, deuint32 type, void* indices) = nullptr;
void (*DvigEngine2::GL4::DrawElementsBaseVertex)(deuint32 mode, deisize count, deuint32 type, void* indices, deint32 baseVertex) = nullptr;

DvigEngine2::FixedSet* DvigEngine2::RenderingSystem::m_Batches = nullptr;
DvigEngine2::debool DvigEngine2::RenderingSystem::m_IsBatchRecording = DV_FALSE;
DvigEngine2::deint32 DvigEngine2::RenderingSystem::m_NextBatchUniformBufferOffset = 0;
DvigEngine2::DynamicBuffer* DvigEngine2::RenderingSystem::m_UniformBuffer = nullptr;
DvigEngine2::deuint32 DvigEngine2::RenderingSystem::m_GLUniformBuffer = DV_NULL;

void DvigEngine2::GL4::Load()
{
    // Load GL4 procedures
    if (DvigEngine2::GL4::_Init == nullptr)
    {
        DvigEngine2::GL4::_Init = (void (*)())DV_TRUE;
        DvigEngine2::GL4::Viewport = (void (*)(DvigEngine2::deint32 x, DvigEngine2::deint32 y, DvigEngine2::deisize width, DvigEngine2::deisize height))glfwGetProcAddress( "glViewport" ); 
        DvigEngine2::GL4::Clear = (void (*)(DvigEngine2::deuint32 mask))glfwGetProcAddress( "glClear" );
        DvigEngine2::GL4::ClearColor = (void (*)(DvigEngine2::defloat32 red, DvigEngine2::defloat32 green, DvigEngine2::defloat32 blue, DvigEngine2::defloat32 alpha))glfwGetProcAddress( "glClearColor" );
        DvigEngine2::GL4::GenBuffers = (void (*)(DvigEngine2::deisize n, DvigEngine2::deuint32* buffers))glfwGetProcAddress( "glGenBuffers" );
        DvigEngine2::GL4::BindBuffer = (void (*)(DvigEngine2::deuint32 target, DvigEngine2::deuint32 buffer))glfwGetProcAddress( "glBindBuffer" );
        DvigEngine2::GL4::BufferData = (void (*)(DvigEngine2::deuint32 target, DvigEngine2::demachword size, const void* data, DvigEngine2::deuint32 usage))glfwGetProcAddress( "glBufferData" );
        DvigEngine2::GL4::CreateShader = (void (*)(DvigEngine2::deuint32 shaderType))glfwGetProcAddress( "glCreateShader" );
        DvigEngine2::GL4::ShaderSource = (void (*)(DvigEngine2::deuint32 shader, DvigEngine2::deisize count, const DvigEngine2::deuchar** string, const DvigEngine2::deint32* length))glfwGetProcAddress( "glShaderSource" );
        DvigEngine2::GL4::CompileShader = (void (*)(DvigEngine2::deuint32 shader))glfwGetProcAddress( "glCompileShader" );
        DvigEngine2::GL4::GetShaderiv = (void (*)(DvigEngine2::deint32 shader, DvigEngine2::deuint32 pname, DvigEngine2::deint32* params))glfwGetProcAddress( "glGetShaderiv" );
        DvigEngine2::GL4::GetShaderInfoLog = (void (*)(DvigEngine2::deuint32 shader, DvigEngine2::deisize maxLength, DvigEngine2::deisize* length, DvigEngine2::deuchar* infoLog))glfwGetProcAddress( "glGetShaderInfoLog" );
        DvigEngine2::GL4::DeleteShader = (void (*)(DvigEngine2::deuint32 shader))glfwGetProcAddress( "glDeleteShader" );
        DvigEngine2::GL4::CreateProgram = (void (*)())glfwGetProcAddress( "glCreateProgram" );
        DvigEngine2::GL4::AttachShader = (void (*)(DvigEngine2::deuint32 program, DvigEngine2::deuint32 shader))glfwGetProcAddress( "glAttachShader" );
        DvigEngine2::GL4::LinkProgram = (void (*)(DvigEngine2::deuint32 program))glfwGetProcAddress( "glLinkProgram" );
        DvigEngine2::GL4::DetachShader = (void (*)(DvigEngine2::deuint32 program, DvigEngine2::deuint32 shader))glfwGetProcAddress( "glDetachShader" );
        DvigEngine2::GL4::UseProgram = (void (*)(DvigEngine2::deuint32 program))glfwGetProcAddress( "glUseProgram" );
        DvigEngine2::GL4::DrawElements = (void (*)(DvigEngine2::deuint32 mode, DvigEngine2::deisize count, DvigEngine2::deuint32 type, void* indices))glfwGetProcAddress( "glDrawElements" );
        DvigEngine2::GL4::DrawElementsBaseVertex = (void (*)(DvigEngine2::deuint32 mode, DvigEngine2::deisize count, DvigEngine2::deuint32 type, void* indices, DvigEngine2::deint32 baseVertex))glfwGetProcAddress( "glDrawElementsBaseVertex" );
    }
}

void DvigEngine2::RenderingSystem::Init()
{
    // Create batch set if needed
    if (m_Batches == nullptr)
    {
        DvigEngine2::Engine* engine = DvigEngine2::Engine::GetClassInstance();
        engine->Create<DvigEngine2::FixedSet>( &m_Batches, "_RenderingSystemBatchSet" );
        m_Batches->Init( 0, 1024, sizeof(BatchData) );
    }

    // Create uniform buffer instance if needed
    if (m_UniformBuffer == nullptr)
    {
        // On-Heap buffer
        DvigEngine2::Engine* engine = DvigEngine2::Engine::GetClassInstance();
        engine->Create <DvigEngine2::DynamicBuffer> ( &m_UniformBuffer, "_RenderingSystemUniformBuffer" );
        m_UniformBuffer->Init( 0, 1024 );

        // GL buffer
        DvigEngine2::GL4::GenBuffers( 1, &m_GLUniformBuffer );
        DvigEngine2::GL4::BindBuffer( GL_UNIFORM_BUFFER, m_GLUniformBuffer );
        DvigEngine2::GL4::BufferData( GL_UNIFORM_BUFFER, 24 * DV_MEMORY_MiB, nullptr, GL_DYNAMIC_DRAW );
        DvigEngine2::GL4::BindBuffer( GL_UNIFORM_BUFFER, 0 );
    }
}

void DvigEngine2::RenderingSystem::BeginRender()
{
    // Clear uniform buffer
    m_NextBatchUniformBufferOffset = 0;
    m_UniformBuffer->Clear();

    // Clear batch set
    m_Batches->Clear();

    // Map uniform buffer here
}

void DvigEngine2::RenderingSystem::BeginBatch()
{
    // Start command recording
    m_IsBatchRecording = DV_TRUE;
}

void DvigEngine2::RenderingSystem::EndBatch()
{
    // Finish uniform buffer population
    // Make a draw call
}

void DvigEngine2::RenderingSystem::EndRender()
{
    // Unmap uniform buffer here
}

void DvigEngine2::RenderingSystem::Draw(INode* node)
{
    DvigEngine2::GeometryComponent* nodeGeometry = (DvigEngine2::GeometryComponent*)node->GetComponent<DvigEngine2::GeometryComponent>(nullptr);
    DvigEngine2::TransformComponent* nodeTransform = (DvigEngine2::TransformComponent*)node->GetComponent<DvigEngine2::TransformComponent>(nullptr);
    
    if (nodeGeometry == nullptr || nodeTransform == nullptr) { return; }

    // Create new batch if needed
    if (m_IsBatchRecording == DV_TRUE)
    {
        DvigEngine2::BatchData batch;
        batch.m_GeometryComponent = nodeGeometry;
        batch.m_UniformBufferOffset = DvigEngine2::RenderingSystem::m_NextBatchUniformBufferOffset;
        DvigEngine2::RenderingSystem::m_Batches->Insert( &batch );

        // Only first is recorded
        m_IsBatchRecording = DV_FALSE;
    }

    // Populate uniform buffer
    DvigEngine2::BatchInstanceData batchInstance;
    DvigEngine2::Engine::MemoryCopy( &batchInstance.m_Transform, nodeTransform, sizeof(DvigEngine2::TransformComponent) );
    DvigEngine2::RenderingSystem::m_UniformBuffer->Insert(
        DvigEngine2::RenderingSystem::m_NextBatchUniformBufferOffset,
        &batchInstance,
        sizeof(DvigEngine2::BatchInstanceData)
    );
    DvigEngine2::RenderingSystem::m_NextBatchUniformBufferOffset += sizeof(DvigEngine2::BatchInstanceData);
}