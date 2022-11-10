#include "../../include/DERendering.hpp"

DV_MACRO_DEFINE_SINGLETON(DvigEngine2::RenderingSystem)

void DvigEngine2::RenderingSystem::Init()
{
    // Create batch set if needed
    if (m_Instance != nullptr && m_Instance->m_Batches == nullptr)
    {
        DvigEngine2::Engine* engine = m_Instance->GetEngine();
        m_Instance->m_Batches = engine->Create<DvigEngine2::FixedSet>( "_BatchSet" );
        m_Instance->m_Batches->Init( 0, 1024, sizeof(GeometryBatch*) );
    }
}

void DvigEngine2::RenderingSystem::BeginRender()
{
    
}

void DvigEngine2::RenderingSystem::BeginBatch()
{
    
}

void DvigEngine2::RenderingSystem::EndBatch()
{
    
}

void DvigEngine2::RenderingSystem::EndRender()
{

}