#ifndef _DE_LIB_RENDERING_H_
#define _DE_LIB_RENDERING_H_

#include "DECore.hpp"
#include "DEThirdPartyGL.hpp"
#include "DEThirdPartyMath.hpp"
#include "DEThirdPartyWindow.hpp"

namespace DvigEngine
{
    class TextureMergerTexture;

    class GeometryVertex
    {
        public:
            defloat32 m_Position[3];
            defloat32 m_Normal[3];
    };

    class GeometryComponent : public IComponent
    {
        public:
            void Init(const char* optGeometryDataPathOnDrive, const char* optIndicesDataPathOnDrive, void* optGeometryData, void* optIndicesData, deusize optGeometryDataByteWidth, deusize optIndicesDataByteWidth);
            void Init(const char* meshPathOnDrive);
            void Free() override final;

        private:
            static void ClearGeometryBuffer();

        public:
            deusize m_IndexCount;
            deusize m_GeometryBufferByteWidth;
            deint32 m_GeometryBufferOffset;
            deusize m_IndexBufferByteWidth;
            deint32 m_IndexBufferOffset;
    };

    class TransformComponent : public IComponent
    {
        public:
            void Init();
            void Free() override final;
            
            DV_FUNCTION_INLINE demfloat GetPositionX() { return (demfloat)this->m_Position.x; }
            DV_FUNCTION_INLINE demfloat GetPositionY() { return (demfloat)this->m_Position.y; }
            DV_FUNCTION_INLINE demfloat GetPositionZ() { return (demfloat)this->m_Position.z; }
            DV_FUNCTION_INLINE demfloat GetRotationX() { return (demfloat)glm::degrees(this->m_RotationEuler.x); }
            DV_FUNCTION_INLINE demfloat GetRotationY() { return (demfloat)glm::degrees(this->m_RotationEuler.y); }
            DV_FUNCTION_INLINE demfloat GetRotationZ() { return (demfloat)glm::degrees(this->m_RotationEuler.z); }
            DV_FUNCTION_INLINE demfloat GetScaleX() { return (demfloat)this->m_Scale.x; }
            DV_FUNCTION_INLINE demfloat GetScaleY() { return (demfloat)this->m_Scale.y; }
            DV_FUNCTION_INLINE demfloat GetScaleZ() { return (demfloat)this->m_Scale.z; }

            void AddPositionX(demfloat value);
            void AddPositionY(demfloat value);
            void AddPositionZ(demfloat value);
            void AddRotationEulerX(demfloat value);
            void AddRotationEulerY(demfloat value);
            void AddRotationEulerZ(demfloat value);
            void AddScaleX(demfloat value);
            void AddScaleY(demfloat value);
            void AddScaleZ(demfloat value);
            void SetPosition(demfloat x, demfloat y, demfloat z);
            void SetRotationEuler(demfloat x, demfloat y, demfloat z);
            void SetScale(demfloat x, demfloat y, demfloat z);

        public:
            glm::vec3 m_Position;
            glm::vec3 m_RotationEuler;
            glm::vec3 m_Scale;
            glm::quat m_WorldRotationQuaternion;
            glm::mat4 m_WorldTranslationMatrix;
            glm::mat4 m_WorldRotationMatrix;
            glm::mat4 m_WorldScaleMatrix;
            glm::mat4 m_WorldSpaceMatrix;
    };

    class ShaderComponent : public IComponent
    {
        public:
            void Init(const char* vertexShaderPathOnDrive, const char* fragmentShaderPathOnDrive);
            void Free() override final;

        public:
            deuint32 m_GLProgram;
    };

    class MaterialComponent : public IComponent
    {
        public:
            void Init();
            void Free() override final;

        public:
            TextureMergerTexture* m_DiffuseTexture;
    };

    class ViewerComponent : public IComponent
    {
        public:
            void Init();
            void Free() override final;
            void AddPositionX(demfloat value);
            void AddPositionY(demfloat value);
            void AddPositionZ(demfloat value);
            void AddRotationEulerX(demfloat value);
            void AddRotationEulerY(demfloat value);
            void AddRotationEulerZ(demfloat value);
            void Move(demfloat value);
            void Strafe(demfloat value);
            void Lift(demfloat value);

            DV_FUNCTION_INLINE demfloat GetPositionX() { return this->m_Position.x; }
            DV_FUNCTION_INLINE demfloat GetPositionY() { return this->m_Position.y; }
            DV_FUNCTION_INLINE demfloat GetPositionZ() { return this->m_Position.z; }
            DV_FUNCTION_INLINE demfloat GetRotationX() { return glm::degrees(this->m_RotationEuler.x); }
            DV_FUNCTION_INLINE demfloat GetRotationY() { return glm::degrees(this->m_RotationEuler.y); }
            DV_FUNCTION_INLINE demfloat GetRotationZ() { return glm::degrees(this->m_RotationEuler.z); }
            void SetPosition(demfloat x, demfloat y, demfloat z);
            void SetRotationX(demfloat value);
            void SetRotationY(demfloat value);
            void SetRotationZ(demfloat value);
            void SetRotationEuler(demfloat eulerX, demfloat eulerY, demfloat eulerZ);
            void SetOrthographicProjection(demfloat left, demfloat right, demfloat bottom, demfloat top);
            void SetPerspectiveProjection(demfloat fov, demfloat aspect, demfloat zNear, demfloat zFar);

