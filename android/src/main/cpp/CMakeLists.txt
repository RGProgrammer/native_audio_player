
project(android_native_audio)
cmake_minimum_required(VERSION 3.10.2)


include_directories ( "internal"
						"external/oboe"
						"external/decoders/minimp3" )

add_subdirectory("external/decoders")
add_subdirectory("external/oboe")


add_library( android_native_audio
             SHARED
            internal/AudioStreamCallBackImpl.cpp
			internal/AudioDataProvider.cpp
			internal/MP3AudioDataProvider.cpp
			internal/AudioDataProviderHelper.cpp
            android_native_audio.cpp
		)

find_library( android-lib
		android )
add_dependencies(android_native_audio
        minimp3
        oboe)
find_library( log-lib
              log )

target_link_libraries( android_native_audio
		minimp3
		oboe
		${log-lib}
		${android-lib} )