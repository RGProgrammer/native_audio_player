package com.rgpro.native_audio_player

import android.content.res.AssetManager

class NativeAudioItf {

    external fun init(device : Int):Boolean
    external fun terminate()
    external fun loadAudioFromAssets(filename:String , manager :AssetManager):Int
    external fun loadAudioFromStorage(filename:String ):Int
    external fun isValidAudioHandle(handle :Int): Boolean
    external fun removeAudio(jandle :Int):Boolean
    external fun clearAll()
    external fun getProgressInMillis(handle : Int) : Double
    external fun getLengthInMillis(handle : Int) : Double
    external fun seek(handle: Int,progress : Double)
    external fun playAudio(handle: Int ,looping : Boolean = false )
    external fun isPlaying(handle: Int):Boolean
    external fun isLooping(handle: Int):Boolean
    external fun pauseAudio(handle: Int )
    external fun stopAudio(handle: Int )
    external fun setVolume(handle: Int ,volume : Double)

    companion object {
        // Used to load the 'native-lib' library on application startup.
        init {
            System.loadLibrary("android_native_audio")
        }
    }
}