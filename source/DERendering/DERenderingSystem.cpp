#include "../../include/DERendering.hpp"
#include "../../include/DEThirdPartyWindow.hpp"

void (*DvigEngine::GL4::_Init)(void) = nullptr;
void (*DvigEngine::GL4::Enable)(deuint32 cap) = nullptr;
void (*DvigEngine::GL4::Disable)(deuint32 cap) = nullptr;
void (*DvigEngine::GL4::Viewport)(deint32 x, deint32 y, deisize width, deisize height) = nullptr;
void (*DvigEngine::GL4::Clear)(deuint32 mask) = nullptr;
void (*DvigEngine::GL4::ClearColor)(defloat32 red, defloat32 green, defloat32 blue, defloat32 alpha) = nullptr;
void (*DvigEngine::GL4::GenBuffers)(deisize n, deuint32* buffers) = nullptr;
void (*DvigEngine::GL4::GenVertexArrays)(deisize n, deuint32* arrays) = nullptr;
void (*DvigEngine::GL4::BindBuffer)(deuint32 target, deuint32 buffer) = nullptr;
void (*DvigEngine::GL4::BindBufferBase)(deuint32 target, deuint32 index, deuint32 buffer) = nullptr;
void (*DvigEngine::GL4::BindVertexArray)(deuint32 buffer) = nullptr;
void (*DvigEngine::GL4::BufferData)(deuint32 target, demachword size, const void* data, deuint32 usage) = nullptr;
void (*DvigEngine::GL4::BufferSubData)(deuint32 target, demachword offset, demachword size, const void* data) = nullptr;
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
DvigEngine::deuint32 (*DvigEngine::GL4::GetError)(void) = nullptr;
void (*DvigEngine::GL4::DrawElementsBaseVertex)(deuint32 mode, deisize count, deuint32 type, void* indices, deint32 baseVertex) = nullptr;

DvigEngine::INode* DvigEngine::RenderingSystem::m_Viewer = nullptr;
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

