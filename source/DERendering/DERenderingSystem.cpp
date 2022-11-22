#include "../../include/DERendering.hpp"
#include "../../include/DEOpenGL4.hpp"

DvigEngine::RenderPassInfo* DvigEngine::RenderingSystem::m_CurRenderPass = nullptr;
DvigEngine::FixedSet* DvigEngine::RenderingSystem::m_Batches = nullptr;
DvigEngine::debool DvigEngine::RenderingSystem::m_IsBatchRecording = DV_FALSE;
DvigEngine::deint32 DvigEngine::RenderingSystem::m_NextBatchUniformBufferOffset = 0;
DvigEngine::MemoryObject* DvigEngine::RenderingSystem::m_UniformBufferMemoryObject = nullptr;
DvigEngine::deuint32 DvigEngine::RenderingSystem::m_GLUniformBuffer = DV_NULL;
DvigEngine::DynamicBuffer* DvigEngine::RenderingSystem::m_GlobalGeometryBuffer = nullptr;
DvigEngine::DynamicBuffer* DvigEngine::RenderingSystem::m_GlobalIndexBuffer = nullptr;
DvigEngine::deuint32 DvigEngine::RenderingSystem::m_GLGlobalGeometryBuffer = DV_NULL;
DvigEngine::deuint32 DvigEngine::RenderingSystem::m_GLGlobalIndexBuffer = DV_NULL;
DvigEngine::deuint32 DvigEngine::RenderingSystem::m_GLVAO = DV_NULL;

DvigEngine::deusize DvigEngine::UniformBatchInstanceData::m_GLAlignedByteWidth = sizeof(DvigEngine::UniformBatchInstanceData);

void DvigEngine::RenderingSystem::Init()
{
    DvigEngine::Engine* engine = DvigEngine::Engine::GetClassInstance();

    // Create global geometry and index buffers
    // VAO, Frame buffer only if needed
    if (DvigEngine::RenderingSystem::m_GlobalGeometryBuffer == nullptr)
    {
        engine->Create<DvigEngine::DynamicBuffer>( &RenderingSystem::m_GlobalGeometryBuffer, "_GlobalGeometryBuffer" );
        RenderingSystem::m_GlobalGeometryBuffer->Init( 0, 1024 );
        engine->Create<DvigEngine::DynamicBuffer>( &RenderingSystem::m_GlobalIndexBuffer, "_GlobalIndexBuffer" );
        RenderingSystem::m_GlobalIndexBuffer->Init( 0, 1024 );

        // GL vertex buffer
        DvigEngine::GL4::GenBuffers( 1, &RenderingSystem::m_GLGlobalGeometryBuffer );
        DvigEngine::GL4::BindBuffer( GL_ARRAY_BUFFER, RenderingSystem::m_GLGlobalGeometryBuffer );
        DvigEngine::GL4::BufferData( GL_ARRAY_BUFFER, DV_MAX_GL_DEFAULT_BUFFER_BYTE_WIDTH, nullptr, GL_DYNAMIC_DRAW );
        DvigEngine::GL4::BindBuffer( GL_ARRAY_BUFFER, 0 );

        // GL index buffer
        DvigEngine::GL4::GenBuffers( 1, &RenderingSystem::m_GLGlobalIndexBuffer );
        DvigEngine::GL4::BindBuffer( GL_ELEMENT_ARRAY_BUFFER, RenderingSystem::m_GLGlobalIndexBuffer );
        DvigEngine::GL4::BufferData( GL_ELEMENT_ARRAY_BUFFER, DV_MAX_GL_DEFAULT_BUFFER_BYTE_WIDTH, nullptr, GL_DYNAMIC_DRAW );
        DvigEngine::GL4::BindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

        // VAO
        DvigEngine::GL4::GenVertexArrays( 1, &DvigEngine::RenderingSystem::m_GLVAO );

        // Bind VAO here
        DvigEngine::GL4::BindVertexArray( DvigEngine::RenderingSystem::m_GLVAO );
        DvigEngine::GL4::BindBuffer( GL_ARRAY_BUFFER, DvigEngine::RenderingSystem::m_GLGlobalGeometryBuffer );
        DvigEngine::GL4::BindBuffer( GL_ELEMENT_ARRAY_BUFFER, DvigEngine::RenderingSystem::m_GLGlobalIndexBuffer );
        DvigEngine::GL4::EnableVertexAttribArray( 0 );
        DvigEngine::GL4::EnableVertexAttribArray( 1 );
        DvigEngine::GL4::VertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof(DvigEngine::GeometryVertex), (void*)0 );
        DvigEngine::GL4::VertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, sizeof(DvigEngine::GeometryVertex), (void*)12 );
        DvigEngine::GL4::BindVertexArray( 0 );
    }

    // Create batch set if needed
    if (m_Batches == nullptr)
    {
        engine->Create<DvigEngine::FixedSet>( &m_Batches, "_RenderingSystemBatchSet" );
        m_Batches->Init( 0, 1024, sizeof(BatchData) );
    }

    // Create uniform buffer instance if needed
    if (m_UniformBufferMemoryObject == nullptr)
    {
        // On-Heap buffer
        DvigEngine::Engine* engine = DvigEngine::Engine::GetClassInstance();
        m_UniformBufferMemoryObject = DvigEngine::Engine::Allocate( 0, DV_MAX_GL_DEFAULT_BUFFER_BYTE_WIDTH );

        // GL buffer
        DvigEngine::GL4::GenBuffers( 1, &m_GLUniformBuffer );
        DvigEngine::GL4::BindBuffer( GL_UNIFORM_BUFFER, m_GLUniformBuffer );
        DvigEngine::GL4::BufferData( GL_UNIFORM_BUFFER, DV_MAX_GL_DEFAULT_BUFFER_BYTE_WIDTH, nullptr, GL_DYNAMIC_DRAW );
        DvigEngine::GL4::BindBuffer( GL_UNIFORM_BUFFER, 0 );
    }

    // Bind vertex and index buffer
}

