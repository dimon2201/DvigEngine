#ifndef _DE_LIB_RENDERING_H_
#define _DE_LIB_RENDERING_H_

#include "DECore.hpp"
#include "DEThirdPartyGL.hpp"
#include "DEThirdPartyMath.hpp"

namespace DvigEngine2
{
    class GL4
    {
        DV_XMACRO_DECLARE_STATIC_CLASS(GL4)

        public:
            static void Load();

        public:
            static void (*_Init)(void);
            static void (*Viewport)(deint32 x, deint32 y, deisize width, deisize height);
            static void (*Clear)(deuint32 mask);
            static void (*ClearColor)(defloat32 red, defloat32 green, defloat32 blue, defloat32 alpha);
            static void (*GenBuffers)(deisize n, deuint32* buffers);
            static void (*BindBuffer)(deuint32 target, deuint32 buffer);
            static void (*BufferData)(deuint32 target, demachword size, const void* data, deuint32 usage);
            static void (*BufferSubData)(deuint32 target, demachword offset, demachword size, const void* data);
            static deuint32 (*CreateShader)(deuint32 shaderType);
            static void (*ShaderSource)(deuint32 shader, deisize count, const deuchar** string, const deint32* length);
            static void (*CompileShader)(deuint32 shader);
            static void (*GetShaderiv)(deint32 shader, deuint32 pname, deint32* params);
            static void (*GetShaderInfoLog)(deuint32 shader, deisize maxLength, deisize* length, deuchar* infoLog);
            static void (*DeleteShader)(deuint32 shader);
            static deuint32 (*CreateProgram)(void);
            static void (*AttachShader)(deuint32 program, deuint32 shader);
            static void (*LinkProgram)(deuint32 program);
            static void (*DetachShader)(deuint32 program, deuint32 shader);
            static void (*DeleteProgram)(deuint32 program);
            static void (*UseProgram)(deuint32 program);
            static void (*DrawElements)(deuint32 mode, deisize count, deuint32 type, void* indices);
            static void (*DrawElementsBaseVertex)(deuint32 mode, deisize count, deuint32 type, void* indices, deint32 baseVertex);
    };

    class GeometryComponent : public IComponent
    {
        public:
            void Init(const char* optGeometryDataPathOnDrive, const char* optIndicesDataPathOnDrive, void* optGeometryData, void* optIndicesData, deusize optGeometryDataByteWidth, deusize optIndicesDataByteWidth);
            void Free() override final;

        private:
            static void ClearGeometryBuffer();

        private:
            static DynamicBuffer* m_GlobalGeometryBuffer;
            static DynamicBuffer* m_GlobalIndexBuffer;
            static deuint32 m_GLGlobalGeometryBuffer;
            static deuint32 m_GLGlobalIndexBuffer;

        public:
            deusize m_GeometryBufferByteWidth;
            deint32 m_GeometryBufferOffset;
            deusize m_IndexBufferByteWidth;
            deint32 m_IndexBufferOffset;
    };

    class TransformComponent : public IComponent
    {
        public:
            void Init() { }
            void Free() override final { }

        public:
            glm::vec3 m_Position;
    };

    class ShaderComponent : public IComponent
    {
        public:
            void Init(const char* vertexShaderPathOnDrive, const char* fragmentShaderPathOnDrive);
            void Free() override final;

        public:
            deuint32 m_GLProgram;
    };

    class BatchInstanceData
    {
        public:
            glm::vec3 m_Position;
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
            static deuint32 m_GLUniformBuffer;
    };
}

#endif