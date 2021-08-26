//
// Created by RGProgrammer on 4/1/2021.
//

#include "AudioStreamCallBackImpl.h"



AudioStreamCallBackImpl::AudioStreamCallBackImpl(AudioDataProvider *data):m_Provider(data){
    if(m_Provider==NULL){
        __android_log_print(ANDROID_LOG_ERROR, "AudioStreamCallBAckImpl","no Source for audio data provided ");
    }
}

oboe::DataCallbackResult
AudioStreamCallBackImpl::onAudioReady(oboe::AudioStream *audioStream, void *audioData,
                                      int32_t numFrames) {

        if(!m_Provider){
            return oboe::DataCallbackResult::Stop ;
        }
        if( audioStream ->getDirection()== oboe::Direction::Output){
            if(m_Provider->endOfBuffer()) {
                if (!m_Provider->isLooping())
                    return oboe::DataCallbackResult::Stop;
                else {
                    m_Provider->resetCursor();
                    return oboe::DataCallbackResult::Continue;
                }
            }
            else{
                if(m_Provider->getData(numFrames,audioData,NULL))
                    return oboe::DataCallbackResult::Continue ;
                else{
                    return oboe::DataCallbackResult::Stop ;
                }
            }

        }else{
            return oboe::DataCallbackResult::Stop ;
        }
}

bool AudioStreamCallBackImpl::onError(oboe::AudioStream * , oboe::Result res) {
    __android_log_print(ANDROID_LOG_ERROR, "AudioStreamCallBackImpl","error text : %s", oboe::convertToText(res));
    return false ;
}

void AudioStreamCallBackImpl::onErrorBeforeClose(oboe::AudioStream *, oboe::Result res) {
    __android_log_print(ANDROID_LOG_ERROR, "AudioStreamCallBackImpl","error text : %s", oboe::convertToText(res));
}

void AudioStreamCallBackImpl::onErrorAfterClose(oboe::AudioStream *, oboe::Result res ) {
    __android_log_print(ANDROID_LOG_ERROR, "AudioStreamCallBackImpl","error text : %s", oboe::convertToText(res));
}
