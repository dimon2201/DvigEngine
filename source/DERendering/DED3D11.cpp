#include "../../include/dvigengine/DED3D11.hpp"
#include "../../include/dvigengine/DERendering.hpp"
#include <iostream>

ID3D11Device* DvigEngine::D3D11::m_Device = nullptr;
ID3D11DeviceContext* DvigEngine::D3D11::m_DeviceContext = nullptr;
ID3D11InfoQueue* DvigEngine::D3D11::m_DebugInfoQueue = nullptr;
IDXGISwapChain* DvigEngine::D3D11::m_SwapChain = nullptr;
ID3D11Texture2D* DvigEngine::D3D11::m_SwapChainColorBackBuffer = nullptr;
DvigEngine::RenderTargetGroup* DvigEngine::D3D11::m_ScreenRenderTargetGroup = nullptr;

void DvigEngine::D3D11::Init(HWND hWnd, glm::uvec2& size)
{
    HRESULT result;

    // Create device and swap chain
    D3D_FEATURE_LEVEL featureLevels[1];
    featureLevels[0] = D3D_FEATURE_LEVEL_11_0;

    DXGI_MODE_DESC bufferDesc;
    Engine::GetClassInstance()->MemorySet( &bufferDesc, 0, sizeof(DXGI_MODE_DESC) );
    bufferDesc.Width = 640;
    bufferDesc.Height = 480;
    bufferDesc.RefreshRate.Numerator = 60;
    bufferDesc.RefreshRate.Denominator = 1;
    bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

    DXGI_SWAP_CHAIN_DESC swapChainDesc;
    Engine::GetClassInstance()->MemorySet( &swapChainDesc, 0, sizeof(DXGI_SWAP_CHAIN_DESC) );
    swapChainDesc.BufferDesc = bufferDesc;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferCount = 1;
    swapChainDesc.OutputWindow = hWnd; 
    swapChainDesc.Windowed = TRUE; 
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    result = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, NULL, D3D11_CREATE_DEVICE_DEBUG, 
        &featureLevels[0], 1, D3D11_SDK_VERSION, &swapChainDesc,
        &D3D11::m_SwapChain, &D3D11::m_Device, nullptr, &D3D11::m_DeviceContext
    );

    if (FAILED( result )) { std::cout << "Error creating Swap Chain!" << std::endl; }
    DVIG_ASSERT( (result == S_OK), DebugResult::NOT_CREATED_SWAPCHAIN_D3D11 );

    // Create Debug
    D3D11::m_Device->QueryInterface( __uuidof(ID3D11InfoQueue), (void **)&D3D11::m_DebugInfoQueue);
    D3D11::m_DebugInfoQueue->PushEmptyStorageFilter();

    // Create render target
    result = D3D11::m_SwapChain->GetBuffer( 0, __uuidof(ID3D11Texture2D), (void**)&D3D11::m_SwapChainColorBackBuffer );
    D3D11::m_ScreenRenderTargetGroup = Engine::GetClassInstance()->Create<RenderTargetGroup>( "_RenderTargetGroup" );
    D3D11::m_ScreenRenderTargetGroup->Init(D3D11::m_SwapChainColorBackBuffer, nullptr, size);
}

ID3D11Buffer* DvigEngine::D3D11::CreateBuffer(D3D11_USAGE usage, UINT bindFlags, UINT accessFlags, UINT miscFlags, UINT stride, const void* memory, const usize byteWidth)
{
    ID3D11Buffer* buffer;

    D3D11_BUFFER_DESC desc;
    Engine::GetClassInstance()->MemorySet( &desc, 0, sizeof(D3D11_BUFFER_DESC) );
    desc.ByteWidth = byteWidth;
    desc.Usage = usage;
    desc.BindFlags = bindFlags;
    desc.CPUAccessFlags = accessFlags;
    desc.MiscFlags = miscFlags;
    desc.StructureByteStride = stride;

    D3D11::m_Device->CreateBuffer( &desc, nullptr, &buffer );
    return buffer;
}