#include <libloaderapi.h>
void DvigEngine::GL4::Load()
{
    // Load GL4 procedures
    if (DvigEngine::GL4::_Init == nullptr)
    {
        DvigEngine::GL4::_Init = (void (*)())DV_TRUE;
        HMODULE module = LoadLibraryA("opengl32.dll");
        DvigEngine::GL4::CreateProgram = (DvigEngine::deuint32 (*)())glfwGetProcAddress( "glCreateProgram" );
        DvigEngine::GL4::CreateProgram();
        std::cout << DvigEngine::GL4::CreateProgram() << std::endl;
        DvigEngine::GL4::Disable = (void (*)(DvigEngine::deuint32 cap))glfwGetProcAddress( "glDisable" );
        DvigEngine::GL4::Viewport = (void (*)(DvigEngine::deint32 x, DvigEngine::deint32 y, DvigEngine::deisize width, DvigEngine::deisize height))glfwGetProcAddress( "glViewport" ); 
        DvigEngine::GL4::Clear = (void (*)(DvigEngine::deuint32 mask))glfwGetProcAddress( "glClear" );
        DvigEngine::GL4::ClearColor = (void (*)(DvigEngine::defloat32 red, DvigEngine::defloat32 green, DvigEngine::defloat32 blue, DvigEngine::defloat32 alpha))glfwGetProcAddress( "glClearColor" );
        DvigEngine::GL4::GenBuffers = (void (*)(DvigEngine::deisize n, DvigEngine::deuint32* buffers))glfwGetProcAddress( "glGenBuffers" );
        DvigEngine::GL4::GenVertexArrays = (void (*)(DvigEngine::deisize n, DvigEngine::deuint32* arrays))glfwGetProcAddress( "glGenVertexArrays" );
        DvigEngine::GL4::BindBuffer = (void (*)(DvigEngine::deuint32 target, DvigEngine::deuint32 buffer))glfwGetProcAddress( "glBindBuffer" );
        DvigEngine::GL4::BindBufferBase = (void (*)(DvigEngine::deuint32 target, DvigEngine::deuint32 index, DvigEngine::deuint32 buffer))glfwGetProcAddress( "glBindBufferBase" );
        DvigEngine::GL4::BindVertexArray = (void (*)(DvigEngine::deuint32 array))glfwGetProcAddress( "glBindVertexArray" );
        DvigEngine::GL4::BufferData = (void (*)(DvigEngine::deuint32 target, DvigEngine::demachword size, const void* data, DvigEngine::deuint32 usage))glfwGetProcAddress( "glBufferData" );
        DvigEngine::GL4::BufferSubData = (void (*)(DvigEngine::deuint32 target, DvigEngine::demachword offset, DvigEngine::demachword size, const void* data))glfwGetProcAddress( "glBufferSubData" );
        DvigEngine::GL4::CreateShader = (DvigEngine::deuint32 (*)(DvigEngine::deuint32 shaderType))glfwGetProcAddress( "glCreateShader" );
        DvigEngine::GL4::ShaderSource = (void (*)(DvigEngine::deuint32 shader, DvigEngine::deisize count, const DvigEngine::deuchar** string, const DvigEngine::deint32* length))glfwGetProcAddress( "glShaderSource" );
        DvigEngine::GL4::CompileShader = (void (*)(DvigEngine::deuint32 shader))glfwGetProcAddress( "glCompileShader" );
        DvigEngine::GL4::GetShaderiv = (void (*)(DvigEngine::deint32 shader, DvigEngine::deuint32 pname, DvigEngine::deint32* params))glfwGetProcAddress( "glGetShaderiv" );
        DvigEngine::GL4::GetShaderInfoLog = (void (*)(DvigEngine::deuint32 shader, DvigEngine::deisize maxLength, DvigEngine::deisize* length, DvigEngine::deuchar* infoLog))glfwGetProcAddress( "glGetShaderInfoLog" );
        DvigEngine::GL4::GetProgramiv = (void (*)(DvigEngine::deuint32 program, DvigEngine::deuint32 pname, DvigEngine::deint32* params))glfwGetProcAddress( "glGetProgramiv" );
        DvigEngine::GL4::DeleteShader = (void (*)(DvigEngine::deuint32 shader))glfwGetProcAddress( "glDeleteShader" );
        DvigEngine::GL4::CreateProgram = (DvigEngine::deuint32 (*)())glfwGetProcAddress( "glCreateProgram" );
        DvigEngine::GL4::AttachShader = (void (*)(DvigEngine::deuint32 program, DvigEngine::deuint32 shader))glfwGetProcAddress( "glAttachShader" );
        DvigEngine::GL4::LinkProgram = (void (*)(DvigEngine::deuint32 program))glfwGetProcAddress( "glLinkProgram" );
        DvigEngine::GL4::DetachShader = (void (*)(DvigEngine::deuint32 program, DvigEngine::deuint32 shader))glfwGetProcAddress( "glDetachShader" );
        DvigEngine::GL4::DeleteProgram = (void (*)(DvigEngine::deuint32 program))glfwGetProcAddress( "glDeleteProgram" );
        DvigEngine::GL4::EnableVertexAttribArray = (void (*)(DvigEngine::deuint32 index))glfwGetProcAddress( "glEnableVertexAttribArray" );
        DvigEngine::GL4::VertexAttribPointer = (void (*)(DvigEngine::deuint32 index, DvigEngine::deint32 size, DvigEngine::deuint32 type, bool normalized, DvigEngine::deisize stride, const void* pointer))glfwGetProcAddress( "glVertexAttribPointer" );
        DvigEngine::GL4::GetUniformBlockIndex = (DvigEngine::deuint32 (*)(DvigEngine::deuint32 program, const char* uniformBlockName))glfwGetProcAddress( "glGetUniformBlockIndex" );
        DvigEngine::GL4::UniformBlockBinding = (void (*)(DvigEngine::deuint32 program, DvigEngine::deuint32 uniformBlockIndex, DvigEngine::deuint32 uniformBlockBinding))glfwGetProcAddress( "glUniformBlockBinding" );
        DvigEngine::GL4::UseProgram = (void (*)(DvigEngine::deuint32 program))glfwGetProcAddress( "glUseProgram" );
        DvigEngine::GL4::DrawArrays = (void (*)(DvigEngine::deuint32 mode, DvigEngine::deint32 first, DvigEngine::deisize count))glfwGetProcAddress( "glDrawArrays" );
        DvigEngine::GL4::DrawElements = (void (*)(DvigEngine::deuint32 mode, DvigEngine::deisize count, DvigEngine::deuint32 type, void* indices))glfwGetProcAddress( "glDrawElements" );
        DvigEngine::GL4::DrawElementsInstanced = (void (*)(DvigEngine::deuint32 mode, DvigEngine::deisize count, DvigEngine::deuint32 type, void* indices, deisize instancecount))glfwGetProcAddress( "glDrawElementsInstanced" );
        DvigEngine::GL4::DrawElementsBaseVertex = (void (*)(DvigEngine::deuint32 mode, DvigEngine::deisize count, DvigEngine::deuint32 type, void* indices, DvigEngine::deint32 baseVertex))glfwGetProcAddress( "glDrawElementsBaseVertex" );
        DvigEngine::GL4::GetError = (DvigEngine::deuint32 (*)())glfwGetProcAddress( "glGetError" );
    }
}

void DvigEngine::RenderingSystem::Init()
{
    DvigEngine::Engine* engine = DvigEngine::Engine::GetClassInstance();

    // Create global geometry and index buffers
    // And VAO only if needed
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

void DvigEngine::RenderingSystem::BeginRender(INode* const viewer)
{
    // Bind viewer
    m_Viewer = viewer;

    // Clear uniform buffer
    m_NextBatchUniformBufferOffset = 0;

    // Clear batch set
    m_Batches->Clear();

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

void DvigEngine::RenderingSystem::EndRender()
{
    // Unmap uniform buffer here
}

void DvigEngine::RenderingSystem::Draw(INode* const node)
{
    DvigEngine::INode* viewer = DvigEngine::RenderingSystem::m_Viewer;
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
    const deint32 actualUniformDataOffset = sizeof(DvigEngine::UniformViewerData) + m_NextBatchUniformBufferOffset;
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