        public:
            glm::vec3 m_Position;
            glm::vec3 m_Direction;
            glm::vec3 m_RotationEuler;
            glm::quat m_RotationQuaternion;
            glm::mat4 m_ViewSpaceMatrix;
            glm::mat4 m_ProjectionSpaceMatrix;
    };

    class PostProcessorComponent : public IComponent
    {
        public:
            void Init();
            void Free() override final;

        public:
    };

    class RenderTargetGroup : public IHelperObject
    {
        public:
            void Init(glm::uvec2& size);
            void Free() override final;

            DV_FUNCTION_INLINE deuint32 GetGLFramebuffer() { return m_Framebuffer; }
            DV_FUNCTION_INLINE deuint32 GetGLColorRenderTarget() { return m_RenderTargets[0]; }
            DV_FUNCTION_INLINE deuint32 GetGLDepthRenderTarget() { return m_RenderTargets[1]; }

        private:
            deuint32 m_Framebuffer;
            deuint32 m_RenderTargets[2];
    };

    enum class RenderPassType
    {
        FRAMEBUFFER = 0,
        SCREEN_FINAL = 1
    };

    class RenderPassInfo
    {
        public:
            RenderPassType Type;
            RenderTargetGroup* InputRenderTargets;
            RenderTargetGroup* OutputRenderTargets;
            INode* Viewer;
            INode* PostProcessor;

        private:
            static deint32 m_TargetPingPongIndex;
    };

    class UniformViewerData
    {
        public:
            glm::mat4 m_WorldSpaceMatrix;
            glm::mat4 m_ViewSpaceMatrix;
            glm::mat4 m_ProjectionSpaceMatrix;
    };

    class UniformBatchInstanceData
    {
        public:
            static deusize m_GLAlignedByteWidth;

        public:
            glm::mat4 m_TransformMatrix;
    };

    class BatchData
    {
        public:
            deusize m_InstanceCount;
            GeometryComponent* m_GeometryComponent;
            ShaderComponent* m_ShaderComponent;
            deint32 m_UniformBufferOffset;
    };

    class RenderingSystem : public ISystem
    {
        DV_MACRO_FRIENDS(Engine)
        DV_XMACRO_DECLARE_STATIC_CLASS(RenderingSystem)

        public:
            static void Init();
            static void Free();
            static void Viewport(deint32 x, deint32 y, deisize width, deisize height);
            static void PaintBackground(demfloat red, demfloat green, demfloat blue, demfloat alpha);
            static void BeginRenderPass(RenderPassInfo* renderPassInfo);
            static void BeginBatch();
            static void EndRenderPass();
            static void EndBatch();
            static void Draw(INode* const node);
            static void ClearGeometryAndIndexBuffers();

        public:
            static DynamicBuffer* m_GlobalGeometryBuffer;
            static DynamicBuffer* m_GlobalIndexBuffer;
            static deuint32 m_GLGlobalGeometryBuffer;
            static deuint32 m_GLGlobalIndexBuffer;
            static deuint32 m_GLVAO;

        private:
            static RenderPassInfo* m_CurRenderPass;
            static FixedSet* m_Batches;
            static debool m_IsBatchRecording;
            static deint32 m_NextBatchUniformBufferOffset;
            static MemoryObject* m_UniformBufferMemoryObject;
            static deuint32 m_GLUniformBuffer;
    };

    class TextureMergerTexture
    {
        public:
            deint32 m_X;
            deint32 m_Y;
            deint32 m_Z;
            deusize m_Width;
            deusize m_Height;
    };

    class TextureMergerSystem : public ISystem
    {
        DV_MACRO_FRIENDS(Engine)
        DV_XMACRO_DECLARE_STATIC_CLASS(TextureMergerSystem)

        public:
            static void Init(deusize atlasWidth, deusize atlasHeight, deusize atlasDepth);
            static void Free();
            static deint32 AddTexture(const deusize width, const deusize height, const void* pixels);
            static void RemoveTexture(const deint32 index);

            DV_FUNCTION_INLINE static deuint32 GetGLAtlas() { return m_Atlas; }
            DV_FUNCTION_INLINE static TextureMergerTexture* GetAtlasTexture(const deint32 index) { return TextureMergerSystem::m_Textures->Find<TextureMergerTexture*>( index ); }
            DV_FUNCTION_INLINE static deusize GetAtlasWidth() { return TextureMergerSystem::m_AtlasWidth; }
            DV_FUNCTION_INLINE static deusize GetAtlasHeight() { return TextureMergerSystem::m_AtlasHeight; }
            DV_FUNCTION_INLINE static deusize GetAtlasDepth() { return TextureMergerSystem::m_AtlasDepth; }

        private:
            static deusize m_AtlasWidth;
            static deusize m_AtlasHeight;
            static deusize m_AtlasDepth;
            static deuint32 m_Atlas;
            static FixedSet* m_Textures;
    };
}

#endif