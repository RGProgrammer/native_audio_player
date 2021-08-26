
import 'dart:async';

import 'package:flutter/services.dart';

class NativeAudioPlayer {
  static const MethodChannel _channel =
      const MethodChannel('native_audio_player');

  static Future<int>  loadAudioFromAssets(String filename) async {
    final int handle = await _channel.invokeMethod('loadAudioFromAssets',filename);
    return handle;
  }

   static Future<void>  playAudio(int handle) async {
      await _channel.invokeMethod('playAudio',handle);
  }
  static Future<double>  getLengthInMillis(int handle) async {
      return await _channel.invokeMethod('getLengthInMillis',handle);
  }
}
