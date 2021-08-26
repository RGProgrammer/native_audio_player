

#ifndef ANDROID_NATIVE_AUDIO_MP3AUDIODATAPROVIDER_H
#define ANDROID_NATIVE_AUDIO_MP3AUDIODATAPROVIDER_H


#include "AudioDataProvider.h"
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <android/log.h>
#include <stdlib.h>
#include <string.h>

#include <minimp3_ex.h>


class MP3AudioDataProvider : public AudioDataProvider {
public :
    MP3AudioDataProvider();
    virtual ~MP3AudioDataProvider();
    virtual void release();
    virtual bool loadAudioFromFile( const char * filename , bool Async ) ;
    virtual bool loadAudioFromAssets(const char * filename, AAssetManager* manager, bool Async);
    bool getData(int32_t numFrames , void* dataOut, int32_t* numFilledFrames);
    double getLengthInMillis();
    double getProgressInMillis();
    void setProgressInMillis(double millis);
    int getSampleRate();
    int getNumChannels ();
    bool endOfBuffer();

protected:
    virtual bool loadAudioFromBuffer(unsigned char * buffer , size_t size) ;

private :
    mp3dec_t            m_Mp3d;
    mp3dec_file_info_t   m_Info;
    FILE*               m_File ;
    size_t              m_FilePos ;
    size_t              m_FileLen ;
    unsigned char*      m_EncodedBuffer ;
    size_t              m_EncodedBufferSize ;

};


#endif //ANDROID_NATIVE_AUDIO_MP3AUDIODATAPROVIDER_H
