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
            static void (*Enable)(deuint32 cap);
            static void (*Disable)(deuint32 cap);
            static void (*Viewport)(deint32 x, deint32 y, deisize width, deisize height);
            static void (*Clear)(deuint32 mask);
            static void (*ClearColor)(defloat32 red, defloat32 green, defloat32 blue, defloat32 alpha);
            static void (*GenBuffers)(deisize n, deuint32* buffers);
            static void (*GenVertexArrays)(deisize n, deuint32* arrays);
            static void (*BindBuffer)(deuint32 target, deuint32 buffer);
            static void (*BindBufferBase)(deuint32 target, deuint32 index, deuint32 buffer);
            static void (*BindVertexArray)(deuint32 buffer);
            static void (*BufferData)(deuint32 target, demachword size, const void* data, deuint32 usage);
            static void (*BufferSubData)(deuint32 target, demachword offset, demachword size, const void* data);
            static deuint32 (*CreateShader)(deuint32 shaderType);
            static void (*ShaderSource)(deuint32 shader, deisize count, const deuchar** string, const deint32* length);
            static void (*CompileShader)(deuint32 shader);
            static void (*GetShaderiv)(deint32 shader, deuint32 pname, deint32* params);
            static void (*GetShaderInfoLog)(deuint32 shader, deisize maxLength, deisize* length, deuchar* infoLog);
            static void (*GetProgramiv)(deuint32 program, deuint32 pname, deint32* params);
            static void (*DeleteShader)(deuint32 shader);
            static deuint32 (*CreateProgram)(void);
            static void (*AttachShader)(deuint32 program, deuint32 shader);
            static void (*LinkProgram)(deuint32 program);
            static void (*DetachShader)(deuint32 program, deuint32 shader);
            static void (*DeleteProgram)(deuint32 program);
            static void (*EnableVertexAttribArray)(deuint32 index);
            static void (*VertexAttribPointer)(deuint32 index, deint32 size, deuint32 type, bool normalized, deisize stride, const void* pointer);
            static deuint32 (*GetUniformBlockIndex)(deuint32 program, const char* uniformBlockName);
            static void (*UniformBlockBinding)(deuint32 program, deuint32 uniformBlockIndex, deuint32 uniformBlockBinding);
            static void (*UseProgram)(deuint32 program);
            static void (*DrawArrays)(deuint32 mode, deint32 first, deisize count);
            static void (*DrawElements)(deuint32 mode, deisize count, deuint32 type, void* indices);
            static void (*DrawElementsInstanced)(deuint32 mode, deisize count, deuint32 type, void* indices, deisize instancecount);
            static void (*DrawElementsBaseVertex)(deuint32 mode, deisize count, deuint32 type, void* indices, deint32 baseVertex);
            static deuint32 (*GetError)();
    };

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
            DV_FUNCTION_INLINE demfloat GetRotationX() { return (demfloat)this->m_RotationEuler.x; }
            DV_FUNCTION_INLINE demfloat GetRotationY() { return (demfloat)this->m_RotationEuler.y; }
            DV_FUNCTION_INLINE demfloat GetRotationZ() { return (demfloat)this->m_RotationEuler.z; }
            DV_FUNCTION_INLINE demfloat GetScaleX() { return (demfloat)this->m_Scale.x; }
            DV_FUNCTION_INLINE demfloat GetScaleY() { return (demfloat)this->m_Scale.y; }
            DV_FUNCTION_INLINE demfloat GetScaleZ() { return (demfloat)this->m_Scale.z; }

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

    class ViewerComponent : public IComponent
    {
        public:
            void Init();
            void Free() override final;

            DV_FUNCTION_INLINE demfloat GetRotationX() { return (demfloat)this->m_RotationEuler.x; }
            DV_FUNCTION_INLINE demfloat GetRotationY() { return (demfloat)this->m_RotationEuler.y; }
            DV_FUNCTION_INLINE demfloat GetRotationZ() { return (demfloat)this->m_RotationEuler.z; }

            void SetRotation(glm::vec3 eye, demfloat eulerX, demfloat eulerY, demfloat eulerZ);
            void SetOrthographicProjection(demfloat left, demfloat right, demfloat bottom, demfloat top);
            void SetPerspectiveProjection(demfloat fov, demfloat aspect, demfloat zNear, demfloat zFar);

        public:
            glm::vec3 m_RotationEuler;
            glm::quat m_ViewRotationQuaternion;
            glm::mat4 m_ViewSpaceMatrix;
            glm::mat4 m_ProjectionSpaceMatrix;
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
        DV_MACRO_FRIENDS(DvigEngine2::Engine)
        DV_XMACRO_DECLARE_STATIC_CLASS(RenderingSystem)

        public:
            static void Init();
            static void BeginRender(INode* const viewer);
            static void BeginBatch();
            static void EndRender();
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
            static INode* m_Viewer;
            static FixedSet* m_Batches;
            static debool m_IsBatchRecording;
            static deint32 m_NextBatchUniformBufferOffset;
            static MemoryObject* m_UniformBufferMemoryObject;
            static deuint32 m_GLUniformBuffer;
    };
}

#endif