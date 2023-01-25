#include "../../Include/DvigEngine/D3D11.hpp"
#include "../../Include/DvigEngine/Rendering.hpp"

void DvigEngine::RenderTargetGroup::Init(void* const colorTexture, void* const depthTexture, glm::uvec2& size)
{
    if (colorTexture == nullptr) {
        this->m_ColorRenderTarget = D3D11::CreateTexture2D( size.x, size.y, DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_RENDER_TARGET, nullptr );
    } else {
        this->m_ColorRenderTarget = (ID3D11Texture2D*)colorTexture;
    }
    
    if (depthTexture == nullptr) {
        this->m_DepthRenderTarget = D3D11::CreateTexture2D( size.x, size.y, DXGI_FORMAT_R32_TYPELESS, D3D11_BIND_DEPTH_STENCIL, nullptr );
    } else {
        this->m_DepthRenderTarget = (ID3D11Texture2D*)depthTexture;
    }
    
    D3D11::CreateRenderTargetViews( this->m_ColorRenderTarget, this->m_DepthRenderTarget, &this->m_RenderTargetView, &this->m_DepthStencilView );
}

void DvigEngine::RenderTargetGroup::Free()
{
    D3D11::DeleteTexture( this->m_ColorRenderTarget );
    D3D11::DeleteTexture( this->m_DepthRenderTarget );
    this->m_RenderTargetView->Release();
    this->m_DepthStencilView->Release();

    Engine::GetClassInstance()->MemoryDelete( this->GetMemoryObject() );
}