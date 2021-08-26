
#include "AudioDataProvider.h"

AudioDataProvider::AudioDataProvider(): m_SelectedSample(0),m_isLooping(false)
{
}

AudioDataProvider::~AudioDataProvider() {
   release();
}

void AudioDataProvider::resetCursor() {
    m_SelectedSample= 0 ;
}

bool AudioDataProvider::isLooping() {
    return m_isLooping;
}

void AudioDataProvider::setLooping(bool loop) {
    m_isLooping = loop ;
}





