#import "NativeAudioPlayerPlugin.h"
#if __has_include(<native_audio_player/native_audio_player-Swift.h>)
#import <native_audio_player/native_audio_player-Swift.h>
#else
// Support project import fallback if the generated compatibility header
// is not copied when this plugin is created as a library.
// https://forums.swift.org/t/swift-static-libraries-dont-copy-generated-objective-c-header/19816
#import "native_audio_player-Swift.h"
#endif

@implementation NativeAudioPlayerPlugin
+ (void)registerWithRegistrar:(NSObject<FlutterPluginRegistrar>*)registrar {
  [SwiftNativeAudioPlayerPlugin registerWithRegistrar:registrar];
}
@end
