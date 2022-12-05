#include "../../include/dvigengine/DED3D11.hpp"
#include "../../include/dvigengine/DERendering.hpp"

ID3D11Buffer* DvigEngine::RenderingSystem::m_GlobalVertexBuffer = nullptr;
ID3D11Buffer* DvigEngine::RenderingSystem::m_GlobalIndexBuffer = nullptr;
ID3D11Buffer* DvigEngine::RenderingSystem::m_GlobalUniformBuffer = nullptr;
ID3D11Buffer* DvigEngine::RenderingSystem::m_GlobalConstantBuffer = nullptr;
ID3D11ShaderResourceView* DvigEngine::RenderingSystem::m_GlobalUniformBufferView = nullptr;
DvigEngine::sint32 DvigEngine::RenderingSystem::m_GlobalVertexBufferOffset = 0;
DvigEngine::sint32 DvigEngine::RenderingSystem::m_GlobalIndexBufferOffset = 0;
DvigEngine::RenderPassInfo* DvigEngine::RenderingSystem::m_CurRenderPass = nullptr;
DvigEngine::Collection* DvigEngine::RenderingSystem::m_Batches = nullptr;
DvigEngine::uint32 DvigEngine::RenderingSystem::m_GlobalInstanceCount = 0;
ID3D11DepthStencilState* DvigEngine::RenderingSystem::m_DepthStencilState = nullptr;

void DvigEngine::RenderingSystem::Init()
{
    if (RenderingSystem::m_GlobalVertexBuffer == nullptr)
    {
        RenderingSystem::m_GlobalVertexBuffer = D3D11::CreateBuffer( D3D11_USAGE_DYNAMIC, D3D11_BIND_VERTEX_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0, nullptr, DVIG_MAX_GFX_BUFFER_BYTE_WIDTH );
        RenderingSystem::m_GlobalIndexBuffer = D3D11::CreateBuffer( D3D11_USAGE_DYNAMIC, D3D11_BIND_INDEX_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0, nullptr, DVIG_MAX_GFX_BUFFER_BYTE_WIDTH );
        RenderingSystem::m_GlobalUniformBuffer = D3D11::CreateBuffer( D3D11_USAGE_DYNAMIC, D3D11_BIND_SHADER_RESOURCE, D3D11_CPU_ACCESS_WRITE, D3D11_RESOURCE_MISC_BUFFER_STRUCTURED, sizeof(mfloat), nullptr, DVIG_MAX_GFX_BUFFER_BYTE_WIDTH );
        RenderingSystem::m_GlobalConstantBuffer = D3D11::CreateBuffer( D3D11_USAGE_DEFAULT, D3D11_BIND_CONSTANT_BUFFER, 0, 0, 0, nullptr, 2 * 64 );
        RenderingSystem::m_GlobalUniformBufferView = D3D11::CreateShaderResourceView( RenderingSystem::m_GlobalUniformBuffer, DVIG_MAX_GFX_BUFFER_BYTE_WIDTH );
        RenderingSystem::m_Batches = Engine::GetClassInstance()->Create<Collection>("_EngineRenderingSystemBatchCollection");
        RenderingSystem::m_Batches->Init( sizeof(RenderBatchInfo), 0 );
    }

    RenderingSystem::m_DepthStencilState = D3D11::CreateDepthStencilState();
}

void DvigEngine::RenderingSystem::Free()
{
    RenderingSystem::m_Batches->Free();
}

void DvigEngine::RenderingSystem::Viewport(mfloat x, mfloat y, mfloat width, mfloat height)
{
    D3D11_VIEWPORT viewports[1];
    viewports[0].TopLeftX = x;
    viewports[0].TopLeftY = y;
    viewports[0].Width = width;
    viewports[0].Height = height;
    viewports[0].MinDepth = 0.0f;
    viewports[0].MaxDepth = 1.0f;
    D3D11::m_DeviceContext->RSSetViewports( 1, &viewports[0] );
}

