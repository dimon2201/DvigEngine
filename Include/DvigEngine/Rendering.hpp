#ifndef _DE_LIB_RENDERING_H_
#define _DE_LIB_RENDERING_H_

#include "Core.hpp"
#include "GUI.hpp"
#include "../ThirdParty/ThirdPartyD3D11.hpp"
#include "../ThirdParty/ThirdPartyMath.hpp"

namespace DvigEngine
{
    #define DVIG_MAX_GFX_BUFFER_BYTE_WIDTH 24 * DVIG_MEMORY_MiB

    class RenderingSystem;

    class TransformComponent : public Component
    {
        public:
            void Init();
            void Free();

            void SetPosition(mfloat x, mfloat y, mfloat z);
            void SetRotation(mfloat x, mfloat y, mfloat z);
            void SetScale(mfloat x, mfloat y, mfloat z);
            DVIG_FUNCTION_INLINE mfloat GetPositionX() { return this->m_Position.x; }
            DVIG_FUNCTION_INLINE mfloat GetPositionY() { return this->m_Position.y; }
            DVIG_FUNCTION_INLINE mfloat GetPositionZ() { return this->m_Position.z; }
            DVIG_FUNCTION_INLINE mfloat GetRotationX() { return this->m_Rotation.x; }
            DVIG_FUNCTION_INLINE mfloat GetRotationY() { return this->m_Rotation.y; }
            DVIG_FUNCTION_INLINE mfloat GetRotationZ() { return this->m_Rotation.z; }
            DVIG_FUNCTION_INLINE mfloat GetScaleX() { return this->m_Scale.x; }
            DVIG_FUNCTION_INLINE mfloat GetScaleY() { return this->m_Scale.y; }
            DVIG_FUNCTION_INLINE mfloat GetScaleZ() { return this->m_Scale.z; }

        public:
            glm::vec3 m_Position;
            glm::vec3 m_Rotation;
            glm::vec3 m_Scale;
            glm::mat4 m_WorldSpaceMatrix;
    };

    class alignas(4) GeometryVertex
    {
        public:
            glm::vec3 m_Position;
            glm::vec2 m_Texcoord;
            glm::vec3 m_Normal;
    };
    
    class GeometryComponent : public Component
    {
        friend Engine;

        public:
            void Init(const char* meshPathOnDrive);
            void Free();

        private:
            static void ClearGeometryBuffer();

        public:
            usize m_IndexCount;
            usize m_GeometryBufferByteWidth;
            sint32 m_GeometryBufferOffset;
            usize m_IndexBufferByteWidth;
            sint32 m_IndexBufferOffset;
    };

    class ShaderComponent : public Component
    {
        friend Engine;

        public:
            void Init();
            void Free();

            void SetVertexShader(const char* shaderPathOnDrive);
            void SetPixelShader(const char* shaderPathOnDrive);

        public:
            ID3D11VertexShader* m_VertexShader;
            ID3D11PixelShader* m_PixelShader;
            ID3D11InputLayout* m_InputLayout;
    };

    class ViewerComponent : public Component
    {
        public:
            void Init();
            void Free();
            void AddPositionX(mfloat value);
            void AddPositionY(mfloat value);
            void AddPositionZ(mfloat value);
            void AddRotationEulerX(mfloat value);
            void AddRotationEulerY(mfloat value);
            void AddRotationEulerZ(mfloat value);
            void Move(mfloat value);
            void Strafe(mfloat value);
            void Lift(mfloat value);
            void DoMouseLook(IWindow* window);

