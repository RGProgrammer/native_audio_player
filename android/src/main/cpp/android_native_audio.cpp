#include <jni.h>
#include <string>
#include <android/log.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <cstdio>
#include <oboe/Oboe.h>
#include <AudioStreamCallBackImpl.h>
#include <AudioDataProviderHelper.h>
#ifdef __cplusplus
extern "C" {
#endif

#define MAXAUDIOSOURCES 20


typedef struct {
    oboe::AudioStream *audioOutStream = NULL;
    AudioDataProvider *provider = NULL;
    AudioStreamCallBackImpl *impl = NULL;
} AudioPlayerStruct;

oboe::AudioStreamBuilder builder; // only one instance need since stream creation a sync task
int selectedDevice = -1;

AudioPlayerStruct listAudio[MAXAUDIOSOURCES];

JNIEXPORT jboolean  JNICALL
Java_com_rgpro_native_1audio_1player_NativeAudioItf_init(
        JNIEnv *env,
        jobject /* this */, jint deviceId
) {

    selectedDevice = deviceId;
    for(int i = 0 ; i < MAXAUDIOSOURCES ; i++){
        listAudio[i].audioOutStream = NULL ;
        listAudio[i].provider = NULL ;
        listAudio[i].impl = NULL ;

    }
    return true;
} ;

JNIEXPORT void  JNICALL
Java_com_rgpro_native_1audio_1player_NativeAudioItf_terminate(
        JNIEnv *env,
        jobject /* this */ obj
) {
    for (int i = 0; i < MAXAUDIOSOURCES; i++) {
        if (listAudio[i].audioOutStream) {
            listAudio[i].audioOutStream->close();
            delete listAudio[i].audioOutStream;
            listAudio[i].audioOutStream = NULL;
        }
        if (listAudio[i].provider) {
            delete listAudio[i].provider;
            listAudio[i].provider = NULL;
        }
        if (listAudio[i].impl) {
            delete listAudio[i].impl;
            listAudio[i].impl = NULL;
        }
    }

} ;

JNIEXPORT jint  JNICALL
Java_com_rgpro_native_1audio_1player_NativeAudioItf_loadAudioFromAssets(
        JNIEnv *env,
        jobject /* this */,
        jstring filename,
        jobject assetManager) {

    AAssetManager *manager = AAssetManager_fromJava(env, assetManager);
    if (filename == NULL || manager == NULL)
        return 0;

    //find a empty place
    int index = 0 ;
    for( ; index <MAXAUDIOSOURCES ; index++){
        if(listAudio[ index].audioOutStream == NULL)
            break ;
    }
    if(index == MAXAUDIOSOURCES){
        return 0 ;
    }
    const char *utfFilename = env->GetStringUTFChars(filename, 0);
    const char* extension = strrchr(utfFilename,'.');
    //create the correct provider
    if(strcmp(extension,".mp3")==0){
      listAudio[index].provider = AudioDataProviderHelper::createAudioDataProvider(AudioDataProviderHelper::MP3);
      if(listAudio[index].provider->loadAudioFromAssets(utfFilename,manager,false)==false){
          delete listAudio[index].provider ;
          listAudio[index] .provider =NULL;
          __android_log_print(ANDROID_LOG_ERROR,"Native Audio Api","error decoding file  %s",utfFilename);
          env->ReleaseStringUTFChars(filename,utfFilename);
          return 0 ;
      }
    }
    else{
        __android_log_print(ANDROID_LOG_ERROR,"Native Audio Api","file is not supported %s",utfFilename);
        env->ReleaseStringUTFChars(filename,utfFilename);
        return 0 ;
    }
    env->ReleaseStringUTFChars(filename,utfFilename);
    //create callbacks
    listAudio[index] .impl= new AudioStreamCallBackImpl(listAudio[index].provider);
    //create audio out stream
    builder.setFormat(oboe::AudioFormat::Float)
            ->setDirection(oboe::Direction::Output)
            ->setDeviceId(selectedDevice)
            ->setChannelCount(listAudio[index].provider->getNumChannels())
            ->setSampleRate(listAudio[index].provider->getSampleRate())
            ->setAudioApi((oboe::AudioStreamBuilder::isAAudioRecommended()?
                           oboe::AudioApi::AAudio:
                           oboe::AudioApi::OpenSLES))
            ->setSharingMode(oboe::SharingMode::Exclusive)
            ->setPerformanceMode(oboe::PerformanceMode::LowLatency)
            ->setDataCallback( listAudio[index].impl)
            ->setErrorCallback( listAudio[index].impl);
    oboe::Result res = builder.openStream(&listAudio[index].audioOutStream);
    if(res!=oboe::Result::OK){
        __android_log_print(ANDROID_LOG_ERROR,"JNI","error creating audioStream : %s",oboe::convertToText(res));
        delete listAudio[index].provider ;
        listAudio[index].provider =NULL;
        listAudio[index].audioOutStream =NULL;
        return 0 ;
    }
    return index + 1;
} ;


JNIEXPORT jint  JNICALL
Java_com_rgpro_native_1audio_1player_NativeAudioItf_loadAudioFromStorage(
        JNIEnv *env,
        jobject /* this */,
        jstring filename) {


    if (filename == NULL)
        return 0;
    //find a empty place
    int index = 0 ;
    for( ; index <MAXAUDIOSOURCES ; index++){
        if(listAudio[ index].audioOutStream == NULL)
            break ;
    }
    if(index == MAXAUDIOSOURCES){
        return 0 ;
    }


    const char *utfFilename = env->GetStringUTFChars(filename, 0);
    const char* extension = strrchr(utfFilename,'.');
    //create the correct provider
    if(strcmp(extension,".mp3")==0){
        listAudio[index].provider = AudioDataProviderHelper::createAudioDataProvider(AudioDataProviderHelper::MP3);
        if(!listAudio[index].provider->loadAudioFromFile(utfFilename,false)){
            delete listAudio[index].provider ;
            listAudio[index].provider =NULL;
            return 0 ;
        }
    }
    else{
        __android_log_print(ANDROID_LOG_ERROR,"Native Audio Api","file is not supported %s",utfFilename);
        env->ReleaseStringUTFChars(filename,utfFilename);
        return 0 ;
    }
    env->ReleaseStringUTFChars(filename,utfFilename);
    //create audio out stream
    builder.setFormat(oboe::AudioFormat::Float)
            ->setDirection(oboe::Direction::Output)
            ->setDeviceId(selectedDevice)
            ->setChannelCount(listAudio[index].provider->getNumChannels())
            ->setSampleRate(listAudio[index].provider->getSampleRate())
            ->setAudioApi((oboe::AudioStreamBuilder::isAAudioRecommended()?
                           oboe::AudioApi::AAudio:
                           oboe::AudioApi::OpenSLES))
            ->setSharingMode(oboe::SharingMode::Exclusive)
            ->setPerformanceMode(oboe::PerformanceMode::LowLatency)
            ->setDataCallback( listAudio[index].impl)
            ->setErrorCallback( listAudio[index].impl);
    oboe::Result res = builder.openStream(&listAudio[index].audioOutStream);
    if(res!=oboe::Result::OK){
        __android_log_print(ANDROID_LOG_ERROR,"JNI","error creating audioStream : %s",oboe::convertToText(res));
        delete listAudio[index].provider ;
        listAudio[index].provider =NULL;
        listAudio[index].audioOutStream =NULL;
        return 0 ;
    }else{
        __android_log_print(ANDROID_LOG_DEBUG,"JNI","created audioStream : %s",oboe::convertToText(res));
    }
    return index + 1;
} ;

JNIEXPORT jboolean  JNICALL
Java_com_rgpro_native_1audio_1player_NativeAudioItf_isValidAudioHandle(
        JNIEnv *env,
        jobject /* this */,
        jint handle
) {
    if (handle <= 0 || handle>MAXAUDIOSOURCES)
        return false;
    if (listAudio[handle - 1].audioOutStream == NULL)
        return false;
    return true;
}

JNIEXPORT jboolean  JNICALL
Java_com_rgpro_native_1audio_1player_NativeAudioItf_removeAudio(
        JNIEnv *env,
        jobject /* this */,
        jint handle
) {
    if (handle <= 0 || handle > MAXAUDIOSOURCES)
        return false;
    if (listAudio[handle - 1].provider) {
        listAudio[handle - 1].provider->release();
        delete listAudio[handle-1].provider ;
        listAudio[handle-1].provider=NULL;
    }
    if (listAudio[handle - 1].audioOutStream) {
        listAudio[handle - 1].audioOutStream->close();
        delete listAudio[handle-1].audioOutStream ;
        listAudio[handle-1].audioOutStream=NULL;
    }
    return true;
}

JNIEXPORT void  JNICALL
Java_com_rgpro_native_1audio_1player_NativeAudioItf_clearAll(
        JNIEnv *env,
        jobject /* this */
) {
    for (int i = 0; i < MAXAUDIOSOURCES; i++) {
        if (listAudio[i].audioOutStream) {
            listAudio[i].audioOutStream->close();
            delete listAudio[i].audioOutStream;
            listAudio[i].audioOutStream = NULL;
        }
        if (listAudio[i].provider) {
            delete listAudio[i].provider;
            listAudio[i].provider = NULL;
        }
    }
}
JNIEXPORT jdouble  JNICALL
Java_com_rgpro_native_1audio_1player_NativeAudioItf_getProgressInMillis(
        JNIEnv *env,
        jobject /* this */,
        jint handle
) {
    if (handle <= 0 || handle > MAXAUDIOSOURCES)
        return 0.0;
    if (listAudio[handle - 1].provider)
        return listAudio[handle - 1].provider->getProgressInMillis();
    return 0.0;
}

JNIEXPORT jdouble  JNICALL
Java_com_rgpro_native_1audio_1player_NativeAudioItf_getLengthInMillis(
        JNIEnv *env,
        jobject /* this */,
        jint handle
) {
    if (handle <= 0 || handle > MAXAUDIOSOURCES)
        return 0.0;
    if (listAudio[handle - 1].provider)
        return listAudio[handle - 1].provider->getLengthInMillis();
    return 0.0;
}

JNIEXPORT void  JNICALL
Java_com_rgpro_native_1audio_1player_NativeAudioItf_seek(
        JNIEnv *env,
        jobject /* this */,
        jint handle,
        jdouble progress
) {

    if (handle <= 0 || handle > MAXAUDIOSOURCES)
        return;
    if (listAudio[handle - 1].provider)
        listAudio[handle - 1].provider->setProgressInMillis(progress);
}
JNIEXPORT void  JNICALL
Java_com_rgpro_native_1audio_1player_NativeAudioItf_playAudio(
        JNIEnv *env,
        jobject /* this */,
        jint handle,
        jboolean looping) {
    if (handle <= 0 || handle > MAXAUDIOSOURCES)
        return;
    if (listAudio[handle - 1].audioOutStream == NULL)
        return;

    if (listAudio[handle - 1].audioOutStream) {
        listAudio[handle - 1].audioOutStream->start();
        listAudio[handle - 1].provider->resetCursor();

    }
}
JNIEXPORT jboolean  JNICALL
Java_com_rgpro_native_1audio_1player_NativeAudioItf_isPlaying(
        JNIEnv *env,
        jobject /* this */,
        jint handle
) {
    if (handle <= 0 || handle > MAXAUDIOSOURCES)
        return false;
    if (listAudio[handle - 1].audioOutStream == NULL)
        return false;
    if (listAudio[handle - 1].audioOutStream) {
        return listAudio[handle - 1].audioOutStream->getState() == oboe::StreamState::Started;
    }
    return false;
}

JNIEXPORT jboolean  JNICALL
Java_com_rgpro_native_1audio_1player_NativeAudioItf_isLooping(
        JNIEnv *env,
        jobject /* this */,
        jint handle
) {

    if (handle <= 0 || handle > MAXAUDIOSOURCES)
        return false;
    if (listAudio[handle - 1].provider == NULL)
        return false;

    return listAudio[handle - 1].provider->isLooping() ;

}

JNIEXPORT void  JNICALL
Java_com_rgpro_native_1audio_1player_NativeAudioItf_stopAudio(
        JNIEnv *env,
        jobject /* this */,
        jint handle) {

    if (handle <= 0 || handle > MAXAUDIOSOURCES)
        return;
    if (listAudio[handle - 1].audioOutStream) {
        listAudio[handle - 1].audioOutStream->stop();
    }
}

JNIEXPORT void  JNICALL
Java_com_rgpro_native_1audio_1player_NativeAudioItf_pauseAudio(
        JNIEnv *env,
        jobject /* this */,
        jint handle
) {
    if (handle <= 0 || handle > MAXAUDIOSOURCES)
        return;
    if (listAudio[handle - 1].audioOutStream) {
        listAudio[handle - 1].audioOutStream->pause();
    }
}

JNIEXPORT void  JNICALL
Java_com_rgpro_native_1audio_1player_NativeAudioItf_setVolume(
        JNIEnv *env,
        jobject /* this */,
        jint handle,jdouble volume
) {
    if (handle <= 0 || handle > MAXAUDIOSOURCES)
        return;
    if (listAudio[handle - 1].audioOutStream) {
        listAudio[handle - 1].audioOutStream->pause();
    }
}

#ifdef __cplusplus
}
#endif


