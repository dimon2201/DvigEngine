#ifndef _DE_LIB_D3D11_H_
#define _DE_LIB_D3D11_H_

#include "DECore.hpp"
#include "../thirdparty/DEThirdPartyD3D11.hpp"
#include "../thirdparty/DEThirdPartyMath.hpp"

namespace DvigEngine
{
    class ShaderComponent;
    class RenderTargetGroup;
    class RenderingSystem;

    class D3D11
    {
        friend Engine;
        friend ShaderComponent;
        friend RenderTargetGroup;
        friend RenderingSystem;
        DVIG_XMACRO_DECLARE_STATIC_CLASS(D3D11)

        public:
            static void Init(HWND hWnd, glm::uvec2& size);
            static void Free();
            static ID3D11Buffer* CreateBuffer(D3D11_USAGE usage, UINT bindFlags, UINT accessFlags, UINT miscFlags, UINT stride, const void* memory, const usize byteWidth);
            static ID3D11Texture2D* CreateTexture2D(UINT width, UINT height, DXGI_FORMAT format, UINT bindFlags, const void* memory);
            static void CreateRenderTargetViews(ID3D11Texture2D* colorRenderTarget, ID3D11Texture2D* depthRenderTarget, ID3D11RenderTargetView** renderTargetView, ID3D11DepthStencilView** depthStencilView);
            static ID3D11DepthStencilState* CreateDepthStencilState();
            static ID3D11ShaderResourceView* CreateShaderResourceView(ID3D11Buffer* buffer, const usize byteWidth);
            static void DeleteBuffer(ID3D11Buffer* buffer);
            static void DeleteTexture(ID3D11Texture2D* texture);
            static void WriteToBuffer(ID3D11Buffer* buffer, const void* memory, const sint32 offset, const usize byteWidth);
            static void WriteToConstantBuffer(ID3D11Buffer* buffer, const void* memory);

            DVIG_FUNCTION_INLINE static ID3D11Device* GetDevice() { return D3D11::m_Device; }
            DVIG_FUNCTION_INLINE static ID3D11DeviceContext* GetContext() { return D3D11::m_DeviceContext; }
            DVIG_FUNCTION_INLINE static IDXGISwapChain* GetSwapChain() { return D3D11::m_SwapChain; }
            // DVIG_FUNCTION_INLINE static RenderTargetGroup* GetScreenRenderTargetGroup() { return D3D11::m_ScreenRenderTargetGroup; }

        private:
            static ID3D11Device* m_Device;
            static ID3D11DeviceContext* m_DeviceContext;
            static ID3D11InfoQueue* m_DebugInfoQueue;
            static IDXGISwapChain* m_SwapChain;
            static ID3D11Texture2D* m_SwapChainColorBackBuffer;
            static RenderTargetGroup* m_ScreenRenderTargetGroup;
    };
}

#endif