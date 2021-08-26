package com.rgpro.native_audio_player

import androidx.annotation.NonNull

import io.flutter.embedding.engine.plugins.FlutterPlugin
import io.flutter.plugin.common.MethodCall
import io.flutter.plugin.common.MethodChannel
import io.flutter.plugin.common.MethodChannel.MethodCallHandler
import io.flutter.plugin.common.MethodChannel.Result
import io.flutter.plugin.common.PluginRegistry.Registrar
import io.flutter.embedding.engine.loader.FlutterLoader
import io.flutter.FlutterInjector;
import android.content.Context
import android.content.Context.AUDIO_SERVICE
import android.content.res.AssetManager
import android.media.AudioManager

/** NativeAudioPlayerPlugin */
class NativeAudioPlayerPlugin: FlutterPlugin, MethodCallHandler {

  private lateinit var channel : MethodChannel
  private lateinit var audioItf : NativeAudioItf 
  private lateinit var context : Context
  private lateinit var manager : AssetManager

  override fun onAttachedToEngine(@NonNull flutterPluginBinding: FlutterPlugin.FlutterPluginBinding) {
    channel = MethodChannel(flutterPluginBinding.binaryMessenger, "native_audio_player")
    channel.setMethodCallHandler(this)
    audioItf = NativeAudioItf()
    context = flutterPluginBinding.getApplicationContext()
    manager = context.assets
    val myAudioMgr : AudioManager = context.getSystemService(AUDIO_SERVICE) as AudioManager
    val devices = myAudioMgr.getDevices(AudioManager.GET_DEVICES_OUTPUTS)
    audioItf.init(devices[0].id)
  }

  override fun onMethodCall(@NonNull call: MethodCall, @NonNull result: Result) {
    if (call.method == "loadAudioFromAssets") {
      val filename = call.arguments as String
      val loader: FlutterLoader = FlutterInjector.instance().flutterLoader()
      val key: String = loader.getLookupKeyForAsset(filename)
      result.success(audioItf.loadAudioFromAssets(key,manager))
    } else if (call.method == "playAudio") {
      val handle = call.arguments as Int
      if(handle != 0){
        audioItf.playAudio(handle)
        result.success(null)
      }
      else{
        result.error("audio api","invalid handle value $handle",null);
      }
    }else if(call.method == "getLengthInMillis"){
      val handle = call.arguments as Int
      if(handle != 0){
        result.success(audioItf.getLengthInMillis(handle))
      }
      else{
        result.error("audio api","invalid handle value $handle",null);
      }
    } 
    else {
      result.error("audio api","unhandled call ${call.method} with the arguments ${call.arguments}",null)
    }
  }

  override fun onDetachedFromEngine(@NonNull binding: FlutterPlugin.FlutterPluginBinding) {
    channel.setMethodCallHandler(null)
    audioItf.terminate()
  }
}
