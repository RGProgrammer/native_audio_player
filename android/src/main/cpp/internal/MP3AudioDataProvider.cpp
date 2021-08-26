//
// Created by RGProgrammer on 8/13/2021.
//

#include "MP3AudioDataProvider.h"

MP3AudioDataProvider::MP3AudioDataProvider():AudioDataProvider(),m_File(NULL),m_FileLen(0),m_FilePos(0),
        m_EncodedBuffer(NULL),m_EncodedBufferSize(0) {
    m_Info.buffer = NULL ;
};
MP3AudioDataProvider::~MP3AudioDataProvider() {
    this->release();
};

void MP3AudioDataProvider::release() {
    if(m_Info.buffer ){
        free(m_Info.buffer);
        m_Info.buffer=NULL ;
        m_Info.samples= 0 ;
    }
    if(m_EncodedBuffer){
        free(m_EncodedBuffer);
        m_EncodedBuffer = NULL ;
        m_EncodedBufferSize = 0 ;
    }
    m_SelectedSample= 0 ;
}

bool MP3AudioDataProvider::loadAudioFromFile( const char * filename , bool Async ) {
    this->release();
    if(!(m_File = fopen(filename,"rb"))){
        __android_log_print(ANDROID_LOG_ERROR,"MP3AudioDataProvider","error opening file %s",filename);
        return false;
    }
    m_FilePos = 0 ;
    m_FileLen = fseek(m_File,0,SEEK_END);
    m_EncodedBuffer =(unsigned char *)malloc(m_FileLen);
    m_EncodedBufferSize = fread (m_EncodedBuffer,m_FileLen,0,m_File);
    bool res = this->loadAudioFromBuffer(m_EncodedBuffer,m_EncodedBufferSize);

    //the encoded data is no longer needed
    fclose(m_File);
    m_File = NULL ;
    m_FilePos = 0 ;
    m_FileLen = 0 ;
    if(m_EncodedBuffer){
        free(m_EncodedBuffer);
        m_EncodedBuffer = NULL ;
        m_EncodedBufferSize = 0 ;
    }

    return res ;
};
bool  MP3AudioDataProvider::loadAudioFromAssets(const char * filename, AAssetManager* manager, bool Async){
    this->release();
    AAsset* file = AAssetManager_open(manager,filename,AASSET_MODE_BUFFER);
    if(!file){
        __android_log_print(ANDROID_LOG_ERROR,"MP3AudioDataProvider","error opening file %s",filename);
        return false ;
    }
    const void* data = AAsset_getBuffer(file);
    size_t size = AAsset_getLength(file);
    if(!data){
        __android_log_print(ANDROID_LOG_ERROR,"MP3AudioDataProvider","cannot retrieve file data %s",filename);
        return false ;
    }
    m_EncodedBuffer = (unsigned char *)malloc(size);
    memcpy(m_EncodedBuffer,data,size);
    m_EncodedBufferSize = size ;
    AAsset_close(file);
    bool res = this->loadAudioFromBuffer(m_EncodedBuffer,m_EncodedBufferSize);
    if(m_EncodedBuffer){
        free(m_EncodedBuffer);
        m_EncodedBuffer = NULL ;
        m_EncodedBufferSize = 0 ;
    }
    return res ;
}
bool  MP3AudioDataProvider::loadAudioFromBuffer(unsigned char * buffer , size_t size){

    if(mp3dec_detect_buf((unsigned char *)buffer,size)) {
        __android_log_print(ANDROID_LOG_ERROR,"MP3 decoding","not a mp3 data");
        return false;
    }
    else if(mp3dec_load_buf(&m_Mp3d, (unsigned char *)buffer, size, &m_Info, 0, 0)){
        __android_log_print(ANDROID_LOG_ERROR,"MP3 decoding","error decoding mp3 data");
        return false ;
    }
    return true ;
}

bool  MP3AudioDataProvider::getData(int32_t numFrames , void* dataOut, int32_t* numFilledFrames){
    if(numFrames == 0 || dataOut== NULL )
        return false ;

    if(m_SelectedSample== m_Info.samples){
        if(numFilledFrames){
            *numFilledFrames= 0 ;
        }
        return false;
    }

    //the size of data to copy
    int n = m_Info.samples - m_SelectedSample ;
    if(n > numFrames*m_Info.channels)
        n=numFrames*m_Info.channels ;

    memcpy(dataOut, m_Info.buffer+m_SelectedSample*getNumChannels(),n*getNumChannels()*sizeof(float));
    //increment pointer
    m_SelectedSample+=n ;
    //return number of filled frames
    if(numFilledFrames){
        *numFilledFrames= n ;
    }
    return true;
}
double  MP3AudioDataProvider::getLengthInMillis(){
    if(m_Info.hz==0 ||m_Info.samples==0)
        return 0.0;
    return ((double)m_Info.samples/m_Info.hz)/getNumChannels()* 1000 ;
}
double  MP3AudioDataProvider::getProgressInMillis(){
    if (m_Info.hz== 0 )
        return 0 ;
    return ((double)m_SelectedSample /m_Info.hz)/getNumChannels()* 1000 ;
}
void  MP3AudioDataProvider::setProgressInMillis(double millis){
    if( m_Info.buffer==NULL )
        return ;
    uint index = (uint)((millis/1000)*m_Info.hz*getNumChannels()) ;
    if (index < m_Info.samples &&  (index+1)%getNumChannels()==0)
        m_SelectedSample = index ;
}
int  MP3AudioDataProvider::getSampleRate(){
    return m_Info.hz ;
}
int  MP3AudioDataProvider::getNumChannels (){
    return m_Info.channels ;
}

bool  MP3AudioDataProvider::endOfBuffer(){
    return m_SelectedSample == m_Info.samples;
}




