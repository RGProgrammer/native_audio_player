

#ifndef ANDROID_NATIVE_AUDIO_AUDIODATAPROVIDERHELPER_H
#define ANDROID_NATIVE_AUDIO_AUDIODATAPROVIDERHELPER_H
#include "AudioDataProvider.h"
class AudioDataProvider ;
class AudioDataProviderHelper {
public:
    enum Types{
        MP3,
        FLAC,
        OGG,
    };
    static AudioDataProvider * createAudioDataProvider(Types type);
};


#endif //ANDROID_NATIVE_AUDIO_AUDIODATAPROVIDERHELPER_H