void DvigEngine::RenderingSystem::Viewport(deint32 x, deint32 y, deisize width, deisize height)
{
    GL4::Viewport( x, y, width, height );
}

void DvigEngine::RenderingSystem::PaintBackground(demfloat red, demfloat green, demfloat blue, demfloat alpha)
{
    GL4::Clear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    GL4::ClearColor( red, green, blue, alpha );
}

void DvigEngine::RenderingSystem::BeginRenderPass(RenderPassInfo* renderPassInfo)
{
    RenderingSystem::m_CurRenderPass = renderPassInfo;

    // Bind framebuffer
    if (RenderingSystem::m_CurRenderPass->Framebuffer != DV_NULL) {
        GL4::BindFramebuffer( GL_FRAMEBUFFER, RenderingSystem::m_CurRenderPass->Framebuffer );
    } else {
        GL4::BindFramebuffer( GL_FRAMEBUFFER, 0 );
    }

    // Clear uniform buffer
    RenderingSystem::m_NextBatchUniformBufferOffset = 0;

    // Clear batch set
    RenderingSystem::m_Batches->Clear();

    // Map uniform buffer here
}

void DvigEngine::RenderingSystem::BeginBatch()
{
    // Start command recording
    m_IsBatchRecording = DV_TRUE;
}

