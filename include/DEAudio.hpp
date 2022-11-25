#ifndef _DE_LIB_AUDIO_H_
#define _DE_LIB_AUDIO_H_

#include "DECore.hpp"
#include "DEMath.hpp"
#include "thirdparty/DEThirdPartyAL.hpp"

namespace DvigEngine
{
    // From 'ffainelli.github.io/openal-example/'
    DV_FUNCTION_INLINE static ALenum to_al_format(deuint16 channels, deuint16 samples)
    {
        debool stereo = (channels > 1);

        switch (samples) {
            case 16:
                if (stereo) {
                    return AL_FORMAT_STEREO16;
                } else {
                    return AL_FORMAT_MONO16;
                }
            case 8:
                if (stereo) {
                    return AL_FORMAT_STEREO8;
                } else {
                    return AL_FORMAT_MONO8;
                }
            default:
                return -1;
        }
    }

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

            DV_FUNCTION_INLINE defloat32 GetVolume() { ALfloat value = 0; alGetSourcef(this->m_ALSource, AL_GAIN, &value); return value; }
            DV_FUNCTION_INLINE void SetVolume(defloat32 value) { alSourcef(this->m_ALSource, AL_GAIN, value); }

        public:
            ALuint m_ALSource;
            
        private:
            ALuint m_ALBuffer;
    };

    class AudioSystem : public ISystem
    {
        DV_XMACRO_DECLARE_STATIC_CLASS(AudioSystem)

        public:
            static void Init();
            static void Free();
            static void Play(INode* const node);

        private:
            static ALCdevice* m_Device;
            static ALCcontext* m_Context;
    };
}

#endif