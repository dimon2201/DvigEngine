#ifndef _DE_LIB_GFX_H_
#define _DE_LIB_GFX_H_

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
            DynamicBuffer* m_VertexBuffer;
    };
}

#endif