ID3D11Texture2D* DvigEngine::D3D11::CreateTexture2D(UINT width, UINT height, DXGI_FORMAT format, UINT bindFlags, const void* memory)
{
    ID3D11Texture2D* texture;

    D3D11_TEXTURE2D_DESC desc;
    Engine::GetClassInstance()->MemorySet( &desc, 0, sizeof(D3D11_TEXTURE2D_DESC) );
    desc.Width = width;
    desc.Height = height;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = format;
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | bindFlags;

    D3D11::m_Device->CreateTexture2D( &desc, nullptr, &texture );
    return texture;
}

void DvigEngine::D3D11::CreateRenderTargetViews(ID3D11Texture2D* colorRenderTarget, ID3D11Texture2D* depthRenderTarget, ID3D11RenderTargetView** renderTargetView, ID3D11DepthStencilView** depthStencilView)
{
    D3D11_RENDER_TARGET_VIEW_DESC rtViewDesc;
    Engine::GetClassInstance()->MemorySet( &rtViewDesc, 0, sizeof(D3D11_RENDER_TARGET_VIEW_DESC) );
    rtViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    rtViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    rtViewDesc.Texture2D.MipSlice = 0;

    D3D11::m_Device->CreateRenderTargetView( colorRenderTarget, &rtViewDesc, renderTargetView );

    D3D11_DEPTH_STENCIL_VIEW_DESC dsViewDesc;
    Engine::GetClassInstance()->MemorySet( &dsViewDesc, 0, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC) );
    dsViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
    dsViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    dsViewDesc.Texture2D.MipSlice = 0;

    D3D11::m_Device->CreateDepthStencilView( depthRenderTarget, &dsViewDesc, depthStencilView );
}

ID3D11DepthStencilState* DvigEngine::D3D11::CreateDepthStencilState()
{
    ID3D11DepthStencilState* dsState;
    
    D3D11_DEPTH_STENCIL_DESC dsDesc;
    Engine::GetClassInstance()->MemorySet( &dsDesc, 0, sizeof(D3D11_DEPTH_STENCIL_DESC) );
    dsDesc.DepthEnable = TRUE;
    dsDesc.StencilEnable = FALSE;

    D3D11::m_Device->CreateDepthStencilState( &dsDesc, &dsState );

    return dsState;
}

ID3D11ShaderResourceView* DvigEngine::D3D11::CreateShaderResourceView(ID3D11Buffer* buffer, const usize byteWidth)
{
    ID3D11ShaderResourceView* srv;

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    srvDesc.Format = DXGI_FORMAT_UNKNOWN;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
    srvDesc.Buffer.FirstElement = 0;
    srvDesc.Buffer.ElementWidth = 4;
    srvDesc.Buffer.ElementOffset = 0;
    srvDesc.Buffer.NumElements = byteWidth / 4;

    D3D11::m_Device->CreateShaderResourceView( buffer, &srvDesc, &srv );
    return srv;
}

void DvigEngine::D3D11::DeleteBuffer(ID3D11Buffer* buffer)
{
    buffer->Release();
}

void DvigEngine::D3D11::DeleteTexture(ID3D11Texture2D* texture)
{
    texture->Release();
}

void DvigEngine::D3D11::WriteToBuffer(ID3D11Buffer* buffer, const void* memory, const sint32 offset, const usize byteWidth)
{
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    D3D11::m_DeviceContext->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

    void* offsetAddress = (void*)((cpuword)mappedResource.pData + (cpuword)offset);
    Engine::GetClassInstance()->MemoryCopy(offsetAddress, memory, byteWidth);
    
    D3D11::m_DeviceContext->Unmap(buffer, 0);
}

void DvigEngine::D3D11::WriteToConstantBuffer(ID3D11Buffer* buffer, const void* memory)
{
    D3D11::m_DeviceContext->UpdateSubresource( buffer, 0, 0, memory, 0, 0 );
}