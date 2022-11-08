#ifndef _DE_LIB_GFX_H_
#define _DE_LIB_GFX_H_

#include "DECore.hpp"

namespace DvigEngine2
{
    class GeometryComponent : public IComponent
    {
        public:
            void Initx(const char* optGeometryPathOnDrive, void* optGeometryData, deusize optGeometryDataByteWidth);

        public:
            DynamicBuffer* m_VertexBuffer;
    };
}

#endif