void DvigEngine::RenderingSystem::EndBatch()
{
    // Extract last batch
    const deusize batchSetCapacity = DvigEngine::RenderingSystem::m_Batches->GetCapacity();
    const DvigEngine::BatchData* lastBatch = DvigEngine::RenderingSystem::m_Batches->Find<const DvigEngine::BatchData*>( batchSetCapacity - 1 );

    // Extract data from batch
    DvigEngine::GeometryComponent* geometry = lastBatch->m_GeometryComponent;
    const deusize geometryIndexCount = geometry->m_IndexCount;
    DvigEngine::deuint32 shaderProgram = lastBatch->m_ShaderComponent->m_GLProgram;

    // Finish uniform buffer population
    // Make a draw call
    // Bind VAO here
    DvigEngine::GL4::BindVertexArray( DvigEngine::RenderingSystem::m_GLVAO );
    DvigEngine::deuint32 ubufferIndex = DvigEngine::GL4::GetUniformBlockIndex( shaderProgram, "UBuffer" );
    DvigEngine::GL4::UniformBlockBinding( shaderProgram, ubufferIndex, 0 );
    DvigEngine::GL4::BindBufferBase( GL_UNIFORM_BUFFER, 0, DvigEngine::RenderingSystem::m_GLUniformBuffer );
    DvigEngine::GL4::UseProgram( shaderProgram );
    DvigEngine::GL4::DrawElementsInstanced( GL_TRIANGLES, geometryIndexCount, GL_UNSIGNED_INT, nullptr, lastBatch->m_InstanceCount );
}

void DvigEngine::RenderingSystem::EndRenderPass()
{
    if (RenderingSystem::m_CurRenderPass->Framebuffer == DV_NULL)
    {
        // Render full-screen quad
        INode* postp = RenderingSystem::m_CurRenderPass->PostProcessor;
        if (postp == nullptr) { return; }
        
        PostProcessorComponent* postProcessor = postp->GetComponent<PostProcessorComponent>(nullptr);
        if (postProcessor == nullptr) { return; }
        ShaderComponent* postShader = postp->GetComponent<ShaderComponent>(nullptr);
        if (postShader == nullptr) { return; }

        // Populate shader uniforms
        GL4::UseProgram( postShader->m_GLProgram );
        GL4::ActiveTexture( GL_TEXTURE0 );
        GL4::BindTexture( GL_TEXTURE_2D, RenderingSystem::m_CurRenderPass->ColorRenderTarget );
        GL4::ActiveTexture( GL_TEXTURE1 );
        GL4::BindTexture( GL_TEXTURE_2D, RenderingSystem::m_CurRenderPass->DepthRenderTarget );
        GL4::Uniform1i( GL4::GetUniformLocation( postShader->m_GLProgram, "u_PostProcessor.m_ColorRenderTarget" ), 0 );
        GL4::Uniform1i( GL4::GetUniformLocation( postShader->m_GLProgram, "u_PostProcessor.m_DepthRenderTarget" ), 1 );

        // Issue draw call
        GL4::BindFramebuffer( GL_FRAMEBUFFER, 0 );
        GL4::DrawArrays( GL_QUADS, 0, 4 );
    }

    // Unbind framebuffer
    GL4::BindFramebuffer( GL_FRAMEBUFFER, 0 );

    RenderingSystem::m_CurRenderPass = nullptr;
}

