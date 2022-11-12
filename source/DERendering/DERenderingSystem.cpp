#include "../../include/DERendering.hpp"

DV_MACRO_DEFINE_SINGLETON(DvigEngine2::RenderingSystem)

void DvigEngine2::RenderingSystem::Init()
{
    m_Instance = DvigEngine2::RenderingSystem::GetClassInstance();

    // Create batch set if needed
    if (m_Instance != nullptr && m_Instance->m_Batches == nullptr)
    {
        DvigEngine2::Engine* engine = DvigEngine2::Engine::GetClassInstance();
        engine->Create<DvigEngine2::FixedSet>( &m_Instance->m_Batches, "_RenderingSystemBatchSet" );
        m_Instance->m_Batches->Init( 0, 1024, sizeof(GeometryBatch*) );
    }

    m_Instance->m_IsBatchRecording = DV_FALSE;
}

void DvigEngine2::RenderingSystem::BeginRender()
{
    DV_ASSERT_PTR(m_Instance)

    // Create uniform buffer instance if needed
    if (m_Instance->m_UniformBuffer == nullptr)
    {
        DvigEngine2::Engine* engine = DvigEngine2::Engine::GetClassInstance();
        engine->Create <DvigEngine2::DynamicBuffer> ( &m_Instance->m_UniformBuffer, "_RenderingSystemUniformBuffer" );
        m_Instance->m_UniformBuffer->Init( 0, 1024 );
    }

    // Clear uniform buffer
    m_Instance->m_UniformBuffer->Clear();

    // Map uniform buffer here
}

void DvigEngine2::RenderingSystem::BeginBatch()
{
    
}

void DvigEngine2::RenderingSystem::EndBatch()
{
    
}

void DvigEngine2::RenderingSystem::EndRender()
{
    // Unmap uniform buffer here
}