#include "../../include/DERendering.hpp"

DvigEngine2::FixedSet* DvigEngine2::RenderingSystem::m_Batches = nullptr;
DvigEngine2::debool DvigEngine2::RenderingSystem::m_IsBatchRecording = DV_FALSE;
DvigEngine2::deint32 DvigEngine2::RenderingSystem::m_NextBatchUniformBufferOffset = 0;
DvigEngine2::DynamicBuffer* DvigEngine2::RenderingSystem::m_UniformBuffer = nullptr;

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
    // Finish command recording
    m_IsBatchRecording = DV_FALSE;
}

void DvigEngine2::RenderingSystem::EndRender()
{
    // Unmap uniform buffer here
}

void DvigEngine2::RenderingSystem::Draw(INode* node)
{
    DvigEngine2::GeometryComponent* nodeGeometry = (DvigEngine2::GeometryComponent*)node->GetComponent<DvigEngine2::GeometryComponent>(nullptr);

    // Create new batch if needed
    if (m_IsBatchRecording == DV_TRUE)
    {
        DvigEngine2::BatchData batch;
    }
}