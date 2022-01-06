
# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.10.2)

# Declares and names the project.

project("pocketcam")

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.
file(GLOB jni_src "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

file(GLOB camera_src "${CMAKE_CURRENT_SOURCE_DIR}/camera/*.cpp")
file(GLOB camera_common_src "${CMAKE_CURRENT_SOURCE_DIR}/camera/common/*.cpp")

file(GLOB gl_src "${CMAKE_CURRENT_SOURCE_DIR}/gl/*.cpp")
file(GLOB gl_primatives_src "${CMAKE_CURRENT_SOURCE_DIR}/gl/primatives/*.cpp")

add_library(pocketcam
            SHARED
            ${CMAKE_CURRENT_SOURCE_DIR}/camera_preview.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/jni_camera.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/jni_gl.cpp
            ${camera_src}
            ${camera_common_src}
            ${gl_src}
            ${gl_primatives_src})

target_include_directories(pocketcam
                           PRIVATE
                           ${CMAKE_CURRENT_SOURCE_DIR}
                           ${CMAKE_CURRENT_SOURCE_DIR}/camera/
                           ${CMAKE_CURRENT_SOURCE_DIR}/camera/common/
                           ${CMAKE_CURRENT_SOURCE_DIR}/gl/
                           ${CMAKE_CURRENT_SOURCE_DIR}/gl/shaders
                           ${CMAKE_CURRENT_SOURCE_DIR}/gl/primatives)

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.
find_library(log-lib log )
find_library(camera-lib camera2ndk)
find_library(media-lib mediandk)

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, pgirebuilt third-party libraries, or system libraries.

# GLES must be linked directly, find_library defaults to GLESv2 which
# doesn't implement support for Vertex Arrays

target_link_libraries( # Specifies the target library.
        pocketcam
        # Links the target library to the log library
        # included in the NDK.
        ${log-lib}
        ${camera-lib}
        ${media-lib}
        GLESv3
        android
        m)