            DVIG_FUNCTION_INLINE mfloat GetPositionX() { return this->m_Position.x; }
            DVIG_FUNCTION_INLINE mfloat GetPositionY() { return this->m_Position.y; }
            DVIG_FUNCTION_INLINE mfloat GetPositionZ() { return this->m_Position.z; }
            DVIG_FUNCTION_INLINE mfloat GetRotationX() { return glm::degrees(this->m_RotationEuler.x); }
            DVIG_FUNCTION_INLINE mfloat GetRotationY() { return glm::degrees(this->m_RotationEuler.y); }
            DVIG_FUNCTION_INLINE mfloat GetRotationZ() { return glm::degrees(this->m_RotationEuler.z); }
            void SetPosition(mfloat x, mfloat y, mfloat z);
            void SetRotationX(mfloat value);
            void SetRotationY(mfloat value);
            void SetRotationZ(mfloat value);
            void SetRotationEuler(mfloat eulerX, mfloat eulerY, mfloat eulerZ);
            void SetOrthographicProjection(mfloat left, mfloat right, mfloat bottom, mfloat top);
            void SetPerspectiveProjection(mfloat fov, mfloat aspect, mfloat zNear, mfloat zFar);

        public:
            glm::vec3 m_Position;
            glm::vec3 m_Direction;
            glm::vec3 m_RotationEuler;
            glm::quat m_RotationQuaternion;
            glm::mat4 m_ViewSpaceMatrix;
            glm::mat4 m_ProjectionSpaceMatrix;
    };

    class RenderTargetGroup : public Node
    {
        public:
            void Init(void* const colorTexture, void* const depthTexture, glm::uvec2& size);
            void Free();

            DVIG_FUNCTION_INLINE ID3D11Texture2D* GetD3DColorRenderTarget() { return m_ColorRenderTarget; }
            DVIG_FUNCTION_INLINE ID3D11Texture2D* GetD3DDepthRenderTarget() { return m_DepthRenderTarget; }
            DVIG_FUNCTION_INLINE ID3D11RenderTargetView* GetD3DRenderTargetView() { return m_RenderTargetView; }
            DVIG_FUNCTION_INLINE ID3D11DepthStencilView* GetD3DDepthStencilView() { return m_DepthStencilView; }

        private:
            ID3D11Texture2D* m_ColorRenderTarget;
            ID3D11Texture2D* m_DepthRenderTarget;
            ID3D11RenderTargetView* m_RenderTargetView;
            ID3D11DepthStencilView* m_DepthStencilView;
    };

    enum class RenderPassType
    {
        RENDERTARGET = 0,
        SCREEN = 1
    };

    class RenderPassInfo : public Info
    {
        public:
            RenderPassType Type;
            RenderTargetGroup* InputRenderTargets;
            RenderTargetGroup* OutputRenderTargets;
            Node* Viewer;
            Node* PostProcessor;
    };

    class RenderBatchInfo : public Info
    {
        friend RenderingSystem;

        public:
            GeometryComponent* Geometry;
            ShaderComponent* Shader;

        private:
            uint32 StartVertex;
            uint32 StartIndex;
            uint32 StartInstance;
            uint32 InstanceCount;
    };

    class RenderBatchInstanceInfo : public Info
    {
        public:
            glm::mat4 WorldSpaceMatrix;
    };

    class RenderingSystem : public ISystem
    {
        friend GeometryComponent;

        DVIG_XMACRO_DECLARE_STATIC_CLASS(RenderingSystem)

        public:
            static void Init();
            static void Free();
            
            static void Viewport(mfloat x, mfloat y, mfloat width, mfloat height);
            static void ClearBackground(mfloat red, mfloat green, mfloat blue, mfloat alpha);
            static void BeginRenderPass(const RenderPassInfo* renderPass);
            static void BeginBatch(const RenderBatchInfo* batch);
            static void Draw(Node* node);
            static void EndBatch();
            static void EndRenderPass();

        private:
            static ID3D11Buffer* m_GlobalVertexBuffer;
            static ID3D11Buffer* m_GlobalIndexBuffer;
            static ID3D11Buffer* m_GlobalUniformBuffer;
            static ID3D11Buffer* m_GlobalConstantBuffer;
            static ID3D11ShaderResourceView* m_GlobalUniformBufferView;
            static sint32 m_GlobalVertexBufferOffset;
            static sint32 m_GlobalIndexBufferOffset;
            static RenderPassInfo* m_CurRenderPass;
            static Collection* m_Batches;
            static uint32 m_GlobalInstanceCount;
            static ID3D11DepthStencilState* m_DepthStencilState;
    };
}

#endif