void DvigEngine::RenderingSystem::ClearBackground(mfloat red, mfloat green, mfloat blue, mfloat alpha)
{
    const FLOAT color[] = { red, green, blue, alpha };
    D3D11::m_DeviceContext->ClearRenderTargetView( RenderingSystem::m_CurRenderPass->OutputRenderTargets->GetD3DRenderTargetView(), &color[0] );
    D3D11::m_DeviceContext->ClearDepthStencilView( RenderingSystem::m_CurRenderPass->OutputRenderTargets->GetD3DDepthStencilView(), D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void DvigEngine::RenderingSystem::BeginRenderPass(const RenderPassInfo* renderPass)
{
    RenderingSystem::m_CurRenderPass = (RenderPassInfo*)renderPass;
    RenderingSystem::m_GlobalInstanceCount = 0;
    
    if (RenderingSystem::m_CurRenderPass->OutputRenderTargets != nullptr)
    {
        ID3D11RenderTargetView* rtViews[1] = { RenderingSystem::m_CurRenderPass->OutputRenderTargets->GetD3DRenderTargetView() };
        ID3D11DepthStencilView* dpViews[1] = { RenderingSystem::m_CurRenderPass->OutputRenderTargets->GetD3DDepthStencilView() };
        D3D11::m_DeviceContext->OMSetRenderTargets( 1, &rtViews[0], dpViews[0] );
    }
    else
    {
        RenderingSystem::m_CurRenderPass->OutputRenderTargets = D3D11::m_ScreenRenderTargetGroup;
        ID3D11RenderTargetView* rtViews[1] = { RenderingSystem::m_CurRenderPass->OutputRenderTargets->GetD3DRenderTargetView() };
        ID3D11DepthStencilView* dpViews[1] = { RenderingSystem::m_CurRenderPass->OutputRenderTargets->GetD3DDepthStencilView() };
        D3D11::m_DeviceContext->OMSetRenderTargets( 1, &rtViews[0], dpViews[0] );
        D3D11::m_DeviceContext->OMSetDepthStencilState( RenderingSystem::m_DepthStencilState, 0 );
        mfloat color[4] = { 0.0f, 0.0f, 1.0f, 1.0f };
        D3D11::m_DeviceContext->ClearRenderTargetView( rtViews[0], &color[0] );
        D3D11::m_DeviceContext->ClearDepthStencilView( dpViews[0], D3D11_CLEAR_DEPTH , 1.0f, 0 );
    }
}

void DvigEngine::RenderingSystem::BeginBatch()
{
    RenderBatchInfo batch;
    batch.StartVertex = DVIG_NULL;
    batch.StartIndex = DVIG_NULL;
    batch.StartInstance = DVIG_NULL;

    RenderingSystem::m_Batches->Insert( &batch, "new" );
    RenderingSystem::m_GlobalInstanceCount = 0;
}

void DvigEngine::RenderingSystem::Draw(INode* node)
{
    RenderBatchInfo* lastBatch = (RenderBatchInfo*)RenderingSystem::m_Batches->FindAt( RenderingSystem::m_Batches->GetCount() - 1 );
    if (lastBatch == nullptr) { return; }
    
    if (lastBatch->StartVertex == DVIG_NULL)
    {
        // Populate batch info
        GeometryComponent* geometry = (GeometryComponent*)node->FindChildInstanceByType<GeometryComponent>();
        ShaderComponent* shader = (ShaderComponent*)node->FindChildInstanceByType<ShaderComponent>();
        if (geometry == nullptr || shader == nullptr) { return; }

        lastBatch->Geometry = geometry;
        lastBatch->Shader = shader;
        lastBatch->StartVertex = geometry->m_GeometryBufferOffset;
        lastBatch->StartIndex = geometry->m_IndexBufferOffset;
        lastBatch->StartIndex = RenderingSystem::m_GlobalInstanceCount;
        lastBatch->InstanceCount = 0;
    }

    // Populate constant buffer
    ViewerComponent* viewer = (ViewerComponent*)RenderingSystem::m_CurRenderPass->Viewer->FindChildInstanceByType<ViewerComponent>();
    if (viewer == nullptr) { return; }

    for (int i = 0; i < D3D11::m_DebugInfoQueue->GetNumStoredMessages(); ++i)
        {
            SIZE_T messageByteWidth = 0;
            D3D11::m_DebugInfoQueue->GetMessage( i, nullptr, &messageByteWidth );
            D3D11_MESSAGE* message = (D3D11_MESSAGE*)malloc( messageByteWidth );
            D3D11::m_DebugInfoQueue->GetMessage( i, message, &messageByteWidth );
            std::cout << message->pDescription << std::endl;
            free( message );
        }

    glm::mat4 mats[2];
    Engine::GetClassInstance()->MemoryCopy( &mats[0], &viewer->m_ViewSpaceMatrix[0][0], sizeof(glm::mat4) );
    Engine::GetClassInstance()->MemoryCopy( &mats[1], &viewer->m_ProjectionSpaceMatrix[0][0], sizeof(glm::mat4) );
    D3D11::WriteToConstantBuffer( RenderingSystem::m_GlobalConstantBuffer, &mats[0] );

    // Populate uniform buffer
    TransformComponent* transform = (TransformComponent*)node->FindChildInstanceByType<TransformComponent>();
    if (transform == nullptr) { return; }

    const sint32 offset = RenderingSystem::m_GlobalInstanceCount * sizeof(RenderBatchInstanceInfo);
    D3D11::WriteToBuffer( RenderingSystem::m_GlobalUniformBuffer, &transform->m_Position[0], offset, sizeof(RenderBatchInstanceInfo) );

    lastBatch->InstanceCount += 1;
    RenderingSystem::m_GlobalInstanceCount += 1;
}

void DvigEngine::RenderingSystem::EndBatch()
{
    RenderBatchInfo* lastBatch = (RenderBatchInfo*)RenderingSystem::m_Batches->FindAt( RenderingSystem::m_Batches->GetCount() - 1 );
    if (lastBatch == nullptr || lastBatch->StartVertex == DVIG_NULL) { return; }

    // Bind D3D11 pipeline
    ID3D11Buffer* vertexBuffers[1] = { RenderingSystem::m_GlobalVertexBuffer };
    const UINT strides[1] = { 32 };
    const UINT offsets[1] = { 0 };
    ID3D11Buffer* constantBuffers[1] = { RenderingSystem::m_GlobalConstantBuffer };
    ID3D11ShaderResourceView* shaderResources[1] = { RenderingSystem::m_GlobalUniformBufferView };
    D3D11::m_DeviceContext->IASetInputLayout( lastBatch->Shader->m_InputLayout );
    D3D11::m_DeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
    D3D11::m_DeviceContext->IASetVertexBuffers( 0, 1, &vertexBuffers[0], &strides[0], &offsets[0] );
    D3D11::m_DeviceContext->IASetIndexBuffer( RenderingSystem::m_GlobalIndexBuffer, DXGI_FORMAT_R32_UINT, 0 );
    D3D11::m_DeviceContext->VSSetShader( lastBatch->Shader->m_VertexShader, nullptr, 0 );
    D3D11::m_DeviceContext->VSSetConstantBuffers( 0, 1, &constantBuffers[0] );
    D3D11::m_DeviceContext->VSSetShaderResources( 0, 1, &shaderResources[0] );
    D3D11::m_DeviceContext->PSSetShader( lastBatch->Shader->m_PixelShader, nullptr, 0 );

    // Draw
    D3D11::m_DeviceContext->DrawIndexedInstanced( lastBatch->Geometry->m_IndexCount, lastBatch->InstanceCount,
        0, 0, 0
    );
}

void DvigEngine::RenderingSystem::EndRenderPass()
{
    RenderingSystem::m_CurRenderPass = nullptr;

    // Clear batches
    RenderingSystem::m_Batches->Clear();
}