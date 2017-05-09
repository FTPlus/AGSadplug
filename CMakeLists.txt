cmake_minimum_required (VERSION 2.8.2)

project(AGSadplug)

if (WIN32)
	# Compile runtime statically so that the dll does not require the redist kit
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
	
	include_directories(libs/adplug)
	link_directories(libs/adplug)
	include_directories(libs/RtAudio)
	link_directories(libs/RtAudio)
endif()

add_definitions(-DTHIS_IS_THE_PLUGIN=1)
add_library(agsadp SHARED src/agsplugin.cpp src/agsadp.cpp src/sound.cpp res/agsadp.rc)

if (WIN32)
	target_link_libraries(agsadp adplug.lib libbinio.lib rtaudio_static.lib)
else()
	target_link_libraries(agsadp adplug binio rtaudio)
endif()

