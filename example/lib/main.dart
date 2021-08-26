import 'package:flutter/material.dart';
import 'dart:async';

import 'package:flutter/services.dart';
import 'package:native_audio_player/native_audio_player.dart';

void main() {
  runApp(MyApp());
}

class MyApp extends StatefulWidget {
  @override
  _MyAppState createState() => _MyAppState();
}

class _MyAppState extends State<MyApp> {
 
  int _handle;
  double length = 0.0 ;

  @override
  void initState() {
    super.initState();
    initPlatformState();
  }

  // Platform messages are asynchronous, so we initialize in an async method.
  Future<void> initPlatformState() async {
    int handle = 0 ;
    // Platform messages may fail, so we use a try/catch PlatformException.
    try {
      handle = await NativeAudioPlayer.loadAudioFromAssets("assets/start.mp3");
      length = await NativeAudioPlayer.getLengthInMillis(handle);
    } on PlatformException {
      handle = 0;
    }

    // If the widget was removed from the tree while the asynchronous platform
    // message was in flight, we want to discard the reply rather than calling
    // setState to update our non-existent appearance.
    if (!mounted) return;

    setState(() {
      _handle = handle;
    });
  }

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: Scaffold(
        appBar: AppBar(
          title: const Text('Plugin example app'),
        ),
        body: Center(
          child: GestureDetector(child:Text('handle = $_handle length $length ms\n'),
            onTap:(){
                if(_handle!=0){
                  NativeAudioPlayer.playAudio(_handle);
                }
            }
          ),
        ),
      ),
    );
  }
}
