//
// Created by RGProgrammer on 4/1/2021.
//

#ifndef INTERACTIVE_MUSIC_PLAYER_AUDIODATAPROVIDER_H
#define INTERACTIVE_MUSIC_PLAYER_AUDIODATAPROVIDER_H
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <stdlib.h>
#include <string.h>

class AudioDataProvider {

public :
    AudioDataProvider();
    virtual ~AudioDataProvider();
    virtual void release()=0 ;
    virtual bool loadAudioFromFile( const char * filename , bool Async ) = 0 ;
    virtual bool loadAudioFromAssets(const char * filename, AAssetManager* manager, bool Async) = 0 ;
    virtual bool getData(int32_t numFrames , void* dataOut, int32_t* numFilledFrames)=0;
    virtual double getLengthInMillis()=0;
    virtual double getProgressInMillis()=0;
    virtual void setProgressInMillis(double millis)=0;
    virtual int getSampleRate()=0;
    virtual int getNumChannels ()=0;
    virtual bool endOfBuffer()=0;
    void resetCursor();
    bool isLooping();
    void setLooping(bool loop);
protected :
    virtual bool loadAudioFromBuffer(unsigned char * buffer , size_t size) = 0 ;

protected :

    bool            m_isLooping ;
    size_t          m_SelectedSample ;
    //length in Millis calculated

};


#endif //INTERACTIVE_MUSIC_PLAYER_AUDIODATAPROVIDER_H
