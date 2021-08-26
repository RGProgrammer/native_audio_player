
#include "AudioDataProviderHelper.h"
#include "MP3AudioDataProvider.h"
AudioDataProvider *
AudioDataProviderHelper::createAudioDataProvider(AudioDataProviderHelper::Types type) {
    switch (type) {
        case MP3 : return new MP3AudioDataProvider();

        default: return 0 ;

    }
}
