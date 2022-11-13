#ifndef _DE_LIB_RENDERING_H_
#define _DE_LIB_RENDERING_H_

#include "DECore.hpp"
#include "DEThirdPartyMath.hpp"

namespace DvigEngine2
{
    class GeometryComponent : public IComponent
    {
        public:
            void Init(const char* optGeometryPathOnDrive, void* optGeometryData, deusize optGeometryDataByteWidth);
            void Free() override final;

        private:
            static void ClearGeometryBuffer();

        private:
            static DynamicBuffer* m_GlobalGeometryBuffer;

        public:
            int val;
            deusize m_BufferByteWidth;
            deint32 m_BufferOffset;
    };

    class TransformComponent : public IComponent
    {
        public:
            void Init() { }
            void Free() override final { }

        public:
            glm::vec3 m_Position;
    };

    class BatchInstanceData
    {
        public:
            TransformComponent m_Transform;
    };

    class BatchData
    {
        public:
            GeometryComponent* m_GeometryComponent;
            deint32 m_UniformBufferOffset;
    };

    class RenderingSystem : public ISystem
    {
        DV_MACRO_FRIENDS(DvigEngine2::Engine)
        DV_XMACRO_DECLARE_STATIC_CLASS(RenderingSystem)

        public:
            static void Init();
            static void BeginRender();
            static void BeginBatch();
            static void EndRender();
            static void EndBatch();
            static void Draw(INode* node);

        private:
            static FixedSet* m_Batches;
            static debool m_IsBatchRecording;
            static deint32 m_NextBatchUniformBufferOffset;
            static DynamicBuffer* m_UniformBuffer;
    };
}

#endif