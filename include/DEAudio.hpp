#ifndef _DE_LIB_AUDIO_H_
#define _DE_LIB_AUDIO_H_

#include "DECore.hpp"
#include "DEMath.hpp"
#include "thirdparty/DEThirdPartyAudio.hpp"

namespace DvigEngine
{
    class WAVFileHeader
    {
        public:
            deuchar MagicRIFF[4];
            deuint32 FileSize;
            deuchar MagicWAVE[4];
            deuchar MagicFmt[4];
            deuint32 LengthOfFormatData;
            deuint16 TypeOfFormat;
            deuint16 NumberOfChannels;
            deuint32 Freq;
            deuint32 Val0;
            deuint32 Val1;
            deuint16 BitsPerSample;
            deuchar MagicData[4];
            deuint32 DataSize;
    };

    class AudioSourceComponent : public IComponent
    {
        public:
            void Init(const char* wavFilePathOnDrive);
            void Free() override final;
    };

    class AudioSystem : public ISystem
    {
        DV_XMACRO_DECLARE_STATIC_CLASS(AudioSystem)

        public:
            static void Init();
            static void Free();
            static void Play(INode* const node);
    };
}

#endif