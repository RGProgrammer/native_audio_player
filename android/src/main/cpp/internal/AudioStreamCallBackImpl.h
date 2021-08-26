//
// Created by RGProgrammer on 4/1/2021.
//

#ifndef INTERACTIVE_MUSIC_PLAYER_AUDIOSTREAMCALLBACKIMPL_H
#define INTERACTIVE_MUSIC_PLAYER_AUDIOSTREAMCALLBACKIMPL_H
#include <oboe/Oboe.h>
#include <android/log.h>
#include "AudioDataProvider.h"

class AudioStreamCallBackImpl : public oboe::AudioStreamCallback{

public :

    AudioStreamCallBackImpl(AudioDataProvider * data);


     oboe::DataCallbackResult onAudioReady(
            oboe::AudioStream *audioStream,
            void *audioData,
            int32_t numFrames);



     bool onError(oboe::AudioStream* /* audioStream */, oboe::Result /* error */) ;

     void onErrorBeforeClose(oboe::AudioStream* /* audioStream */, oboe::Result /* error */) ;

     void onErrorAfterClose(oboe::AudioStream* /* audioStream */, oboe::Result /* error */) ;
private :
    AudioDataProvider * m_Provider ;
};


#endif //INTERACTIVE_MUSIC_PLAYER_AUDIOSTREAMCALLBACKIMPL_H
