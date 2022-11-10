#ifndef _DE_LIB_RENDERING_H_
#define _DE_LIB_RENDERING_H_

#include "DECore.hpp"

namespace DvigEngine2
{
    class GeometryComponent : public IComponent
    {
        public:
            void Init(const char* optGeometryPathOnDrive, void* optGeometryData, deusize optGeometryDataByteWidth);
            void Free() override final;

        public:
            int val;
            deusize m_BufferByteWidth;
            deint32 m_BufferOffset;
    };

    class GeometryBatch : public IHelperObject
    {
        public:
            void Init();
            void Free() override final;
            static void ClearGeometryBuffer();

        public:
            static DynamicBuffer* m_GlobalGeometryBuffer;
    };

    class RenderingSystem : public ISystem
    {
        DV_MACRO_DECLARE_SINGLETON(RenderingSystem, private)

        public:
            void Init();
            void BeginRender();
            void BeginBatch();
            void EndRender();
            void EndBatch();

        private:
            FixedSet* m_Batches;
    };
}

#endif