void DvigEngine::RenderingSystem::Draw(INode* const node)
{
    if (RenderingSystem::m_CurRenderPass == nullptr) { return; }
    DvigEngine::INode* viewer = RenderingSystem::m_CurRenderPass->Viewer;
    if (viewer == nullptr) { return; }
    DvigEngine::TransformComponent* viewerTransform = viewer->GetComponent<DvigEngine::TransformComponent>(nullptr);
    DvigEngine::ViewerComponent* viewerViewer = viewer->GetComponent<DvigEngine::ViewerComponent>(nullptr);
    if (viewerTransform == nullptr || viewerViewer == nullptr) { return; }

    DvigEngine::GeometryComponent* nodeGeometry = node->GetComponent<DvigEngine::GeometryComponent>(nullptr);
    DvigEngine::TransformComponent* nodeTransform = node->GetComponent<DvigEngine::TransformComponent>(nullptr);
    DvigEngine::ShaderComponent* nodeShader = node->GetComponent<DvigEngine::ShaderComponent>(nullptr);
    if (nodeGeometry == nullptr || nodeTransform == nullptr || nodeShader == nullptr) { return; }

    // Create new batch if needed
    if (m_IsBatchRecording == DV_TRUE)
    {
        DvigEngine::BatchData batch;
        batch.m_InstanceCount = 1;
        batch.m_GeometryComponent = nodeGeometry;
        batch.m_ShaderComponent = nodeShader;
        batch.m_UniformBufferOffset = DvigEngine::RenderingSystem::m_NextBatchUniformBufferOffset;
        DvigEngine::RenderingSystem::m_Batches->Insert( &batch );

        // Only first is recorded
        m_IsBatchRecording = DV_FALSE;
    }
    else
    {
        // Get last batch and increment its
        // instance count
        const deusize batchSetCapacity = DvigEngine::RenderingSystem::m_Batches->GetCapacity();
        DvigEngine::BatchData* lastBatch = DvigEngine::RenderingSystem::m_Batches->Find<DvigEngine::BatchData*>( batchSetCapacity - 1 );
        lastBatch->m_InstanceCount += 1;
    }

    // Populate uniform buffer
    const deisize nodesDataUniformByteWidth = sizeof(UniformViewerData);
    const deint32 actualUniformDataOffset = nodesDataUniformByteWidth + RenderingSystem::m_NextBatchUniformBufferOffset;
    void* uniformBufferMemory = m_UniformBufferMemoryObject->Unwrap<void*>();
    DvigEngine::UniformViewerData uniformViewerData;
    DvigEngine::Engine::MemoryCopy( &uniformViewerData.m_WorldSpaceMatrix, &viewerTransform->m_WorldTranslationMatrix, sizeof(glm::mat4) );
    DvigEngine::Engine::MemoryCopy( &uniformViewerData.m_ViewSpaceMatrix, &viewerViewer->m_ViewSpaceMatrix, sizeof(glm::mat4) );
    DvigEngine::Engine::MemoryCopy( &uniformViewerData.m_ProjectionSpaceMatrix, &viewerViewer->m_ProjectionSpaceMatrix, sizeof(glm::mat4) );
    DvigEngine::Engine::MemoryCopy( uniformBufferMemory, &uniformViewerData, sizeof(DvigEngine::UniformViewerData) );
    uniformBufferMemory = DvigEngine::Ptr<void*>::Add( &uniformBufferMemory, actualUniformDataOffset );
    DvigEngine::UniformBatchInstanceData uniformBatchInstance;
    DvigEngine::Engine::MemoryCopy( &uniformBatchInstance.m_TransformMatrix, &nodeTransform->m_WorldSpaceMatrix, sizeof(glm::mat4) );
    
    DvigEngine::Engine::MemoryCopy( uniformBufferMemory, &uniformBatchInstance, sizeof(DvigEngine::UniformBatchInstanceData) );

    // Send uniform buffer to GPU
    DvigEngine::GL4::BindBuffer( GL_UNIFORM_BUFFER, DvigEngine::RenderingSystem::m_GLUniformBuffer );
    DvigEngine::GL4::BufferSubData( GL_UNIFORM_BUFFER, 0, sizeof(DvigEngine::UniformViewerData), (const void*)&uniformViewerData );
    DvigEngine::GL4::BufferSubData( GL_UNIFORM_BUFFER, actualUniformDataOffset, sizeof(DvigEngine::UniformBatchInstanceData), (const void*)uniformBufferMemory );
    DvigEngine::GL4::BindBuffer( GL_UNIFORM_BUFFER, 0 );

    // Increase uniform buffer offset
    DvigEngine::RenderingSystem::m_NextBatchUniformBufferOffset += DvigEngine::UniformBatchInstanceData::m_GLAlignedByteWidth;
}

void DvigEngine::RenderingSystem::ClearGeometryAndIndexBuffers()
{
    DV_ASSERT_PTR(DvigEngine::RenderingSystem::m_GlobalGeometryBuffer)

    DvigEngine::RenderingSystem::m_GlobalGeometryBuffer->Free();
    DvigEngine::RenderingSystem::m_GlobalGeometryBuffer = nullptr;
}