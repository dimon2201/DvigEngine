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
            deint32 m_GlobalGeometryBufferOffset;
    };

    class GeometryBatch : public IHelperObject
    {
        public:
            void Init();
            void Free() override final;

        public:
            static DynamicBuffer* m_GlobalGeometryBuffer;
    };
}

#endif