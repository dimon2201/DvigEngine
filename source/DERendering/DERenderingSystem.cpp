#include "../../include/DERendering.hpp"
#include "../../include/DEThirdPartyWindow.hpp"

void (*DvigEngine2::GL4::_Init)(void) = nullptr;
void (*DvigEngine2::GL4::Viewport)(deint32 x, deint32 y, deisize width, deisize height) = nullptr;
void (*DvigEngine2::GL4::Clear)(deuint32 mask) = nullptr;
void (*DvigEngine2::GL4::ClearColor)(defloat32 red, defloat32 green, defloat32 blue, defloat32 alpha) = nullptr;
void (*DvigEngine2::GL4::CreateBuffers)(deisize n, deuint32* buffers);
void (*DvigEngine2::GL4::BindBuffer)(deuint32 target, deuint32 buffer);
void (*DvigEngine2::GL4::BufferData)(deuint32 target, demachword size, const void* data, deuint32 usage);
void (*DvigEngine2::GL4::DrawElementsBaseVertex)(deuint32 mode, deisize count, deuint32 type, void* indices, deint32 baseVertex) = nullptr;

DvigEngine2::FixedSet* DvigEngine2::RenderingSystem::m_Batches = nullptr;
DvigEngine2::debool DvigEngine2::RenderingSystem::m_IsBatchRecording = DV_FALSE;
DvigEngine2::deint32 DvigEngine2::RenderingSystem::m_NextBatchUniformBufferOffset = 0;
DvigEngine2::DynamicBuffer* DvigEngine2::RenderingSystem::m_UniformBuffer = nullptr;

void DvigEngine2::GL4::LoadGL4()
{
    // Load GL4 procedures
    if (DvigEngine2::GL4::_Init == nullptr)
    {
        DvigEngine2::GL4::_Init = (void (*)())DV_TRUE;
        DvigEngine2::GL4::Viewport = (void (*)(DvigEngine2::deint32, DvigEngine2::deint32, DvigEngine2::deisize, DvigEngine2::deisize))glfwGetProcAddress( "glViewport" ); 
        DvigEngine2::GL4::Clear = (void (*)(DvigEngine2::deuint32))glfwGetProcAddress( "glClear" );
        DvigEngine2::GL4::ClearColor = (void (*)(DvigEngine2::defloat32, DvigEngine2::defloat32, DvigEngine2::defloat32, DvigEngine2::defloat32))glfwGetProcAddress( "glClearColor" );
        DvigEngine2::GL4::CreateBuffers = (void (*)(DvigEngine2::deisize, DvigEngine2::deuint32*))glfwGetProcAddress( "glCreateBuffers" );
        DvigEngine2::GL4::BindBuffer = (void (*)(DvigEngine2::deuint32, DvigEngine2::deuint32))glfwGetProcAddress( "glBindBuffer" );
        DvigEngine2::GL4::BufferData = (void (*)(DvigEngine2::deuint32, DvigEngine2::demachword, const void*, DvigEngine2::deuint32))glfwGetProcAddress( "glBufferData" );
        DvigEngine2::GL4::DrawElementsBaseVertex = (void (*)(DvigEngine2::deuint32, DvigEngine2::deisize, DvigEngine2::deuint32, void*, DvigEngine2::deint32))glfwGetProcAddress( "glDrawElementsBaseVertex" );
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
}

void DvigEngine2::RenderingSystem::BeginRender()
{
    // Create uniform buffer instance if needed
    if (m_UniformBuffer == nullptr)
    {
        DvigEngine2::Engine* engine = DvigEngine2::Engine::GetClassInstance();
        engine->Create <DvigEngine2::DynamicBuffer> ( &m_UniformBuffer, "_RenderingSystemUniformBuffer" );
        m_UniformBuffer->Init( 0, 1024 );
    }

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
    DvigEngine2::Engine::CopyMemory( &batchInstance.m_Transform, nodeTransform, sizeof(DvigEngine2::TransformComponent) );
    DvigEngine2::RenderingSystem::m_UniformBuffer->Insert(
        DvigEngine2::RenderingSystem::m_NextBatchUniformBufferOffset,
        &batchInstance,
        sizeof(DvigEngine2::BatchInstanceData)
    );
    DvigEngine2::RenderingSystem::m_NextBatchUniformBufferOffset += sizeof(DvigEngine2::BatchInstanceData);
}